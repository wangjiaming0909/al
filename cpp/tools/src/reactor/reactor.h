#pragma once
#include <boost/noncopyable.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <sys/socket.h>

struct event_base;
struct event;
struct evconnlistener;
struct sockaddr;

namespace reactor {

using Period = std::chrono::milliseconds;
using TimerCallBack = std::function<void(void)>;
using CallBack = TimerCallBack;

enum class Event { READ, WRITE, TIMEOUT, LISTEN };
struct EventOptions {
  Event e_type;
  CallBack cb;
  void *data;
};

struct ListenEventOptions : public EventOptions{
  unsigned int flags; // listen fd opts
  int backlog;
  sockaddr sa;
  int socklen;
  CallBack err_cb;
};

struct IReactor {
  IReactor() = default;
  virtual ~IReactor() = default;
  virtual int runSync() = 0;
  virtual int runAsync() = 0;
  virtual int stop() = 0;
  virtual int register_event(int, const EventOptions *) { return -1; }
  virtual int unregister_event(int fd, Event) = 0;

  protected:
    virtual int register_listen_event(int, const ListenEventOptions *) {
      return -1;
    }
};

using ReactorImpl = IReactor;

struct EventReactorImpl : public ReactorImpl {
  EventReactorImpl();
  ~EventReactorImpl();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;
  virtual int unregister_event(int fd, Event) override;

  static void new_listen_event_opt(ListenEventOptions &leo, CallBack cb,
                                   CallBack err_cb, int listen_flags,
                                   int backlog, const sockaddr *sa, int socklen);

private:
  virtual int register_listen_event(int fd, const ListenEventOptions* eo) override;

private:
  event_base* base_;
};

struct Reactor : public IReactor {
  Reactor(ReactorImpl* impl) : impl_(impl) {}
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;
  virtual int register_event(int fd, const EventOptions* eo) override;
  virtual int unregister_event(int fd, Event) override;

  ReactorImpl* impl_;
};

struct TimerImpl {
  TimerImpl(Reactor* reactor) : reactor_(reactor) {}
  virtual ~TimerImpl() = default;
  virtual int start(Period period, TimerCallBack cb) = 0;
  virtual int snooze(Period period) = 0;
  virtual int stop() = 0;

protected:
  TimerCallBack cb_;
  //TODO if needed, use unique_ptr
  Reactor* reactor_;
};

struct EventTimerImpl : public TimerImpl, public std::enable_shared_from_this<EventTimerImpl> {
  EventTimerImpl(Reactor* rector);
  virtual int start(Period period, TimerCallBack cb) override;
  virtual int snooze(Period period) override;
  virtual int stop() override;

  private:
};

class Timer : boost::noncopyable{
public:
  class Options {
    Options() : impl(0) {}
    ~Options() {}
    TimerImpl* impl;
  };

  static std::shared_ptr<Timer> create(const Options &opts, TimerCallBack cb,
                                       Period period);

  Timer(Reactor* reactor, const Options &opts, TimerCallBack cb, Period period);
  ~Timer();
  void start(Period period);
  void snooze(Period period);
  void stop();

private:
  TimerCallBack cb_;
  Period period_;
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

}
