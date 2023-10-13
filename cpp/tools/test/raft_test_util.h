#pragma once
#include "raft/raft_instance.h"
#include "reactor/event_reactor_impl.h"

inline raft::Uuid get_uuid() {
  static unsigned long long i = 0;
  return std::to_string(i++) + "-instance";
}

inline std::shared_ptr<reactor::Reactor> create_reactor_and_run() {
  reactor::EventReactorImpl* impl = new reactor::EventReactorImpl{};
  auto *r = new reactor::Reactor{impl};
  r->runAsync();
  return std::shared_ptr<reactor::Reactor>{r};
}


