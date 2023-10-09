#pragma once
#include "reactor.h"
#include <boost/noncopyable.hpp>
#include <chrono>
#include <memory>

namespace reactor {

class TimerImpl;

using Period = std::chrono::milliseconds;

uint64_t get_usecs(Period period);
uint64_t get_millisecs(Period period);

class Timer : boost::noncopyable{
public:
  friend class TimerImpl;
  struct Options {
    Options(std::shared_ptr<EventHandler> handler, Period period)
        : handler(handler), period(period) {}
    ~Options() {}
    Period period;
    std::shared_ptr<EventHandler> handler;
  };

  static std::shared_ptr<Timer> create(const Options &opts, TimerImpl* impl);

  ~Timer();
  void start(Period period);
  void snooze(Period period);
  void stop();
  const Options &get_opts() const { return opts_; }

protected:
  Options& get_opts() {return opts_;}
  Timer(const Options &opts, TimerImpl *impl);

private:
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

}
