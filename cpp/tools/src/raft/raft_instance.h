#pragma once
#include "raft.pb.h"
#include <memory>
#include <raft.grpc.pb.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace raft {

enum class Role { Candidate, Master, Follower, Learner, Oberserver};
using Uuid = std::string;

struct PeerInfo {

};

struct IRaftProtocol {
  virtual void request_vote(const raft::VoteRequest& request, raft::VoteReply& reply) = 0;
};

class Peer : public IRaftProtocol{
  PeerInfo info_;
  public:
    Peer(const PeerInfo &info);
    virtual void request_vote(const raft::VoteRequest &request,
                              raft::VoteReply &reply);
};

using PeerPtr = std::unique_ptr<Peer>;

class Instance {
  public:
    Instance(const Uuid &uuid, std::unordered_map<Uuid, PeerPtr> &peers);
    virtual ~Instance();

  protected:
    Uuid uuid_;
    std::unordered_map<Uuid, PeerPtr> peers_;

    Role role_;
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
