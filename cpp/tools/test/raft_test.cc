#include "raft.grpc.pb.h"
#include "raft_test_util.h"
#include <glog/logging.h>
#include <grpcpp/server_builder.h>
#include <gtest/gtest.h>
#include <raft_instance.h>
#include <thread_pool.h>

TEST(raft, example) {
  ASSERT_EQ(1, 1);
}

ThreadPool& get_thread_pool() {
  static ThreadPool pool{2};
  return pool;
}

template <typename Func, typename ...ARGS>
void run_in_pool(Func &&f, ARGS &&...args) {
  get_thread_pool().enqueue(std::move(f), std::move(args)...);
}

void wait_pool_finish() {
}

void start_server_in_pool(const raft::Uuid &uid, const std::string &addr) {
  std::string service_addr1{addr};
  raft::RaftRpcServer server1{uid, service_addr1};
  server1.start();
  run_in_pool([&]() { server1.wait(); });
}

raft::PeerInfo my_info{get_uuid(), "localhost:9000"};
raft::PeerInfo peer1{get_uuid(), "localhost:9001"};
raft::PeerInfo peer2{get_uuid(), "localhost:9002"};

void start_servers() {
  start_server_in_pool(my_info.id_, my_info.addr_);
  start_server_in_pool(peer1.id_, peer1.addr_);
  start_server_in_pool(peer2.id_, peer2.addr_);
}

TEST(raft, instance) {
  std::shared_ptr<raft::Instance> instance =
      std::make_shared<raft::Instance>(my_info.id_);
  instance->add_peer(peer1.id_, peer1.addr_);
  instance->add_peer(peer2.id_, peer2.addr_);

  const auto& peers = instance->peer_map();

  auto it = peers.find(peer1.id_);
  ASSERT_NE(it, peers.end());
  ASSERT_EQ(it->second->uuid(), peer1.id_);
  ASSERT_EQ(it->second->addr(), peer1.addr_);

  it = peers.find(peer2.id_);
  ASSERT_NE(it, peers.end());
  ASSERT_EQ(it->second->uuid(), peer2.id_);
  ASSERT_EQ(it->second->addr(), peer2.addr_);

  auto server1 = std::make_shared<raft::RaftRpcServer>(my_info.id_, my_info.addr_);
  server1->start();
  run_in_pool([&]() { server1->wait(); });
  auto server2 = std::make_shared<raft::RaftRpcServer>(peer1.id_, peer1.addr_);
  server2->start();
  run_in_pool([&]() { server2->wait(); });
  auto server3 = std::make_shared<raft::RaftRpcServer>(peer2.id_, peer2.addr_);
  server3->start();
  run_in_pool([&]() { server3->wait(); });

  raft::LeaderElection election{instance};

  auto result = election.elect();
  ASSERT_TRUE(result.granted());

  server1->shutdown();
  server2->shutdown();
  server3->shutdown();
}

