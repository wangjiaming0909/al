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

struct EventCtx;
struct ListenEventCtx;
struct ConnectEventCtx;
struct ReadEventCtx;
struct WriteEventCtx;

class EventHandler {
public:
  EventHandler() = default;
  virtual ~EventHandler() = default;

  virtual void handle_accept(int fd) = 0;
  virtual void handle_event(int fd, int what) = 0;
  virtual void handle_read(void *buffer, size_t len) = 0;
  virtual void handle_write(void *buffer, size_t) = 0;
  virtual void handle_timeout() = 0;
};

enum class Event { READ = 1, WRITE, TIMEOUT, LISTEN, CONNECT };
struct EventOptions {
  EventOptions(Event e) : e_type(e), handler() {}
  EventOptions& operator=(const EventOptions& eos) {
    e_type = eos.e_type;
    handler = eos.handler;
    return *this;
  }
  Event e_type;
  std::weak_ptr<EventHandler> handler;
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
    return *this;
  }
  unsigned int flags; // listen fd opts
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
    return *this;
  }

  sockaddr sa;
  int socklen;
  int flags;
};

struct ReadEventOptions : public EventOptions {
  ReadEventOptions() : EventOptions(Event::READ) {}
  ReadEventOptions& operator=(const ReadEventOptions& reos) {
    EventOptions::operator=(reos);
    return *this;
  }
};

using WriteEventOptions = EventOptions;

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
  virtual ~ReactorImpl() = default;
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
  virtual ~EventReactorImpl();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;

  static void new_listen_event_opt(ListenEventOptions &leos,
                                   std::shared_ptr<EventHandler> handler,
                                   int listen_flags, int backlog,
                                   const sockaddr *sa, int socklen);
  virtual ListenEventCtx *
  register_listen_event(int fd, const ListenEventOptions *eos) override;
  virtual int unregister_listen_event(int, ListenEventCtx *ctx) override;

  static void new_connect_event_opt(ConnectEventOptions &ceos,
                                    std::shared_ptr<EventHandler> handler,
                                    const sockaddr *sa, int socklen, int flags);
  virtual ConnectEventCtx *
  register_connect_event(int, const ConnectEventOptions *) override;
  virtual int unregister_connect_event(int, ConnectEventCtx *ctx) override;

private:
  event_base* base_;
};

class EventMap;
struct Reactor : public IReactor {
  Reactor(ReactorImpl* impl);
  ~Reactor();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;
  virtual int register_event(int fd, const EventOptions* eos) override;
  virtual int unregister_event(int fd, Event) override;

private:
  EventMap* em_;
  ReactorImpl* impl_;
};

struct TimerImpl {
  TimerImpl(Reactor* reactor) : reactor_(reactor) {}
  virtual ~TimerImpl() = default;
  virtual int start(Period period) = 0;
  virtual int snooze(Period period) = 0;
  virtual int stop() = 0;

protected:
  //TODO if needed, use unique_ptr
  Reactor* reactor_;
};

struct EventTimerImpl : public TimerImpl, public std::enable_shared_from_this<EventTimerImpl> {
  EventTimerImpl(Reactor* rector);
  virtual int start(Period period) override;
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

  static std::shared_ptr<Timer> create(const Options &opts, Period period);

  Timer(Reactor* reactor, const Options &opts, Period period);
  ~Timer();
  void start(Period period);
  void snooze(Period period);
  void stop();

private:
  Period period_;
  Options opts_;
  std::unique_ptr<TimerImpl> impl_;
};

}
