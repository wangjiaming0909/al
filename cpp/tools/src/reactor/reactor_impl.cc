#include "reactor_impl.h"

namespace reactor {

EventMap::~EventMap() {
  for (auto &p : s_) {
    delete p;
  }
}

int EventMap::add_event(EventCtx* ctx) {
  s_.insert(ctx);
  return 0;
}

EventCtx* EventMap::remove_event(EventCtx* ctx) {
  auto it = s_.find(ctx);
  if (it == s_.end()) return nullptr;
  auto ret = *it;
  s_.erase(it);
  return ret;
}

}
