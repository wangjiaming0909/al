#include "timer.h"
#include "reactor_impl.h"

namespace reactor {

std::unique_ptr<Timer> Timer::create(const Options &opts, TimerImpl* impl) {
  auto *timer = new Timer(opts, impl);
  return std::unique_ptr<Timer>(timer);
}

Timer::~Timer() {}

std::weak_ptr<EventCtx> Timer::start(Period period, std::shared_ptr<EventHandler> handler) {
  return impl_->start(period, handler);
}

std::weak_ptr<EventCtx> Timer::snooze(std::shared_ptr<EventCtx> ctx,
                                        Period period) {
  return impl_->snooze(ctx, period);
}

void Timer::stop(std::shared_ptr<EventCtx> ctx) { impl_->stop(ctx); }

Timer::Timer(const Options &opts, TimerImpl *impl) : opts_(opts) {
  impl_.reset(impl);
  impl_->set_base(this);
}

uint64_t get_usecs(Period period) {
  return std::chrono::microseconds(period).count();
}
uint64_t get_millisecs(Period period) {
  return std::chrono::milliseconds(period).count();
}
}
