#include <raft_instance.h>

namespace raft {
  Instance::Instance(const std::string &uuid) : uuid_(uuid) {}

  Instance::~Instance() { }
}
