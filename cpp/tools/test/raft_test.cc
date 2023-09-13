#include "raft.grpc.pb.h"
#include "raft_test_util.h"
#include <glog/logging.h>
#include <grpcpp/server_builder.h>
#include <gtest/gtest.h>
#include <memory>
#include <raft_instance.h>
#include <thread_pool.h>

TEST(raft, example) {
  ASSERT_EQ(1, 1);
}

ThreadPool& get_thread_pool() {
  static ThreadPool pool{3};
  return pool;
}

template <typename Func, typename ...ARGS>
void run_in_pool(Func &&f, ARGS &&...args) {
  get_thread_pool().enqueue(std::move(f), std::move(args)...);
}

void wait_pool_finish() {
}

raft::PeerInfo peer1{get_uuid(), "localhost:9000"};
raft::PeerInfo peer2{get_uuid(), "localhost:9001"};
raft::PeerInfo peer3{get_uuid(), "localhost:9002"};

TEST(raft, config) {
  raft_pb::RaftConfig config;
  auto* peer = config.add_peers();
  peer->set_member_type(raft_pb::PeerMemberTypeConfig::VOTER);
  peer->set_permanent_uuid(peer1.id_);
  auto *host_port = peer->mutable_last_known_private_addr()->Add();
}

TEST(raft, instance) {
  std::shared_ptr<raft::RaftInstance> instance1 =
      std::make_shared<raft::RaftInstance>(peer1.id_, peer1.addr_);
  instance1->add_peer(peer2.id_, peer2.addr_);
  instance1->add_peer(peer3.id_, peer3.addr_);

  auto instance2 = std::make_shared<raft::RaftInstance>(peer2.id_, peer2.addr_);
  instance2->add_peer(peer1.id_, peer1.addr_);
  instance2->add_peer(peer3.id_, peer3.addr_);
  auto instance3 = std::make_shared<raft::RaftInstance>(peer3.id_, peer3.addr_);
  instance3->add_peer(peer1.id_, peer1.addr_);
  instance3->add_peer(peer2.id_, peer2.addr_);

  const auto& peers = instance1->peer_map();

  auto it = peers.find(peer2.id_);
  ASSERT_NE(it, peers.end());
  ASSERT_EQ(it->second->uuid(), peer2.id_);
  ASSERT_EQ(it->second->addr(), peer2.addr_);

  it = peers.find(peer3.id_);
  ASSERT_NE(it, peers.end());
  ASSERT_EQ(it->second->uuid(), peer3.id_);
  ASSERT_EQ(it->second->addr(), peer3.addr_);

  instance1->start_server();
  run_in_pool([&]() { instance1->wait_server(); });
  instance2->start_server();
  run_in_pool([&]() { instance2->wait_server(); });
  instance3->start_server();
  run_in_pool([&]() { instance3->wait_server(); });

  raft::LeaderElection election{instance1};

  auto result = election.elect();
  ASSERT_TRUE(result.granted());

  instance1->shtudown_server();
  instance2->shtudown_server();
  instance3->shtudown_server();
}

