#include "event_reactor_impl.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"
#include "event2/thread.h"
#include "reactor_impl.h"
#include "timer.h"
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
  auto *teos =
      EventReactorImpl::new_timeout_event_opt(get_opts().handler, period);
  std::shared_ptr<EventOptions> ptr_guard{teos};
  get_opts().period = period;
  return reactor_->register_event(-1, *ptr_guard.get());
}

int EventTimerImpl::snooze(Period period) {
  return -1;
}

int EventTimerImpl::stop() {
  return -1;
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
  return stopped ? -1 : event_base_loop(base_, EVLOOP_NO_EXIT_ON_EMPTY);
}

int EventReactorImpl::runAsync() {}

int EventReactorImpl::stop() {
  int ret = event_base_loopexit(base_, 0);
  if (ret == 0)
    stopped = true;
  return ret;
}

int EventReactorImpl::brk() { return event_base_loopbreak(base_); }

ListenEventCtx *
EventReactorImpl::register_listen_event(int, const ListenEventOptions &leos) {
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
  c->ec_deleter = [](void *ec) {
    if (ec) {
      auto *evcl = (evconnlistener *)ec;
      evconnlistener_disable(evcl);
      evconnlistener_free(evcl);
    }
  };

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
  c->ec_deleter = [](void *ec) {
    if (ec) {
      auto *bev = (bufferevent *)ec;
      bufferevent_disable(bev, EV_READ);
      bufferevent_free(bev);
    }
  };
  return c.release();
}

int EventReactorImpl::unregister_connect_event(int, ConnectEventCtx *ctx) {
  return 0;
}

ReadEventCtx *
EventReactorImpl::register_read_event(int fd, const ReadEventOptions &reos) {
  auto ctx = std::make_unique<ReadEventCtx>();
  if (!ctx)
    return nullptr;

  auto *be = create_bufferevent(fd, 0, reos, ctx.get());
  if (!be)
    return nullptr;

  if (-1 == bufferevent_enable(be, EV_READ)) {
    LOG(ERROR) << " enable buffer read failed: " << strerror(errno);
    bufferevent_free(be);
    return nullptr;
  }

  *ctx->eos = reos;
  ctx->fd = fd;
  ctx->ec = be;
  ctx->ec_deleter = [](void *ec) {
    if (ec) {
      auto *bev = (bufferevent *)ec;
      bufferevent_disable(bev, EV_READ);
      bufferevent_free(bev);
    }
  };
  return ctx.release();
}

int EventReactorImpl::unregister_read_event(int, ReadEventCtx *ctx) {
  return 0;
}

WriteEventCtx *
EventReactorImpl::register_write_event(int fd, const WriteEventOptions &weos) {
  auto ctx = std::make_unique<WriteEventCtx>();
  if (!ctx)
    return nullptr;

  auto *be = create_bufferevent(fd, 0, weos, ctx.get());
  if (!be)
    return nullptr;

  if (-1 == bufferevent_enable(be, EV_WRITE)) {
    LOG(ERROR) << "enable buffer write failed: " << strerror(errno);
    bufferevent_free(be);
    return nullptr;
  }

  *ctx->eos = weos;
  ctx->fd = fd;
  ctx->ec = be;
  ctx->ec_deleter = [](void *ec) {
    if (ec) {
      auto *bev = (bufferevent *)ec;
      bufferevent_disable(bev, EV_WRITE);
      bufferevent_free(bev);
    }
  };
  return ctx.release();
}

int EventReactorImpl::unregister_write_event(int, WriteEventCtx *ctx) {
  return 0;
}

TimeoutEventCtx *
EventReactorImpl::register_timeout_event(int, const TimeoutEventOptions &teos) {
  auto ctx = std::make_unique<TimeoutEventCtx>();
  if (!ctx)
    return nullptr;

  auto te_cb = [](int, short int, void *ctx) {
    auto *c = (TimeoutEventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler) {
      handler->handle_timeout();
    }
  };

  auto *et = evtimer_new(base_, te_cb, ctx.get());
  if (!et)
    return nullptr;
  struct timeval t = {.tv_sec = 0, .tv_usec = 0};
  t.tv_usec = get_usecs(teos.timeout);
  if (0 != evtimer_add(et, &t)) {
    LOG(ERROR) << "add timer event failed: " << strerror(errno);
    return nullptr;
  }

  ctx->ec = et;
  *ctx->eos = teos;
  ctx->fd = event_get_fd(et);
  ctx->ec_deleter = [](void *ec) {
    if (ec) {
      auto *et = (event *)ec;
      evtimer_del(et);
      event_free(et);
    }
  };

  return ctx.release();
}

int EventReactorImpl::unregister_timeout_event(int, TimeoutEventCtx *ctx) {
  return 0;
}

bufferevent *EventReactorImpl::create_bufferevent(int fd, int fd_flag,
                                                  const EventOptions &eos,
                                                  EventCtx *ctx) {
  bufferevent *be = bufferevent_socket_new(base_, fd, fd_flag);
  if (!be) {
    return nullptr;
  }
  auto read_cb = [](struct bufferevent *bev, void *ctx) {
    EventCtx *c = (EventCtx *)ctx;
    evbuffer *buf = bufferevent_get_input(bev);
    auto len = evbuffer_get_length(buf);
    auto handler = c->eos->handler.lock();
    if (handler && len > 0) {
      auto *p = evbuffer_pullup(buf, len);
      handler->handle_read(p, len);
      evbuffer_drain(buf, len);
    }
  };
  auto write_cb = [](struct bufferevent *bev, void *ctx) {
    EventCtx *c = (EventCtx *)ctx;
    auto handler = c->eos->handler.lock();
    if (handler) {
      evbuffer *buf = bufferevent_get_output(bev);
      const char *data = nullptr;
      size_t len = 0;
      handler->handle_write(data, &len);
      if (data && len > 0) {
        evbuffer_add(buf, data, len);
      }
    }
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
  ReadEventOptions *reos = new ReadEventOptions();
  if (reos) {
    reos->e_type = Event::READ;
    reos->handler = handler;
  }
  return reos;
}

EventOptions *
EventReactorImpl::new_write_event_opt(std::shared_ptr<EventHandler> handler) {
  WriteEventOptions *weos = new WriteEventOptions();
  if (weos) {
    weos->e_type = Event::WRITE;
    weos->handler = handler;
  }
  return weos;
}

EventOptions *
EventReactorImpl::new_timeout_event_opt(std::shared_ptr<EventHandler> handler, Period timeout) {
  TimeoutEventOptions *teos = new TimeoutEventOptions();
  if (teos) {
    teos->e_type = Event::TIMEOUT;
    teos->handler = handler;
    teos->timeout = timeout;
  }
  return teos;
}

} // namespace reactor
