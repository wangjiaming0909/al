#pragma once
#include <raft.grpc.pb.h>
#include <string>

namespace raft {
  class Instance
  {
    public:
      Instance(const std::string& uuid);
      virtual ~Instance();

    protected:
      std::string uuid_;
  };
} /* namespace raft */
