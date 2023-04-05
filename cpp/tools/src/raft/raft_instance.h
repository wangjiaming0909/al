#pragma once
#include <grpcpp/support/status.h>
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

struct IRaftProtocol {
  virtual grpc::Status request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply) = 0;
};

class RaftServiceImpl;
class RaftRpcServer;
class RaftService {
  RaftServiceImpl *impl_;

public:
  RaftService(RaftRpcServer *server);
  ~RaftService();
  RaftServiceImpl *get_service() { return impl_; }
};

class RaftRpcServer {
  RaftService service_;
  std::unique_ptr<::grpc::Server> server_;
  std::string listen_addr_;
  Uuid uid_;
public:
  RaftRpcServer(const Uuid& uuid, const std::string &listen_addr);
  ~RaftRpcServer() {}
  inline const Uuid &uuid() const { return uid_; }
  void start();
  void wait();
  void shutdown();
};

class RaftStubImpl;
class RaftRpcClient : public IRaftProtocol{
  class Stub : public IRaftProtocol {
    std::unique_ptr<RaftStubImpl> stub_;

  public:
    Stub(const std::string& addr);
    ~Stub();
    virtual grpc::Status request_vote(const raft::VoteRequest &request,
                                      raft::VoteReply &reply);
  };
  std::unique_ptr<Stub> stub_;

public:
  RaftRpcClient(const std::string& addr);
  ~RaftRpcClient();

  virtual grpc::Status request_vote(const raft::VoteRequest& request, VoteReply& reply);
};

struct PeerInfo {
  PeerInfo(const Uuid& uuid, const std::string &addr);
  ~PeerInfo() = default;
  Uuid id_;
  std::string addr_;
};

class Peer : public IRaftProtocol {
  PeerInfo info_;
  std::unique_ptr<RaftRpcClient> client_;

public:
  Peer(const PeerInfo &info);
  ~Peer();
  virtual grpc::Status request_vote(const raft::VoteRequest &request,
                            raft::VoteReply &reply);
  const Uuid &uuid() const { return info_.id_; }
  const std::string &addr() const { return info_.addr_; }
};
using PeerPtr = std::unique_ptr<Peer>;
using PeerMap = std::unordered_map<Uuid, PeerPtr>;

class Instance {
  Uuid uuid_;
  PeerMap peers_;
  Role role_;
  uint64_t term_id_;

public:
  Instance(const Uuid &uuid);
  // @brief add one peer into the raft instance
  // @param peer the peer to add
  // @ret false if already existed, true otherwise
  bool add_peer(PeerPtr peer);
  // @brief add one peer into the raft instance
  // @param uuid the uid of this peer
  // @addr the service addr of this peer
  // @ret false if already existed, true otherwise
  bool add_peer(const Uuid &uuid, const std::string &addr);
  virtual ~Instance();
  inline const PeerMap &peer_map() const { return peers_; }
  inline PeerMap &peer_map() { return peers_; }
  inline const Uuid &uuid() const { return uuid_; }
  inline uint64_t term_id() const { return term_id_; }
};

class LeaderElection;

class LeaderElectionResult {
  size_t voter_num_;
  std::unordered_set<Uuid> granted_peers_;
  std::unordered_set<Uuid> denied_peers_;
  std::unordered_set<Uuid> err_peers_;

  inline size_t majority_num() const { return (voter_num_ / 2) + 1; }
  inline void grant(const Uuid &uuid) { granted_peers_.insert(uuid); }
  inline void deny(const Uuid &uuid) { denied_peers_.insert(uuid); }
  inline void err(const Uuid &uuid) { err_peers_.insert(uuid); }

public:
  LeaderElectionResult(size_t voter_num);
  friend class LeaderElection;
  bool decided() const;
  bool granted() const;
  bool denied() const;
};

class LeaderElection {
  /// @brief parse the vote reply, add results into the result
  void handle_reply(const Uuid &uuid, ::grpc::Status rpc_status,
                    const VoteReply &reply, LeaderElectionResult &res);

public:
  LeaderElection(std::shared_ptr<Instance> instance);

  LeaderElectionResult elect();

private:
  std::weak_ptr<Instance> instance_;
};

} /* namespace raft */
