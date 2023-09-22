#include "event_reactor_impl.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include "event2/thread.h"
#include "reactor_impl.h"
#include <glog/logging.h>

namespace reactor {

struct EventListenEventCtx : public ListenEventCtx {
  ~EventListenEventCtx() {
    if (ec) {
      evconnlistener_disable((evconnlistener *)ec);
      evconnlistener_free((evconnlistener *)ec);
      ec = nullptr;
    }
  }
};

struct EventConnectEventCtx : public ConnectEventCtx {
  ~EventConnectEventCtx() {
    if (ec) {
      bufferevent_disable((bufferevent *)ec, EV_READ | EV_WRITE);
      bufferevent_free((bufferevent *)ec);
      ec = nullptr;
    }
  }
};

EventTimerImpl::EventTimerImpl(Reactor *reactor) : TimerImpl(reactor) {}

int EventTimerImpl::start(Period period) {
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

int EventTimerImpl::snooze(Period period) {}

int EventTimerImpl::stop() {}

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

int EventReactorImpl::runSync() { return event_base_dispatch(base_); }
int EventReactorImpl::runAsync() {}
int EventReactorImpl::stop() { return event_base_loopexit(base_, 0); }

/*
int EventReactorImpl::register_event(int fd, const EventOptions* eo, CallBack
cb) { switch (eo->e_type) { case Event::LISTEN: { break;
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

ListenEventCtx *
EventReactorImpl::register_listen_event(int, const ListenEventOptions &leos) {
  // using lcb = void (*)(struct evconnlistener *, evutil_socket_t, struct
  // sockaddr *, int socklen, void *);
  auto *c = new ListenEventCtx();
  if (!c)
    return nullptr;
  *c->eos = leos;
  auto *ecl = evconnlistener_new_bind(base_, 0, c, leos.flags, leos.backlog,
                                      &leos.sa, leos.socklen);
  if (!ecl) {
    delete c;
    LOG(ERROR) << "ev listener new failed: " << strerror(errno);
    return nullptr;
  }
  c->ec = ecl;
  c->fd = evconnlistener_get_fd(ecl);

  auto event_cb = [](struct evconnlistener *, evutil_socket_t fd,
                     struct sockaddr *, int socklen, void *ctx) {
    auto *c = (ListenEventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler)
      handler->handle_accept(fd);
  };
  evconnlistener_set_cb(ecl, event_cb, c);

  auto err_cb = [](struct evconnlistener *, void *ctx) {
    auto *c = (ListenEventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler)
      handler->handle_event(c->fd, 0);
  };
  evconnlistener_set_error_cb(ecl, err_cb);
  return c;
}

int EventReactorImpl::unregister_listen_event(int fd, ListenEventCtx *ctx) {
  auto *ecl = (evconnlistener *)ctx->ec;
  evconnlistener_disable(ecl);
  evconnlistener_free(ecl);
  ctx->ec = nullptr;
  delete ctx;
  return 0;
}

ConnectEventCtx *
EventReactorImpl::register_connect_event(int, const ConnectEventOptions &ceos) {
  auto c = std::make_unique<ConnectEventCtx>();
  if (!c)
    return nullptr;
  bufferevent *be = create_bufferevent(-1, ceos.flags, ceos, c.get());
  if (!be)
    return nullptr;
  if (-1 == bufferevent_enable(be, EV_READ)) {
    LOG(ERROR) << "enable buffer read failed: " << strerror(errno);
    bufferevent_free(be);
    return nullptr;
  }
  if (0 != bufferevent_socket_connect(be, &ceos.sa, ceos.socklen)) {
    LOG(ERROR) << "sock connect failed: " << strerror(errno);
    bufferevent_free(be);
    return nullptr;
  }

  *c->eos = ceos;
  c->fd = bufferevent_getfd(be);
  c->ec = be;
  return c.release();
}

int EventReactorImpl::unregister_connect_event(int, ConnectEventCtx &ctx) {
  bufferevent_disable((bufferevent *)ctx.ec, EV_READ);
  bufferevent_free((bufferevent *)ctx.ec);
  delete &ctx;
  return 0;
}

ReadEventCtx *
EventReactorImpl::register_read_event(int fd, const ReadEventOptions &reos) {
  auto ctx = std::make_unique<ReadEventCtx>();
  if (!ctx)
    return nullptr;

  auto* be = create_bufferevent(fd, 0, reos, ctx.get());
  if (!be) return nullptr;

  if (-1 == bufferevent_enable(be, EV_READ)) {
    LOG(ERROR) << " enable buffer read failed: " << strerror(errno);
    bufferevent_free(be);
    return nullptr;
  }

  *ctx->eos = reos;
  ctx->fd = fd;
  ctx->ec = be;
  return ctx.release();
}

int EventReactorImpl::unregister_read_event(int, ReadEventCtx*) {

}

WriteEventCtx* EventReactorImpl::register_write_event(int, const WriteEventOptions&) {

}
int EventReactorImpl::unregister_write_event(int, WriteEventCtx*) {

}

bufferevent *EventReactorImpl::create_bufferevent(int fd, int fd_flag,
                                                  const EventOptions &eos,
                                                  EventCtx *ctx) {
  bufferevent *be = bufferevent_socket_new(base_, -1, fd_flag);
  if (!be) {
    return nullptr;
  }
  auto read_cb = [](struct bufferevent *bev, void *ctx) {
    EventCtx *c = (EventCtx *)ctx;
    evbuffer *buf = bufferevent_get_input(bev);
    auto handler = c->eos->handler.lock();
    if (handler)
      handler->handle_read(buf, evbuffer_get_length(buf));
  };
  auto write_cb = [](struct bufferevent *bev, void *ctx) {
    EventCtx *c = (EventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler)
      handler->handle_write(bufferevent_get_output(bev), 0);
  };
  auto e_cb = [](struct bufferevent *bev, short what, void *ctx) {
    EventCtx *c = (EventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler)
      handler->handle_event(c->fd, what);
  };
  bufferevent_setcb(be, read_cb, write_cb, e_cb, ctx);
  return be;
}

EventOptions *
EventReactorImpl::new_listen_event_opt(std::shared_ptr<EventHandler> handler,
                                       int listen_flags, int backlog,
                                       const sockaddr *sa, int socklen) {
  ListenEventOptions *leos = new ListenEventOptions();
  if (leos) {
    leos->e_type = Event::LISTEN;
    leos->handler = handler;
    leos->flags = listen_flags;
    leos->backlog = backlog;
    memcpy(&leos->sa, sa, socklen);
    leos->socklen = socklen;
  }
  return leos;
}

EventOptions *
EventReactorImpl::new_connect_event_opt(std::shared_ptr<EventHandler> handler,
                                        const sockaddr *sa, int socklen,
                                        int flags) {
  ConnectEventOptions *ceos = new ConnectEventOptions();
  if (ceos) {
    ceos->e_type = Event::CONNECT;
    ceos->handler = handler;
    memcpy(&ceos->sa, sa, socklen);
    ceos->socklen = socklen;
    ceos->flags = flags;
  }
  return ceos;
}

EventOptions *
EventReactorImpl::new_read_event_opt(std::shared_ptr<EventHandler> handler) {
  ReadEventOptions* reos = new ReadEventOptions();
  if (reos) {
    reos->e_type = Event::READ;
    reos->handler = handler;
  }
  return reos;
}

EventOptions*EventReactorImpl::new_write_event_opt(std::shared_ptr<EventHandler> handler) {
  WriteEventOptions* weos = new WriteEventOptions();
  if (weos) {
    weos->e_type = Event::WRITE;
    weos->handler = handler;
  }
  return weos;
}

EventOptions* EventReactorImpl::new_timeout_event_opt(std::shared_ptr<EventHandler> handler) {
  //TimeoutEventOptions* teos = new TimeoutEventOptions();
}

} // namespace reactor
