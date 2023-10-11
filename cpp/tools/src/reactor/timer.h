#pragma once
#include "reactor.h"
#include <boost/noncopyable.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <any>

namespace reactor {

class TimerImpl;

using Period = std::chrono::milliseconds;

uint64_t get_usecs(Period period);
uint64_t get_millisecs(Period period);

class Timer : boost::noncopyable{
public:
  template <typename T>
  using TimerCallBackT = std::function<void(std::shared_ptr<T>)>;

  friend class TimerImpl;
  struct Options {
    Options(Period period)
        : period(period) {}
    ~Options() {}
    Period period;
  };

  static std::unique_ptr<Timer> create(const Options &opts, TimerImpl* impl);

  ~Timer();
  std::weak_ptr<EventCtx> start(Period period,
                                  std::shared_ptr<EventHandler> handler);
  template <typename T>
  std::weak_ptr<EventCtx> start(Period period, std::shared_ptr<T> data,
                                  TimerCallBackT<T> timer_cb);
  std::weak_ptr<EventCtx> snooze(std::shared_ptr<EventCtx> ctx,
                                   Period period);
  void stop(std::shared_ptr<EventCtx> ctx);
  const Options &get_opts() const { return opts_; }
  void set_opts(const Options &opts) { opts_ = opts; }

protected:
  Options& get_opts() {return opts_;}
  Timer(const Options &opts, TimerImpl *impl);

private:
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

// TODO remove TimerImpl
struct TimerImpl {
  TimerImpl(Reactor *reactor) : reactor_(reactor), base_(nullptr) {}
  virtual ~TimerImpl() = default;
  std::weak_ptr<EventCtx> start(Period period,
                                  std::shared_ptr<EventHandler> handler) {
    return start(period, handler, false);
  }

  template <typename T>
  std::weak_ptr<EventCtx> start(Period period, std::shared_ptr<T> data,
                  Timer::TimerCallBackT<T> timer_cb);
  virtual std::weak_ptr<EventCtx> snooze(std::shared_ptr<EventCtx> ctx, Period period) = 0;
  virtual int stop(std::shared_ptr<EventCtx> ctx) = 0;
  void set_base(Timer *base) { base_ = base; }
  Timer::Options &get_opts() { return base_->get_opts(); }

protected:
  /// @brief schedule a new timer
  /// @param period, timer timeout
  /// @handler timeout handler, handle_timeout will be called
  /// @internal_handler when schedule timeout with EventHandler, always false
  ///                   when schedule timeout with callbacks, always true
  /// @retval nullptr if start failed
  /// @retval event ctx if succeed
  virtual std::weak_ptr<EventCtx> start(Period period,
                                          std::shared_ptr<EventHandler> handler,
                                          bool internal_handler) = 0;

protected:
  Reactor* reactor_;
  Timer* base_;
};

template <typename T>
std::weak_ptr<EventCtx> Timer::start(Period period, std::shared_ptr<T> data,
                       TimerCallBackT<T> timer_cb) {
  return impl_->start(period, data, timer_cb);
}

template <typename T>
std::weak_ptr<EventCtx> TimerImpl::start(Period period,
                                           std::shared_ptr<T> data,
                                           Timer::TimerCallBackT<T> timer_cb) {
  using TimerCBT = typeof(timer_cb);
  struct InternalTimerHandler : public reactor::EventHandler {
    virtual void handle_timeout() {
      if (auto d = data.lock()) {
        cb(d);
      }
    }
    std::weak_ptr<T> data;
    TimerCBT cb;
  };

  auto *handler = new InternalTimerHandler();
  handler->data = data;
  handler->cb = timer_cb;
  std::shared_ptr<EventHandler> internal_handler{handler};
  auto ctx = start(period, internal_handler, true);
  return ctx;
}
}
