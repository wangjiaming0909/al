#include "reactor.h"
#include "reactor_impl.h"
#include <glog/logging.h>
#include <thread>

namespace reactor {

Reactor::Reactor(ReactorImpl *impl)
    : impl_(impl), em_(new EventMap{}), state_(State::STOPPED),
      empty_events_lock_(), empty_events_cond_() {}

Reactor::~Reactor() {
  delete em_;
  delete impl_;
  if (thd_) {
    if (thd_->joinable())
      thd_->join();
    thd_.reset();
  }
}

int Reactor::runSync() {
  auto expected_state = State::STOPPED;
  if (!state_.compare_exchange_strong(expected_state, State::STARTED,
                                      std::memory_order_relaxed,
                                      std::memory_order_relaxed)) {
    return 0;
  }
  auto ret = impl_->runSync();
  expected_state = State::STARTED;
  // don't care about whether succeeded
  state_.compare_exchange_strong(expected_state, State::STOPPED,
                                 std::memory_order_relaxed,
                                 std::memory_order_relaxed);
  return ret;
}

int Reactor::runAsync() {
  auto run = [&]() {
    LOG(INFO) << "reactor started";
    runSync();
    LOG(INFO) << "reactor stopped";
  };
  if (thd_ && thd_->joinable()) {
    thd_->join();
  }
  thd_.reset(new std::thread{run});
  return 0;
}

int Reactor::stop() {
  LOG(INFO) << "stop reactor: " << this;
  return impl_->stop();
}

std::weak_ptr<EventCtx> Reactor::register_event(int fd, const EventOptions &eos) {
  EventCtx *ctx = nullptr;
  std::shared_ptr<EventCtx> ret{nullptr};
  switch (eos.e_type) {
  case Event::LISTEN:
    ctx = impl_->register_listen_event(fd, (const ListenEventOptions &)eos);
    if (ctx)
      LOG(INFO) << "register listen event: " << ctx->fd;
    break;
  case Event::CONNECT:
    ctx = impl_->register_connect_event(fd, (const ConnectEventOptions &)eos);
    if (ctx)
      LOG(INFO) << "register connect event: " << ctx->fd;
    break;
  case Event::READ:
    LOG(INFO) << "register read event: " << fd;
    ctx = impl_->register_read_event(fd, (const ReadEventOptions &)eos);
    break;
  case Event::WRITE: {
    LOG(INFO) << "register write event: " << fd;
    ctx = impl_->register_write_event(fd, (const WriteEventOptions &)eos);
    break;
  }
  case Event::TIMEOUT:
    ctx = impl_->register_timeout_event(fd, (const TimeoutEventOptions &)eos);
    if (ctx) LOG(INFO) << "register timeout event: " << ctx->fd << " ctx: " << ctx;
    break;
  }
  if (ctx) {
    ret.reset(ctx);
    fd = ctx->fd;
    em_->add_event(ret);
  } else {
    LOG(ERROR) << "register event failed: " << strerror(errno);
  }
  return ret;
}

int Reactor::unregister_event(std::shared_ptr<EventCtx> ctx) {
  LOG(INFO) << "unregister e: " << int(ctx->eos->e_type)
            << " for fd: " << ctx->fd << " ctx: " << ctx;
  return em_->remove_event(ctx) ? 0 : -1;
}

} // namespace reactor
