#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace grpc {
class ChannelInterface;
class Server;
}
namespace raft {

enum class Role { Candidate, Master, Follower, Learner, Oberserver };
using Uuid = std::string;

class VoteRequest;
class VoteReply;

class RaftRpc;

class PeerInfo {
public:
  PeerInfo(const std::string &addr);
  ~PeerInfo();
private:
  std::unique_ptr<::grpc::ChannelInterface> channel_;
};

struct IRaftProtocol {
  virtual void request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply) = 0;
};

class RaftServiceImpl;
class RaftService : IRaftProtocol {
  RaftServiceImpl *impl_;

public:
  RaftService();
  virtual void request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply);
  RaftServiceImpl *get_service() { return impl_; }
};

class RaftServer {
  RaftService service_;
  std::unique_ptr<::grpc::Server> server_;
  std::string listen_addr_;
public:
  RaftServer(const std::string &listen_addr);
  ~RaftServer();
  void start();
  void wait();
};

class RaftStubImpl;
class RaftClient {
  class Stub : public IRaftProtocol {
    std::unique_ptr<RaftStubImpl> stub_;

  public:
    virtual void request_vote(const raft::VoteRequest &request,
                              raft::VoteReply &reply);
  };
public:
  RaftClient(const std::string& addr);
  ~RaftClient() = default;
};

class Peer : public IRaftProtocol {
  PeerInfo info_;
  std::unique_ptr<RaftClient> client_;

public:
  Peer(const PeerInfo &info);
  virtual void request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply);
};
using PeerPtr = std::unique_ptr<Peer>;
using PeerMap = std::unordered_map<Uuid, PeerPtr>;

class Instance {
  Uuid uuid_;
  PeerMap peers_;
  Role role_;

public:
  Instance(const Uuid &uuid, PeerMap &peers);
  virtual ~Instance();
  const PeerMap &peer_map() const { return peers_; }
};

class LeaderElection;

class LeaderElectionResult {
  std::unordered_set<Uuid> granted_peers_;
  std::unordered_set<Uuid> denied_peers_;
  std::unordered_set<Uuid> err_peers_;

  void grant(const Uuid &uuid);
  void deny(const Uuid &uuid);
  void err(const Uuid &uuid);

public:
  friend class LeaderElection;
  bool decided() const;
  bool granted() const;
  bool denied() const;
};

class LeaderElection {
public:
  LeaderElection(std::shared_ptr<Instance> instance);

  LeaderElectionResult elect();

private:
  std::weak_ptr<Instance> instance_;
};

} /* namespace raft */
