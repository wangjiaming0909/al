#pragma once
#include <chrono>
#include <functional>
#include <grpcpp/support/status.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace reactor {
class Reactor;
class Timer;
struct EventCtx;
}

namespace grpc {
class ChannelInterface;
class Server;
} // namespace grpc
namespace raft_pb {
class VoteRequest;
class VoteReply;
class RaftMetadata;
class RaftRpc;
class RaftPeerConfig;
class RaftConfig;
class ConsensusRequest;
class ConsensusResponse;
} // namespace raft_pb
namespace google {
namespace protobuf {
class Message;
}
} // namespace google

namespace raft {

enum class Role { Candidate, Master, Follower, Learner, Oberserver };
using Uuid = std::string;

struct IRaftProtocol {
  virtual grpc::Status request_vote(const raft_pb::VoteRequest &request,
                                    raft_pb::VoteReply &reply) = 0;
  virtual void request_vote_async(const raft_pb::VoteRequest &req,
                                  raft_pb::VoteReply &resp,
                                  std::function<void(::grpc::Status)> f) = 0;

  virtual ::grpc::Status update_consensus(const raft_pb::ConsensusRequest &req,
                                          raft_pb::ConsensusResponse &resp) = 0;
  virtual void
  update_consensus_async(const raft_pb::ConsensusRequest &req,
                         raft_pb::ConsensusResponse &resp,
                         std::function<void(::grpc::Status)> f) = 0;
};

class RaftServiceImpl;
class RaftRpcServer;
class LeaderElection;

class RaftService {
  RaftServiceImpl *impl_;

public:
  RaftService(RaftRpcServer *server);
  ~RaftService();
  RaftServiceImpl *get_service() { return impl_; }
};

class RaftInstance;
class RaftRpcServer {
  RaftInstance *raft_instance_;
  RaftService service_;
  std::unique_ptr<::grpc::Server> server_;
  std::string listen_addr_;
  Uuid uid_;

public:
  RaftRpcServer(RaftInstance *instance, const std::string &listen_addr);
  ~RaftRpcServer();
  inline const Uuid &uuid() const { return uid_; }
  inline RaftInstance *raft_instance() { return raft_instance_; }
  int start();
  void wait();
  void shutdown();
};

class RaftStubImpl;
class RaftRpcClient : public IRaftProtocol {
  std::unique_ptr<IRaftProtocol> stub_;

public:
  RaftRpcClient(const std::string &addr);
  ~RaftRpcClient();

  virtual grpc::Status request_vote(const raft_pb::VoteRequest &request,
                                    raft_pb::VoteReply &reply) override;
  virtual void
  request_vote_async(const raft_pb::VoteRequest &req, raft_pb::VoteReply &resp,
                     std::function<void(::grpc::Status)> f) override;

  virtual ::grpc::Status
  update_consensus(const raft_pb::ConsensusRequest &req,
                   raft_pb::ConsensusResponse &resp) override;
  virtual void
  update_consensus_async(const raft_pb::ConsensusRequest &req,
                         raft_pb::ConsensusResponse &resp,
                         std::function<void(::grpc::Status)> f) override;
};

struct PeerInfo {
  PeerInfo(const Uuid &uuid, const std::string &addr);
  ~PeerInfo() = default;
  Uuid id_;
  std::string addr_;
};

class Peer {
  PeerInfo info_;
  std::unique_ptr<RaftRpcClient> client_;
  std::unique_ptr<raft_pb::RaftPeerConfig> local_peer_conf_;

public:
  Peer(const PeerInfo &info);
  ~Peer();
  grpc::Status request_vote(const raft_pb::VoteRequest &request,
                                    raft_pb::VoteReply &reply);
  const Uuid &uuid() const { return info_.id_; }
  const std::string &addr() const { return info_.addr_; }

  grpc::Status signal_request();
};

using PeerPtr = std::unique_ptr<Peer>;
using PeerMap = std::unordered_map<Uuid, PeerPtr>;

class PeerManager {
  Uuid local_uuid_;
  PeerMap peers_;

public:
  PeerManager(const Uuid& local_uuid);
  void signal_request();
  /// @brief update peers, create new peers, add into peers_
  void update_peers(const PeerMap &peers);
};

grpc::Status write_pb_to_file(const std::string &path,
                              const google::protobuf::Message &msg);

grpc::Status read_pb_from_file(const std::string &path,
                               google::protobuf::Message *msg);

class RaftMetadata {
  Uuid leader_uuid_;
  Role active_role_;
  std::unique_ptr<raft_pb::RaftMetadata> meta_pb_;

public:
  RaftMetadata(const Uuid &local_uuid);
  static grpc::Status create(const Uuid &local_uuid,
                             const raft_pb::RaftConfig &config,
                             int64_t current_term,
                             std::unique_ptr<RaftMetadata> meta_out);
  static grpc::Status load(const Uuid &local_uuid,
                           std::unique_ptr<RaftMetadata> meta_out);

  void set_commited_config(const raft_pb::RaftConfig& config);
  void set_current_term(int64_t term);
  void flush();
};

struct RaftOptions {
  std::chrono::milliseconds failure_detection_interval;
};

/// @brief callback func when failure detection timer triggered
/// @note that this function will be invoked in timer thread
void failure_detection_cb(std::shared_ptr<RaftInstance> self);

class RaftInstance : public std::enable_shared_from_this<RaftInstance> {
  Uuid uuid_;
  std::string listen_addr_;
  PeerMap peers_;
  Role role_;
  uint64_t term_id_;
  Uuid master_uuid_;
  uint64_t master_term_id_;
  std::unique_ptr<RaftRpcServer> rpc_server_;
  std::unique_ptr<RaftMetadata> metadata_;
  std::unique_ptr<raft_pb::RaftPeerConfig> local_peer_pb_;
  std::weak_ptr<reactor::Reactor> reactor_;
  std::unique_ptr<reactor::Timer> timer_;
  std::weak_ptr<reactor::EventCtx> timer_ctx_;
  RaftOptions opts_;

  // TODO use shared_ptr, election callback will ref this object
  std::unique_ptr<LeaderElection> leader_election_;

  void grant(const raft_pb::VoteRequest &request, raft_pb::VoteReply &reply);
  void deny(const raft_pb::VoteRequest &request, raft_pb::VoteReply &reply);

public:
  friend void failure_detection_cb(std::shared_ptr<RaftInstance> self);
  RaftInstance(const Uuid &uuid, const std::string &listen_addr,
               std::shared_ptr<reactor::Reactor> reactor,
               const RaftOptions &ros);
  // @brief add one peer into the raft instance
  // @param peer the peer to add
  // @ret false if already existed, true otherwise
  bool add_peer(PeerPtr peer);
  // @brief add one peer into the raft instance
  // @param uuid the uid of this peer
  // @addr the service addr of this peer
  // @ret false if already existed, true otherwise
  bool add_peer(const Uuid &uuid, const std::string &addr);
  virtual ~RaftInstance();
  inline const PeerMap &peer_map() const { return peers_; }
  inline PeerMap &peer_map() { return peers_; }
  inline const Uuid &uuid() const;
  inline uint64_t term_id() const { return term_id_; }
  void inc_term_id() { term_id_++; }

  grpc::Status request_vote(const raft_pb::VoteRequest &request,
                            raft_pb::VoteReply &reply);

  int start();
  void wait();
  void shutdown();
private:
  int start_server() { return rpc_server_->start(); }
  void wait_server() { return rpc_server_->wait(); }
  void shutdown_server() { return rpc_server_->shutdown(); }
};

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
  /// @brief generate leader result accoring to the rpc status and reply
  /// @param rpc_status the rpc call status of request vote
  /// @param reply the reply of requestvote from remote server
  /// @param res leader election res
  void handle_reply(::grpc::Status rpc_status, const raft_pb::VoteReply &reply,
                    LeaderElectionResult &res);

public:
  LeaderElection(std::shared_ptr<RaftInstance> instance);

  LeaderElectionResult elect();

private:
  std::weak_ptr<RaftInstance> instance_;
};

} /* namespace raft */
