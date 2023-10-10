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
  EventCtx *start(Period period);
  EventCtx *snooze(EventCtx* ctx, Period period);
  void stop(EventCtx *ctx);
  const Options &get_opts() const { return opts_; }
  void set_opts(const Options &opts) { opts_ = opts; }

protected:
  Options& get_opts() {return opts_;}
  Timer(const Options &opts, TimerImpl *impl);

private:
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

}
