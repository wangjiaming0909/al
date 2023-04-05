#include <grpcpp/server_builder.h>
#include <raft_instance.h>
#include <gtest/gtest.h>
#include <thread_pool.h>
#include "raft.grpc.pb.h"

TEST(raft, example) {
  ASSERT_EQ(1, 1);
}

ThreadPool& get_thread_pool() {
  static ThreadPool pool{16};
  return pool;
}

template <typename Func, typename ...ARGS>
void run_in_pool(Func &&f, ARGS &&...args) {
  get_thread_pool().enqueue(std::move(f), std::move(args)...);
}

TEST(raft, leader_election) {
  std::string service_addr{"127.0.0.1:9000"};
  raft::RaftServer server{service_addr};
  server.start();
  run_in_pool([&]() { server.wait(); });
}

