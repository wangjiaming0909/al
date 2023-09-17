#include "reactor.h"
#include "event2/event_compat.h"
#include "event2/listener.h"
#include "event2/thread.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <glog/logging.h>
#include <unordered_map>

namespace reactor {

class EventMap {
public:
  EventMap() = default;
  ~EventMap() = default;

  int add_event(int fd, Event e, EventCtx* data) {
    auto ret = m_.emplace(fd, std::unordered_map<Event, EventCtx*>{});
    if (ret.first->second.emplace(e, data).second) {
      return 0;
    } else {
      return -1;
    }
  }
  EventCtx* remove_event(int fd, Event e) {
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

  EventCtx* get_ctx(int fd, Event e) {
    auto it = m_.find(fd);
    if (it != m_.end()) {
      auto it2 = it->second.find(e);
      if (it2 != it->second.end()) {
        return it2->second;
      }
    }
    return nullptr;
  }

private:
  auto find_ctx(int fd, Event e) {
    auto it = m_.find(fd);
    if (it != m_.end()) {
      auto it2 = it->second.find(e);
      if (it2 != it->second.end()) {
        return std::make_pair(true, it2);
      }
    }
    return std::make_pair(false, decltype(it->second)::iterator{});
  }

private:
  std::unordered_map<int, std::unordered_map<Event, EventCtx *>> m_;
  ;
};

struct EventCtx {
  EventCtx() : eos(nullptr) {}
  ~EventCtx() { delete eos; }
  EventOptions* eos;
  int fd;
  void* ec;
};

struct ListenEventCtx : public EventCtx{
  ListenEventCtx() : EventCtx() {
    eos = new ListenEventOptions();
  }
  ~ListenEventCtx() = default;
};

struct ConnectEventCtx : public EventCtx {
  ConnectEventCtx() : EventCtx() {
    eos = new ConnectEventOptions();
  }
};

Reactor::Reactor(ReactorImpl* impl) :impl_(impl), em_(new EventMap{}) { }

int Reactor::runSync() {
  return impl_->runSync();
}

int Reactor::runAsync() {
}
int Reactor::stop() {
  return impl_->stop();
}
int Reactor::register_event(int fd, const EventOptions *eos) {
  EventCtx* ret = nullptr;
  switch (eos->e_type) {
  case Event::LISTEN: {
    const auto *leos = static_cast<const ListenEventOptions *>(eos);

    ret = impl_->register_listen_event(fd, leos);
    break;
  }
  case Event::CONNECT: {
    const auto *ceos = static_cast<const ConnectEventOptions *>(eos);
    ret = impl_->register_connect_event(fd, ceos);
    break;
  }
  case Event::READ:
  case Event::WRITE: {
    break;
  }
  case Event::TIMEOUT: {
  }
  }
  if (ret) {
    fd = ret->fd;
    em_->add_event(fd, eos->e_type, ret);
    return 0;
  }
  return -1;
}

int Reactor::unregister_event(int fd, Event e) {
  int ret;
  EventCtx* ctx = em_->get_ctx(fd, e);
  if (!ctx) return -1;
  switch (e) {
    case Event::LISTEN:
      ret = impl_->unregister_listen_event(fd, (ListenEventCtx*)ctx);
      break;
    default:
      break;
  }
  return ret;
}

EventTimerImpl::EventTimerImpl(Reactor *reactor) : TimerImpl(reactor) {}

int EventTimerImpl::start(Period period, TimerCallBack cb) {
  /*
  if (!base_)
    return -1;
  this->cb_ = cb;

  struct TimerCbCtx {
    std::weak_ptr<EventTimerImpl> impl;
  };

  auto *ctx = new TimerCbCtx{};
  if (!ctx)
    return -1;
  ctx->impl = shared_from_this();

  auto te_cb = [](int, short int, void *ctx) {
    TimerCbCtx *c = (TimerCbCtx *)ctx;
    auto timer = c->impl;
    auto impl = timer.lock();
    if (impl)
      impl->cb_();
    delete c;
  };

  e_ = evtimer_new(base_, te_cb, ctx);
  if (!e_)
    return -1;

  struct timeval t;
  t.tv_usec = period.count() * 1000;
  evtimer_add(e_, &t);
  */

  return 0;
}

int EventTimerImpl::snooze(Period period) {

}

int EventTimerImpl::stop() {

}

EventReactorImpl::EventReactorImpl() : base_(nullptr) {
  if (-1 == evthread_use_pthreads()) {
    LOG(ERROR) << "set libevent with pthreads failed";
  }
  base_ = event_base_new();
}

EventReactorImpl::~EventReactorImpl() {
  if (base_)
    event_base_free(base_);
}

int EventReactorImpl::runSync() {
  return event_base_dispatch(base_);
}
int EventReactorImpl::runAsync() {

}
int EventReactorImpl::stop() {
  return event_base_loopexit(base_, 0);
}

/*
int EventReactorImpl::register_event(int fd, const EventOptions* eo, CallBack cb) {
  switch (eo->e_type) {
  case Event::LISTEN: {
    break;
  }
  case Event::READ:
  case Event::WRITE: {
    break;
  }
  case Event::TIMEOUT: {
    struct Ctx {
      CallBack cb;
    };
    Ctx* c = new Ctx();
    c->cb = cb;
    auto te_cb = [](int, short int, void *ctx) {
      ((Ctx *)ctx)->cb();
      delete (Ctx*)ctx;
    };
    auto *e = evtimer_new(base_, te_cb, c);
    break;
  }
  default:
    break;
  }
}*/

ListenEventCtx* EventReactorImpl::register_listen_event(int, const ListenEventOptions* leos) {
  //using lcb = void (*)(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
  auto *c = new ListenEventCtx();
  if (!c) return nullptr;
  *c->eos = *leos;
  auto* ecl = evconnlistener_new_bind(base_, 0, c, leos->flags, leos->backlog, &leos->sa, leos->socklen);
  if (!ecl) {
    delete c;
    LOG(ERROR) << "ev listener new failed: " << strerror(errno);
    return nullptr;
  }
  c->ec = ecl;
  c->fd = evconnlistener_get_fd(ecl);

  auto event_cb = [](struct evconnlistener*, evutil_socket_t fd, struct sockaddr*, int socklen, void* ctx) {
    auto* c = (ListenEventCtx*)ctx;
    ((ListenEventOptions*)c->eos)->read_cb(fd, c->eos->user_data);
  };
  evconnlistener_set_cb(ecl, event_cb, c);

  auto err_cb = [](struct evconnlistener*, void* ctx) {
    auto *c = (ListenEventCtx*)ctx;
    ((ListenEventOptions*)c->eos)->event_cb(c->eos->user_data);
  };
  evconnlistener_set_error_cb(ecl, err_cb);
  return c;
}

int EventReactorImpl::unregister_listen_event(int fd, ListenEventCtx* ctx) {
  auto *ecl = (evconnlistener*)ctx->ec;
  evconnlistener_disable(ecl);
  evconnlistener_free(ecl);
  delete ctx;
  return 0;
}

ConnectEventCtx *
EventReactorImpl::register_connect_event(int, const ConnectEventOptions *ceos) {
  auto *c = new ConnectEventCtx();
  if (!c) return nullptr;

  *c->eos = *ceos;
  bufferevent* be = bufferevent_socket_new(base_, 0, ceos->flags);
  if (!be) {
    delete c;
    return nullptr;
  }
  c->fd = bufferevent_getfd(be);
  c->ec = be;
  auto read_cb = [](struct bufferevent* bev, void* ctx) {
    ConnectEventCtx* c = (ConnectEventCtx*)ctx;
    //((ConnectEventOptions*)c->eos)->read_cb();
  };
  auto write_cb = [](struct bufferevent* bev, void* ctx) {
    ConnectEventCtx* c = (ConnectEventCtx*)ctx;
    //c->eos->write_cb();
  };
  auto e_cb = [](struct bufferevent* bev, short what, void* ctx) {
    ConnectEventCtx* c = (ConnectEventCtx*)ctx;
    ((ConnectEventOptions*)c->eos)->event_cb(what, c->eos->user_data);
  };
  bufferevent_setcb(be, read_cb, write_cb, e_cb, c);
  bufferevent_enable(be, EV_READ);
  if (0 != bufferevent_socket_connect(be, &ceos->sa, ceos->socklen)) {
    delete c;
    bufferevent_free(be);
    return nullptr;
  }
  return c;
}

int EventReactorImpl::unregister_connect_event(int, ConnectEventCtx *ctx) {
  bufferevent_disable((bufferevent*)ctx->ec, EV_READ);
  bufferevent_free((bufferevent*)ctx->ec);
  delete ctx;
  return 0;
}

void EventReactorImpl::new_listen_event_opt(ListenEventOptions &leos,
                                            AcceptCb cb, AcceptErrCb err_cb,
                                            int listen_flags, int backlog,
                                            const sockaddr *sa, int socklen) {
  leos.e_type = Event::LISTEN;
  leos.user_data = 0;
  leos.read_cb = cb;
  leos.event_cb = err_cb;
  leos.flags = listen_flags;
  leos.backlog = backlog;
  memcpy(&leos.sa, sa, socklen);
  leos.socklen = socklen;
}

void EventReactorImpl::new_connect_event_opt(ConnectEventOptions &ceos,
                                    const sockaddr *sa, int socklen, int flags,
                                    CallBack rd_cb, CallBack wr_cb,
                                    CallBack e_cb) {

}

} // namespace reactor
