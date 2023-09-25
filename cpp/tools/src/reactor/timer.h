#pragma once
#include "reactor.h"
#include <boost/noncopyable.hpp>
#include <chrono>
#include <memory>

namespace reactor {

class TimerImpl;

using Period = std::chrono::milliseconds;

uint64_t get_usecs(Period period);

class Timer : boost::noncopyable{
public:
  struct Options {
    Options(Reactor &rector, Period period)
        : reactor(reactor), period(period) {}
    ~Options() {}
    Reactor &reactor;
    Period period;
  };

  static std::shared_ptr<Timer> create(const Options &opts, TimerImpl* impl);

  ~Timer();
  void start(Period period);
  void snooze(Period period);
  void stop();

protected:
  Timer(const Options &opts, TimerImpl *impl);

private:
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

}
