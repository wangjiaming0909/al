#include "raft_instance.h"
#include "raft.grpc.pb.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <glog/logging.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

namespace raft {
using std::make_unique;

RaftInstance::~RaftInstance() {}
RaftInstance::RaftInstance(const Uuid &uuid, const std::string &listen_addr)
    : uuid_(uuid), listen_addr_(listen_addr), peers_(), role_(Role::Candidate),
      term_id_(0), master_uuid_(), master_term_id_(0),
      rpc_server_(new RaftRpcServer(this, listen_addr_)) {}

PeerInfo::PeerInfo(const Uuid &uuid, const std::string &addr)
    : addr_(addr), id_(uuid) {}

bool RaftInstance::add_peer(PeerPtr peer) {
  auto it = peer_map().find(peer->uuid());
  if (it != peer_map().end())
    return false;

  peers_[peer->uuid()] = std::move(peer);
  return true;
}

const Uuid& RaftInstance::uuid() const {
  return uuid_;
}

bool RaftInstance::add_peer(const Uuid &uuid, const std::string &addr) {
  auto it = peer_map().find(uuid);
  if (it != peer_map().end())
    return false;
  PeerInfo info{uuid, addr};
  PeerPtr peer_ptr = std::make_unique<Peer>(info);
  peers_[uuid] = std::move(peer_ptr);
  return true;
}

void RaftInstance::grant(const raft_pb::VoteRequest &request,
                         raft_pb::VoteReply &reply) {
  role_ = Role::Learner;
  master_uuid_ = request.candidate_id();
  master_term_id_ = request.candidate_term_id();

  LOG(INFO) << uuid() << " granted " << master_uuid_
            << " to be leader, master term id: " << master_term_id_;

  // prepare the reply
  reply.set_responder_uuid(uuid_);
  reply.set_vote_granted(true);
  reply.set_responder_term_id(term_id_);
}

void RaftInstance::deny(const raft_pb::VoteRequest &request,
                        raft_pb::VoteReply &reply) {
  LOG(INFO) << uuid() << " denied " << master_uuid_
            << " to be leader my term id: " << term_id_
            << " candidate term id: " << request.candidate_term_id();
  // prepare the reply
  reply.set_responder_uuid(uuid_);
  reply.set_vote_granted(false);
  reply.set_responder_term_id(term_id_);
}

grpc::Status RaftInstance::request_vote(const raft_pb::VoteRequest &request,
                                        raft_pb::VoteReply &reply) {
  assert(request.dest_id() == uuid_);
  auto term_id = request.candidate_term_id();
  // 1. if we have leader and uuid is not same as the candidate uuid, we deny
  // this vote

  // 2. if candidate is fall behind, we deny this vote
  if (term_id <= term_id_)
    deny(request, reply);

  // 3. if we have no leader, but we already granted someone
  //   a. if same as candidate, we grant again
  //   b. if not, we deny it

  // 4. check last oper id of candidate

  grant(request, reply);
  return ::grpc::Status::OK;
}

grpc::Status Peer::request_vote(const raft_pb::VoteRequest &request,
                                raft_pb::VoteReply &reply) {
  return client_->request_vote(request, reply);
}

Peer::Peer(const PeerInfo &info)
    : info_(info), client_(new RaftRpcClient{info.addr_}) {}

Peer::~Peer() {}

/*********Raft Service***********/

class RaftServiceImpl : public raft_pb::RaftRpc::Service {
  virtual ::grpc::Status request_vote(::grpc::ServerContext *context,
                                      const ::raft_pb::VoteRequest *request,
                                      ::raft_pb::VoteReply *response) {
    return server_->raft_instance()->request_vote(*request, *response);
  }

  RaftRpcServer *server_;

public:
  RaftServiceImpl(RaftRpcServer *server) : server_(server) {}
};

RaftService::RaftService(RaftRpcServer *server)
    : impl_(new RaftServiceImpl{server}) {}
RaftService::~RaftService() { delete impl_; }
/*********Raft Service***********/

/*********Raft Server***********/
RaftRpcServer::RaftRpcServer(RaftInstance *instance,
                             const std::string &listen_addr)
    : service_(this), server_{nullptr}, listen_addr_(listen_addr),
      uid_(instance->uuid()), raft_instance_(instance) {}

void RaftRpcServer::start() {
  if (server_) {
    return;
  }
  grpc::ServerBuilder builder;
  builder.RegisterService(service_.get_service());
  builder.AddListeningPort(listen_addr_, grpc::InsecureServerCredentials());
  server_ = std::move(builder.BuildAndStart());
}

void RaftRpcServer::wait() { server_->Wait(); }

void RaftRpcServer::shutdown() { server_->Shutdown(); }
/*********Raft Server***********/

/**********Raft Client*********/
class RaftStubImpl : public IRaftProtocol {
  std::unique_ptr<raft_pb::RaftRpc::Stub> stub_;

public:
  RaftStubImpl(const std::string &addr)
      : stub_(raft_pb::RaftRpc::NewStub(::grpc::CreateChannel(
            addr, ::grpc::InsecureChannelCredentials()))) {}

  inline virtual ::grpc::Status
  request_vote(const raft_pb::VoteRequest &request, raft_pb::VoteReply &reply) {
    ::grpc::ClientContext context;
    return stub_->request_vote(&context, request, &reply);
  }
};

RaftRpcClient::RaftRpcClient(const std::string &addr) : stub_(new Stub(addr)) {}

::grpc::Status
RaftRpcClient::Stub::request_vote(const raft_pb::VoteRequest &request,
                                  raft_pb::VoteReply &reply) {
  return stub_->request_vote(request, reply);
}

grpc::Status RaftRpcClient::request_vote(const raft_pb::VoteRequest &request,
                                         raft_pb::VoteReply &reply) {
  return this->stub_->request_vote(request, reply);
}

RaftRpcClient::~RaftRpcClient() {}

RaftRpcClient::Stub::Stub(const std::string &addr)
    : stub_(new RaftStubImpl{addr}) {}
RaftRpcClient::Stub::~Stub() {}
/**********Raft Client*********/

/******************leader election****************/

LeaderElectionResult::LeaderElectionResult(size_t voter_num)
    : voter_num_(voter_num) {}

LeaderElection::LeaderElection(std::shared_ptr<RaftInstance> instance)
    : instance_(instance) {}

LeaderElectionResult LeaderElection::elect() {
  auto ins = instance_.lock();
  if (!ins) {
    LOG(ERROR) << "when electing leader, instance has been deleted...";
    return LeaderElectionResult(0);
  }

  LeaderElectionResult res{ins->peer_map().size() + 1};
  ins->inc_term_id();

  // grant self
  res.grant(ins->uuid());
  if (res.decided())
    return res;

  // start to request_vote sync
  for (auto &peer : ins->peer_map()) {
    raft_pb::VoteRequest req;
    raft_pb::VoteReply reply;
    req.set_candidate_id(ins->uuid());
    req.set_candidate_term_id(ins->term_id());
    req.set_dest_id(peer.first);
    ::grpc::Status status = peer.second->request_vote(req, reply);

    handle_reply(status, reply, res);
  }

  return res;
}

void LeaderElection::handle_reply(::grpc::Status rpc_status,
                                  const raft_pb::VoteReply &reply,
                                  LeaderElectionResult &res) {
  LOG(INFO) << "handle reply from: [" << reply.responder_uuid()
            << "] rpc_status: [" << rpc_status.error_message() << "] reply: ["
            << reply.DebugString() << "]";
  auto ins = instance_.lock();
  if (!ins) {
    LOG(ERROR) << "instance has been deleted...";
    return;
  }
  if (rpc_status.ok()) {
    if (reply.vote_granted()) {
      res.grant(reply.responder_uuid());
      LOG(INFO) << "leader granted for [" << ins->uuid() << "] from: ["
                << reply.responder_uuid() << "]";
    } else {
      res.deny(reply.responder_uuid());
      LOG(INFO) << "leader denied for [" << ins->uuid() << "] from: ["
                << reply.responder_uuid() << "]";
    }
  } else {
    res.deny(reply.responder_uuid());
    LOG(INFO) << "leader denied for [" << ins->uuid() << "] from: ["
              << reply.responder_uuid() << "] err: ["
              << rpc_status.error_message() << "]";
  }
}

bool LeaderElectionResult::decided() const { return granted() || denied(); }
bool LeaderElectionResult::granted() const {
  return granted_peers_.size() >= majority_num();
}
bool LeaderElectionResult::denied() const {
  return (denied_peers_.size() + err_peers_.size()) >= majority_num();
}

EventTimerImpl::EventTimerImpl(Reactor *reactor) : TimerImpl(reactor) {}

int EventTimerImpl::start(Period period, TimerCallBack cb) {
  /*
  if (!base_)
    return -1;
  this->cb_ = cb;

  struct TimerCbCtx {
    std::weak_ptr<EventTimerImpl> impl;
  };

  auto *ctx = new TimerCbCtx{};
  if (!ctx)
    return -1;
  ctx->impl = shared_from_this();

  auto te_cb = [](int, short int, void *ctx) {
    TimerCbCtx *c = (TimerCbCtx *)ctx;
    auto timer = c->impl;
    auto impl = timer.lock();
    if (impl)
      impl->cb_();
    delete c;
  };

  e_ = evtimer_new(base_, te_cb, ctx);
  if (!e_)
    return -1;

  struct timeval t;
  t.tv_usec = period.count() * 1000;
  evtimer_add(e_, &t);
  */

  return 0;
}

int EventTimerImpl::snooze(Period period) {

}

int EventTimerImpl::stop() {

}

EventReactorImpl::EventReactorImpl() : base_(event_base_new()) {}

EventReactorImpl::~EventReactorImpl() {
  if (base_)
    event_base_free(base_);
}

int EventReactorImpl::register_event(int fd, IReactor::Event e, int opts, CallBack cb) {
  switch (e) {
  case IReactor::Event::READ: {
    auto *be = bufferevent_socket_new(base_, fd, opts);
    if (!be) return -1;
    return bufferevent_enable(be, EV_READ);
    break;
  }
  case IReactor::Event::WRITE: {
    auto *be = bufferevent_socket_new(base_, fd, opts);
    if (!be) return -1;
    return bufferevent_enable(be, EV_WRITE);
    break;
  }
  case IReactor::Event::TIMEOUT: {
    struct Ctx {
      CallBack cb;
    };
    Ctx* c = new Ctx();
    c->cb = cb;
    auto te_cb = [](int, short int, void *ctx) {
      ((Ctx *)ctx)->cb();
      delete (Ctx*)ctx;
    };
    auto *e = evtimer_new(base_, te_cb, c);
    break;
  }
  default:
    break;
  }
}
int EventReactorImpl::unregister_event(int fd, IReactor::Event e) {

}

} // namespace raft
