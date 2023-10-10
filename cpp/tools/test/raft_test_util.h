#pragma once
#include "raft/raft_instance.h"

raft::Uuid get_uuid() {
  static unsigned long long i = 0;
  return std::to_string(i++) + "-instance";
}


