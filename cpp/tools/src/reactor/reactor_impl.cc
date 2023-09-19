#include "reactor_impl.h"

namespace reactor {

EventMap::~EventMap() {
  for (auto &p : this->m_) {
    for (auto &p2 : p.second) {
      delete p2.second;
    }
  }
}

int EventMap::add_event(int fd, Event e, EventCtx* data) {
  auto ret = m_.emplace(fd, std::unordered_map<Event, EventCtx*>{});
  if (ret.first->second.emplace(e, data).second) {
    return 0;
  } else {
    return -1;
  }
}
EventCtx* EventMap::remove_event(int fd, Event e) {
  EventCtx *ret = nullptr;
  auto it = m_.find(fd);
  if (it != m_.end()) {
    auto it2 = it->second.find(e);
    if (it2 != it->second.end()) {
      ret = it2->second;
      it->second.erase(it2);
    }
  }
  return ret;
}

EventCtx* EventMap::get_ctx(int fd, Event e) {
  auto it = m_.find(fd);
  if (it != m_.end()) {
    auto it2 = it->second.find(e);
    if (it2 != it->second.end()) {
      return it2->second;
    }
  }
  return nullptr;
}
auto EventMap::find_ctx(int fd, Event e) {
  auto it = m_.find(fd);
  if (it != m_.end()) {
    auto it2 = it->second.find(e);
    if (it2 != it->second.end()) {
      return std::make_pair(true, it2);
    }
  }
  return std::make_pair(false, decltype(it->second)::iterator{});
}
}
