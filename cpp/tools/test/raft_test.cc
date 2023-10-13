#include "raft.grpc.pb.h"
#include "raft/raft_instance.h"
#include "raft_test_util.h"
#include <glog/logging.h>
#include <grpcpp/server_builder.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread_pool.h>

using namespace std::chrono_literals;

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
  auto reactor = create_reactor_and_run();
  raft::RaftOptions opts;
  opts.failure_detection_interval = 1s;
  std::shared_ptr<raft::RaftInstance> instance1 =
      std::make_shared<raft::RaftInstance>(peer1.id_, peer1.addr_, reactor,
                                           opts);
  instance1->add_peer(peer2.id_, peer2.addr_);
  instance1->add_peer(peer3.id_, peer3.addr_);

  opts.failure_detection_interval = 2s;
  auto instance2 = std::make_shared<raft::RaftInstance>(peer2.id_, peer2.addr_,
                                                        reactor, opts);
  instance2->add_peer(peer1.id_, peer1.addr_);
  instance2->add_peer(peer3.id_, peer3.addr_);

  opts.failure_detection_interval = 3s;
  auto instance3 = std::make_shared<raft::RaftInstance>(peer3.id_, peer3.addr_,
                                                        reactor, opts);
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

  instance1->start();
  run_in_pool([&]() { instance1->wait(); });
  instance2->start();
  run_in_pool([&]() { instance2->wait(); });
  instance3->start();
  run_in_pool([&]() { instance3->wait(); });

  std::this_thread::sleep_for(2s);

  LOG(INFO) << "start to stop raft test";
  instance1->shutdown();
  instance2->shutdown();
  instance3->shutdown();
  reactor->stop();
}
