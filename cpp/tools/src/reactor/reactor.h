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
using CallBack = std::function<void(void*)>;
using AcceptCb = std::function<void(int fd, void*)>;
using AcceptErrCb = CallBack;
using ConnectEventCb = std::function<void(int what, void*)>;
using ReadCb = std::function<void(const char*, size_t len, void*)>;
using WriteCb = std::function<void(char*, size_t len, void*)>;

struct EventCtx;
struct ListenEventCtx;
struct ConnectEventCtx;

enum class Event { READ = 1, WRITE, TIMEOUT, LISTEN, CONNECT };
struct EventOptions {
  EventOptions(Event e) : e_type(e), user_data(0) {}
  EventOptions& operator=(const EventOptions& eos) {
    e_type = eos.e_type;
    user_data = eos.user_data;
    return *this;
  }
  Event e_type;
  void *user_data;
};

struct ListenEventOptions : public EventOptions{
  ListenEventOptions()
      : EventOptions(Event::LISTEN), flags(0), backlog(0), sa(), socklen(0) {}
  ListenEventOptions& operator=(const ListenEventOptions& leos) {
    EventOptions::operator=(leos);
    flags = leos.flags;
    backlog = leos.backlog;
    sa = leos.sa;
    socklen = leos.socklen;
    read_cb = leos.read_cb;
    event_cb = leos.event_cb;
    return *this;
  }
  unsigned int flags; // listen fd opts
  AcceptCb read_cb;
  AcceptErrCb event_cb;
  int backlog;
  sockaddr sa;
  int socklen;
};

struct ConnectEventOptions : public EventOptions {
  ConnectEventOptions() : EventOptions(Event::CONNECT) {}
  ConnectEventOptions& operator=(const ConnectEventOptions& ceos) {
    EventOptions::operator=(ceos);
    sa = ceos.sa;
    socklen = ceos.socklen;
    flags = ceos.flags;
    event_cb = ceos.event_cb;
    read_cb = ceos.read_cb;
    write_cb = ceos.write_cb;
    return *this;
  }

  ConnectEventCb event_cb;
  ReadCb read_cb;
  WriteCb write_cb;
  sockaddr sa;
  int socklen;
  int flags;
};

struct IReactor {
  IReactor() = default;
  virtual ~IReactor() = default;
  virtual int runSync() = 0;
  virtual int runAsync() = 0;
  virtual int stop() = 0;
  virtual int register_event(int, const EventOptions *) = 0;
  virtual int unregister_event(int fd, Event) = 0;

protected:
};

struct ReactorImpl {
  virtual int runSync() = 0;
  virtual int runAsync() = 0;
  virtual int stop() = 0;
  virtual ListenEventCtx *register_listen_event(int,
                                                const ListenEventOptions *) = 0;
  virtual int unregister_listen_event(int, ListenEventCtx *ctx) = 0;

  virtual ConnectEventCtx *
  register_connect_event(int, const ConnectEventOptions *) = 0;
  virtual int unregister_connect_event(int, ConnectEventCtx *ctx) = 0;
};

struct EventReactorImpl : public ReactorImpl {
  EventReactorImpl();
  ~EventReactorImpl();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;

  static void new_listen_event_opt(ListenEventOptions &leos, AcceptCb cb,
                                   AcceptErrCb err_cb, int listen_flags,
                                   int backlog, const sockaddr *sa, int socklen);

  static void new_connect_event_opt(ConnectEventOptions &ceos,
                                    const sockaddr *sa, int socklen, int flags,
                                    CallBack rd_cb, CallBack wr_cb,
                                    CallBack e_cb);

  virtual ListenEventCtx* register_listen_event(int fd, const ListenEventOptions* eos) override;
  virtual int unregister_listen_event(int, ListenEventCtx* ctx) override;

  virtual ConnectEventCtx *
  register_connect_event(int, const ConnectEventOptions *) override;
  virtual int unregister_connect_event(int, ConnectEventCtx *ctx) override;

private:
  event_base* base_;
};

class EventMap;
struct Reactor : public IReactor {
  Reactor(ReactorImpl* impl);
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;
  virtual int register_event(int fd, const EventOptions* eos) override;
  virtual int unregister_event(int fd, Event) override;

private:
  ReactorImpl* impl_;
  EventMap* em_;
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
