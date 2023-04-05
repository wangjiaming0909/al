#include "raft_instance.h"
#include "raft.grpc.pb.h"
#include <grpcpp/server_builder.h>
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

namespace raft {
using std::make_unique;

Instance::~Instance() {}
Instance::Instance(const Uuid &uuid)
    : uuid_(uuid), peers_(), role_(Role::Candidate), term_id_(0) {}

PeerInfo::PeerInfo(const Uuid &uuid, const std::string &addr)
    : addr_(addr), id_(uuid) {}

bool Instance::add_peer(PeerPtr peer) {
  auto it = peer_map().find(peer->uuid());
  if (it != peer_map().end()) return false;

  peers_[peer->uuid()] = std::move(peer);
  return true;
}

bool Instance::add_peer(const Uuid &uuid, const std::string &addr) {
  auto it = peer_map().find(uuid);
  if (it != peer_map().end()) return false;
  PeerInfo info{uuid, addr};
  PeerPtr peer_ptr = std::make_unique<Peer>(info);
  peers_[uuid] = std::move(peer_ptr);
  return true;
}

grpc::Status Peer::request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply) {
  return client_->request_vote(request, reply);
}

Peer::Peer(const PeerInfo &info)
    : info_(info), client_(new RaftRpcClient{info.addr_}) {}

Peer::~Peer() {}

/*********Raft Service***********/

class RaftServiceImpl : public RaftRpc::Service {
  virtual ::grpc::Status request_vote(::grpc::ServerContext *context,
                                      const ::raft::VoteRequest *request,
                                      ::raft::VoteReply *response) {
    LOG(INFO) << "vote request from: " << request->ShortDebugString();
    response->set_vote_granted(true);
    LOG(INFO) << server_->uuid() << " vote request granted...";
    return ::grpc::Status::OK;
  }

  RaftRpcServer *server_;
public:
  RaftServiceImpl(RaftRpcServer *server) : server_(server) {}
};

RaftService::RaftService(RaftRpcServer* server) : impl_(new RaftServiceImpl{server}) {}
RaftService::~RaftService() { delete impl_; }
/*********Raft Service***********/

/*********Raft Server***********/
RaftRpcServer::RaftRpcServer(const Uuid &uid, const std::string &listen_addr)
    : service_(this), server_{nullptr}, listen_addr_(listen_addr), uid_(uid) {}

void RaftRpcServer::start() {
  if (server_) {
    return;
  }
  grpc::ServerBuilder builder;
  builder.RegisterService(service_.get_service());
  builder.AddListeningPort(listen_addr_, grpc::InsecureServerCredentials());
  server_ = std::move(builder.BuildAndStart());
}

void RaftRpcServer::wait() {
  server_->Wait();
}

void RaftRpcServer::shutdown() {
  server_->Shutdown();
}
/*********Raft Server***********/

/**********Raft Client*********/
class RaftStubImpl : public IRaftProtocol {
  std::unique_ptr<RaftRpc::Stub> stub_;

public:
  RaftStubImpl(const std::string &addr)
      : stub_(RaftRpc::NewStub(::grpc::CreateChannel(
            addr, ::grpc::InsecureChannelCredentials()))) {}

  inline virtual ::grpc::Status request_vote(const raft::VoteRequest &request,
                                             raft::VoteReply &reply) {
    ::grpc::ClientContext context;
    return stub_->request_vote(&context, request, &reply);
  }
};

RaftRpcClient::RaftRpcClient(const std::string &addr) : stub_(new Stub(addr)) {}

::grpc::Status RaftRpcClient::Stub::request_vote(const raft::VoteRequest &request,
    raft::VoteReply &reply) {
  return stub_->request_vote(request, reply);
}

grpc::Status RaftRpcClient::request_vote(const raft::VoteRequest& request, VoteReply& reply) {
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

LeaderElection::LeaderElection(std::shared_ptr<Instance> instance)
    : instance_(instance) {}

LeaderElectionResult LeaderElection::elect() {
  auto ins = instance_.lock();
  if (!ins) {
    LOG(ERROR) << "when electing leader, instance has been deleted...";
    return LeaderElectionResult(0);
  }

  LeaderElectionResult res{ins->peer_map().size() + 1};

  // grant self
  res.grant(ins->uuid());
  if (res.decided()) return res;

  // start to request_vote sync
  for (auto& peer : ins->peer_map()) {
    VoteRequest req;
    VoteReply reply;
    req.set_candidate_id(ins->uuid());
    req.set_term_id(ins->term_id());
    ::grpc::Status status = peer.second->request_vote(req, reply);

    handle_reply(peer.second->uuid(), status, reply, res);
  }

  return res;
}

void LeaderElection::handle_reply(const Uuid &uuid, ::grpc::Status rpc_status,
                                  const VoteReply &reply,
                                  LeaderElectionResult &res) {
  LOG(INFO) << "handle reply from: [" << uuid << "] rpc_status: ["
            << rpc_status.error_message() << "] reply: [" << reply.DebugString()
            << "]";
  auto ins = instance_.lock();
  if (!ins) {
    LOG(ERROR) << "instance has been deleted...";
    return;
  }
  if (rpc_status.ok()) {
    if (reply.vote_granted()) {
      res.grant(uuid);
      LOG(INFO) << "leader granted for [" << ins->uuid() << "] from: [" << uuid
                << "]";
    } else {
      res.deny(uuid);
      LOG(INFO) << "leader denied for [" << ins->uuid() << "] from: [" << uuid
                << "]";
    }
  } else {
    res.deny(uuid);
    LOG(INFO) << "leader denied for [" << ins->uuid() << "] from: [" << uuid
              << "] err: [" << rpc_status.error_message() << "]";
  }
}

bool LeaderElectionResult::decided() const {
  return granted() || denied();
}
bool LeaderElectionResult::granted() const {
  return granted_peers_.size() >= majority_num();
}
bool LeaderElectionResult::denied() const {
  return (denied_peers_.size() + err_peers_.size()) >= majority_num();
}

} // namespace raft
