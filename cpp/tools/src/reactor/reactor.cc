#include "reactor.h"
#include "event2/listener.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <glog/logging.h>

namespace reactor {

int Reactor::runSync() {
}

int Reactor::runAsync() {
}
int Reactor::stop() {
}
int Reactor::register_event(int fd, const EventOptions *eo) {
  int ret;
  switch (eo->e_type) {
  case Event::LISTEN: {
    const auto *leo = static_cast<const ListenEventOptions *>(eo);
    ret = register_listen_event(fd, leo);
    break;
  }
  case Event::READ:
  case Event::WRITE: {
    break;
  }
  case Event::TIMEOUT: {
  }
  }
  return ret;
}
int Reactor::unregister_event(int fd, Event) {

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

EventReactorImpl::EventReactorImpl() : base_(event_base_new()) {}

EventReactorImpl::~EventReactorImpl() {
  if (base_)
    event_base_free(base_);
}

int EventReactorImpl::runSync() {

}
int EventReactorImpl::runAsync() {

}
int EventReactorImpl::stop() {

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

int EventReactorImpl::unregister_event(int fd, Event e) {

}

int EventReactorImpl::register_listen_event(int, const ListenEventOptions* leo) {
  //using lcb = void (*)(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
  struct ListenerCtx {
    CallBack cb;
    CallBack err_cb;
    evconnlistener* evl;
  };
  ListenerCtx *c = new ListenerCtx{};
  if (!c) return -1;
  c->cb = leo->cb;
  c->err_cb = leo->err_cb;
  auto* evl = evconnlistener_new_bind(base_, 0, c, leo->flags, leo->backlog, &leo->sa, leo->socklen);
  if (!evl) {
    delete c;
    LOG(ERROR) << "ev listener new failed: " << strerror(errno);
    return -1;
  }
  c->evl = evl;

  auto event_cb = [](struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void* ctx) {
    auto* c = (ListenerCtx*)ctx;
    c->cb();
  };
  evconnlistener_set_cb(evl, event_cb, c);

  auto err_cb = [](struct evconnlistener*, void* ctx) {
    auto *c = (ListenerCtx*)ctx;
    c->err_cb();
    evconnlistener_free(c->evl);
    delete c;
  };
  evconnlistener_set_error_cb(evl, err_cb);
  return 0;
}

void EventReactorImpl::new_listen_event_opt(ListenEventOptions &leo,
                                            CallBack cb, CallBack err_cb,
                                            int listen_flags, int backlog,
                                            const sockaddr *sa, int socklen) {
  leo.e_type = Event::LISTEN;
  leo.data = 0;
  leo.cb = cb;
  leo.err_cb = err_cb;
  leo.flags = listen_flags;
  leo.backlog = backlog;
  memcpy(&leo.sa, sa, socklen);
  leo.socklen = socklen;
}

} // namespace reactor
