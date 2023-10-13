#include "reactor_impl.h"

namespace reactor {

EventMap::~EventMap() {
}

int EventMap::add_event(std::shared_ptr<EventCtx> ctx) {
  s_.insert(ctx);
  return 0;
}

std::shared_ptr<EventCtx> EventMap::remove_event(std::shared_ptr<EventCtx> ctx) {
  auto it = s_.find(ctx);
  if (it == s_.end()) return nullptr;
  auto ret = *it;
  s_.erase(it);
  return ret;
}

}
