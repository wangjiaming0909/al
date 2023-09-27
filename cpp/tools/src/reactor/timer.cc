#include "timer.h"
#include "reactor_impl.h"

namespace reactor {

std::shared_ptr<Timer> Timer::create(const Options &opts, TimerImpl* impl) {
  auto *timer = new Timer(opts, impl);
  return std::shared_ptr<Timer>(timer);
}

Timer::~Timer() {}

void Timer::start(Period period) { impl_->start(period); }

void Timer::snooze(Period period) { impl_->snooze(period); }

void Timer::stop() { impl_->stop(); }

Timer::Timer(const Options &opts, TimerImpl *impl) : opts_(opts) {
  impl_.reset(impl);
  impl_->set_base(this);
}

uint64_t get_usecs(Period period) {
  return std::chrono::microseconds(period).count();
}
}
