#include "raft_instance.h"
#include "raft.grpc.pb.h"
#include <grpcpp/server_builder.h>>

namespace raft {
using std::make_unique;

Instance::~Instance() {}

PeerInfo::PeerInfo(const std::string &addr) : channel_(nullptr) {}
PeerInfo::~PeerInfo() {}

/*********Raft Service***********/
class RaftServiceImpl : public RaftRpc::Service {
  virtual ::grpc::Status request_vote(::grpc::ServerContext *context,
                                      const ::raft::VoteRequest *request,
                                      ::raft::VoteReply *response) {}
};

RaftService::RaftService() : impl_(new RaftServiceImpl{}) {}
/*********Raft Service***********/

/*********Raft Server***********/
RaftServer::RaftServer(const std::string &listen_addr)
    : service_(), server_{nullptr}, listen_addr_(listen_addr) {}

RaftServer::~RaftServer() {}
void RaftServer::start() {
  if (server_) {
    return;
  }
  grpc::ServerBuilder builder;
  builder.RegisterService(service_.get_service());
  builder.AddListeningPort(listen_addr_, grpc::InsecureServerCredentials());
  server_ = builder.BuildAndStart();
}
void RaftServer::wait() {
  if (server_) server_->Wait();
}
/*********Raft Server***********/
} // namespace raft
