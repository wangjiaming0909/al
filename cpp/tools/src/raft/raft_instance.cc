#include "raft_instance.h"
#include "raft.grpc.pb.h"
#include "reactor/event_reactor_impl.h"
#include "reactor/timer.h"
#include <glog/logging.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>

namespace raft {
RaftInstance::~RaftInstance() {}

RaftInstance::RaftInstance(const Uuid &uuid, const std::string &listen_addr,
                           std::shared_ptr<reactor::Reactor> reactor,
                           const RaftOptions &ros)
    : uuid_(uuid), listen_addr_(listen_addr), peers_(), role_(Role::Candidate),
      term_id_(0), leader_uuid_(), leader_term_id_(0),
      rpc_server_(new RaftRpcServer(this, listen_addr_)), reactor_(reactor),
      failure_detection_timer_(nullptr), opts_(ros) {
  using namespace std::chrono_literals;
  reactor::Timer::Options opts{ros.failure_detection_interval};
  failure_detection_timer_ = reactor::Timer::create(opts, new reactor::EventTimerImpl{reactor.get()});
  metadata_.reset(new RaftMetadata(uuid));
}

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
  leader_uuid_ = request.candidate_id();
  leader_term_id_ = request.candidate_term_id();

  LOG(INFO) << uuid() << " granted " << leader_uuid_
            << " to be leader, master term id: " << leader_term_id_;

  // prepare the reply
  reply.set_responder_uuid(uuid_);
  reply.set_vote_granted(true);
  reply.set_responder_term_id(term_id_);
}

void RaftInstance::deny(const raft_pb::VoteRequest &request,
                        raft_pb::VoteReply &reply) {
  LOG(INFO) << uuid() << " denied " << request.candidate_id()
            << " to be leader my term id: " << term_id_
            << " candidate term id: " << request.candidate_term_id();
  // prepare the reply
  reply.set_responder_uuid(uuid_);
  reply.set_vote_granted(false);
  reply.set_responder_term_id(term_id_);
}

// TODO do we need to snooze the failure detection timer
grpc::Status RaftInstance::request_vote(const raft_pb::VoteRequest &request,
                                        raft_pb::VoteReply &reply) {
  assert(request.dest_id() == uuid_);
  auto candidate_term_id = request.candidate_term_id();
  // 1. if we have leader and uuid is not same as the candidate uuid, we deny
  // this vote
  if (!leader_uuid_.empty() && leader_uuid_ != request.candidate_id()) {
    deny(request, reply);
    return grpc::Status::OK;
  }
  // 2. if candidate is fall behind, we deny this vote
  if (candidate_term_id <= term_id_) {
    deny(request, reply);
    return grpc::Status::OK;
  }

  // 3. if we have no leader, but we already granted someone
  //   a. if same as candidate, we grant again
  //   b. if not, we deny it
  if (leader_uuid_.empty() && metadata_->has_voted_for()) {
    if (metadata_->get_voted_for() == request.candidate_id()) {
      grant(request, reply);
      return grpc::Status::OK;
    } else {
      deny(request, reply);
      return grpc::Status::OK;
    }
  }

  // 4. check last oper id of candidate

  grant(request, reply);
  return ::grpc::Status::OK;
}

int RaftInstance::start() {
  // start service
  auto ret = start_server();
  if (ret != 0) {
    return ret;
  }
  election_res_.reset(new LeaderElectionResult{peers_.size()});
  // start timer for failure detection
  using namespace std::placeholders;
  reactor::Timer::TimerCallBackT<typeof(*this)> cb = failure_detection_cb;
  failure_detection_timer_ctx_ =
      failure_detection_timer_->start(opts_.failure_detection_interval, shared_from_this(), cb);
  if (!failure_detection_timer_ctx_.lock()) {
    LOG(ERROR) << "raft instance start failure detection timer failed: "
               << strerror(errno);
    shutdown_server();
    return -1;
  }
  return 0;
}

void RaftInstance::wait() {
  wait_server();
}

void RaftInstance::shutdown() {
  shutdown_server();
}

grpc::Status Peer::request_vote(::grpc::ClientContext *ctx,
                                const raft_pb::VoteRequest &request,
                                raft_pb::VoteReply &reply) {
  return client_->request_vote(ctx, request, reply);
}

void Peer::request_vote_async(::grpc::ClientContext *ctx,
                              const raft_pb::VoteRequest &req,
                              raft_pb::VoteReply &resp,
                              std::function<void(::grpc::Status)> f) {
  return client_->request_vote_async(ctx, req, resp, f);
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

RaftRpcServer::~RaftRpcServer() {}

int RaftRpcServer::start() {
  if (server_) {
    return 0;
  }
  grpc::ServerBuilder builder;
  builder.RegisterService(service_.get_service());
  builder.AddListeningPort(listen_addr_, grpc::InsecureServerCredentials());
  server_ = std::move(builder.BuildAndStart());
  if (server_) return 0;
  LOG(ERROR) << "start rpc server failed: " << strerror(errno);
  return -1;
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
  request_vote(::grpc::ClientContext *ctx, const raft_pb::VoteRequest &request,
               raft_pb::VoteReply &reply) override {
    return stub_->request_vote(ctx, request, &reply);
  }
  inline void
  request_vote_async(::grpc::ClientContext *ctx,
                     const raft_pb::VoteRequest &req, raft_pb::VoteReply &resp,
                     std::function<void(::grpc::Status)> f) override {
    stub_->async()->request_vote(ctx, &req, &resp, f);
  }

  inline virtual ::grpc::Status
  update_consensus(::grpc::ClientContext *ctx,
                   const raft_pb::ConsensusRequest &req,
                   raft_pb::ConsensusResponse &resp) override {
    return stub_->update_consensus(ctx, req, &resp);
  }

  inline void
  update_consensus_async(::grpc::ClientContext *ctx,
                         const raft_pb::ConsensusRequest &req,
                         raft_pb::ConsensusResponse &resp,
                         std::function<void(::grpc::Status)> f) override {
    stub_->async()->update_consensus(ctx, &req, &resp, f);
  }
};

RaftRpcClient::RaftRpcClient(const std::string &addr)
    : stub_(new RaftStubImpl(addr)) {}

grpc::Status RaftRpcClient::request_vote(::grpc::ClientContext *ctx,
                                         const raft_pb::VoteRequest &request,
                                         raft_pb::VoteReply &reply) {
  return stub_->request_vote(ctx, request, reply);
}

void RaftRpcClient::request_vote_async(::grpc::ClientContext *ctx,
                                       const raft_pb::VoteRequest &req,
                                       raft_pb::VoteReply &resp,
                                       std::function<void(::grpc::Status)> f) {
  stub_->request_vote_async(ctx, req, resp, f);
}

::grpc::Status
RaftRpcClient::update_consensus(::grpc::ClientContext *ctx,
                                const raft_pb::ConsensusRequest &req,
                                raft_pb::ConsensusResponse &resp) {
  return stub_->update_consensus(ctx, req, resp);
}

void RaftRpcClient::update_consensus_async(
    ::grpc::ClientContext *ctx, const raft_pb::ConsensusRequest &req,
    raft_pb::ConsensusResponse &resp, std::function<void(::grpc::Status)> f) {
  stub_->update_consensus_async(ctx, req, resp, f);
}

RaftRpcClient::~RaftRpcClient() {}

/**********Raft Client*********/

/******************leader election****************/

LeaderElectionResult::LeaderElectionResult(size_t voter_num)
    : voter_num_(voter_num) {}

LeaderElection::LeaderElection(std::shared_ptr<RaftInstance> instance)
    : instance_(instance) {}

std::shared_ptr<LeaderElection>
LeaderElection::create_election(std::shared_ptr<RaftInstance> ins) {
  return std::shared_ptr<LeaderElection>(new LeaderElection{ins});
}

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
  // TODO need lock peer_map
  // TODO or copy out from ins, to avoid locking
  for (auto &peer : ins->peer_map()) {
    raft_pb::VoteRequest req;
    auto state_it = peer_states_.find(peer.second->uuid());
    if (state_it == peer_states_.end()) {
      state_it =
          peer_states_
              .emplace(peer.second->uuid(),
                       VoteState{std::make_unique<::raft_pb::VoteReply>(),
                                 std::make_unique<::grpc::ClientContext>()})
              .first;
    }
    req.set_candidate_id(ins->uuid());
    req.set_candidate_term_id(ins->term_id());
    req.set_dest_id(peer.first);
    peer.second->request_vote_async(
        state_it->second.cctx_.get(), req, *state_it->second.reply.get(),
        std::bind(&LeaderElection::request_vote_cb, this, std::placeholders::_1,
                  peer.second->uuid(), shared_from_this()));
  }
  return res;
}

void LeaderElection::request_vote_cb(::grpc::Status status, Uuid peer_id,
                                     std::shared_ptr<LeaderElection> self) {
  std::shared_ptr<RaftInstance> ins = instance_.lock();
  if (!ins) {
    LOG(WARNING) << "receive vote reply from: " << peer_id
                 << " but ins: has been released";
    return;
  }
  LOG(INFO) << ins->uuid() << " receive vote reply from: " << peer_id;
  auto state_it = peer_states_.find(peer_id);
  if (state_it == peer_states_.end()) {
    LOG(WARNING) << "instance: " << ins->uuid()
                 << " peer_state not found for: " << peer_id;
    return;
  }
  auto& res = *ins->get_election_res().get();
  handle_reply(status, *state_it->second.reply.get(),
               res);

  if (res.granted()) {
    LOG(INFO) << ins->uuid() << " leader election succeed, going to be leader";
  } else if (res.denied()) {
    LOG(INFO) << ins->uuid() << " leader election failed";
  } else {
  }
}

void LeaderElection::handle_reply(::grpc::Status rpc_status,
                                  const raft_pb::VoteReply &reply,
                                  LeaderElectionResult &res) {
  auto ins = instance_.lock();
  if (!ins) {
    LOG(ERROR) << "instance has been deleted...";
    return;
  }
  /*LOG(INFO) << ins->uuid() << " handle reply from: [" <<
     reply.responder_uuid()
            << "] rpc_status: [" << rpc_status.error_message() << "] reply: ["
            << reply.DebugString() << "]";*/
  if (rpc_status.ok()) {
    if (reply.vote_granted()) {
      res.grant(reply.responder_uuid());
      LOG(INFO) << ins->uuid() << " receive leader granted from: ["
                << reply.responder_uuid() << "]";
    } else {
      res.deny(reply.responder_uuid());
      LOG(INFO) << ins->uuid() << " receive leader denied from: ["
                << reply.responder_uuid() << "]";
    }
  } else {
    res.deny(reply.responder_uuid());
    LOG(INFO) << ins->uuid() << " receive leader denied from: ["
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

void failure_detection_cb(std::shared_ptr<RaftInstance> self) {
  LOG(INFO) << self->uuid()
            << " leader failure detected, cur leader: " << self->leader_uuid_;
  // start to do leader election
  LOG(INFO) << self->uuid_ << " start to leader election";
  if (!self->leader_election_) {
    self->leader_election_ = LeaderElection::create_election(self);
  }
  self->leader_election_->elect();
}

RaftMetadata::RaftMetadata(const Uuid &local_id)
    : leader_uuid_(), active_role_(Role::Candidate),
      meta_pb_(new ::raft_pb::RaftMetadata{}) {}

void RaftMetadata::set_commited_config(const raft_pb::RaftConfig& config) {

}

void RaftMetadata::set_current_term(int64_t term) {

}

void RaftMetadata::set_voted_for(Uuid id) {
  meta_pb_->set_voted_for(id);
}

bool RaftMetadata::has_voted_for() const {
  return meta_pb_->has_voted_for();
}

Uuid RaftMetadata::get_voted_for() const {
  return meta_pb_->voted_for();
}

} // namespace raft
