#pragma once
#include "reactor.h"
#include "timer.h"
#include <functional>
#include <memory>
#include <sys/socket.h>
#include <unordered_map>

namespace reactor {

struct ReactorImpl {
  friend class Reactor;
  virtual ~ReactorImpl() = default;
  virtual int runSync() = 0;
  virtual int runAsync() = 0;
  virtual int stop() = 0;
  virtual int brk() = 0;
protected:
  virtual ListenEventCtx *register_listen_event(int, const ListenEventOptions &eos) = 0;
  virtual int unregister_listen_event(int, ListenEventCtx *ctx) = 0;

  virtual ConnectEventCtx *register_connect_event(int, const ConnectEventOptions &eos) = 0;
  virtual int unregister_connect_event(int, ConnectEventCtx *ctx) = 0;

  virtual ReadEventCtx* register_read_event(int, const ReadEventOptions&) = 0;
  virtual int unregister_read_event(int, ReadEventCtx*) = 0;

  virtual WriteEventCtx* register_write_event(int, const WriteEventOptions&) = 0;
  virtual int unregister_write_event(int, WriteEventCtx*) = 0;

  virtual TimeoutEventCtx* register_timeout_event(int, const TimeoutEventOptions&) = 0;
  virtual int unregister_timeout_event(int, TimeoutEventCtx*) = 0;
};

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

struct WriteEventOptions : public EventOptions {
  WriteEventOptions() : EventOptions(Event::WRITE) {}
};

struct TimeoutEventOptions : public EventOptions {
  TimeoutEventOptions() : EventOptions(Event::TIMEOUT) {}
  TimeoutEventOptions& operator=(const TimeoutEventOptions& teos) {
    EventOptions::operator=(teos);
    timeout = teos.timeout;
    return *this;
  }

  Period timeout;
};

struct EventCtx {
  EventCtx() : eos(nullptr) {}
  virtual ~EventCtx() {
    delete eos;
    if (ec_deleter) ec_deleter(ec);
    ec = nullptr;
  }
  EventOptions* eos;
  int fd;
  void* ec;
  std::function<void(void*ec)> ec_deleter;
};

class EventMap {
public:
  EventMap() = default;
  ~EventMap();

  int add_event(int fd, Event e, EventCtx* data);
  EventCtx* remove_event(int fd, Event e);
  EventCtx* get_ctx(int fd, Event e);

private:
  auto find_ctx(int fd, Event e);
private:
  std::unordered_map<int, std::unordered_map<Event, EventCtx *>> m_;
};

struct ListenEventCtx : public EventCtx{
  ListenEventCtx() : EventCtx() {
    eos = new ListenEventOptions();
  }
};

struct ConnectEventCtx : public EventCtx {
  ConnectEventCtx() : EventCtx() {
    eos = new ConnectEventOptions();
  }
};

struct ReadEventCtx : public EventCtx {
  ReadEventCtx() : EventCtx() {
    eos = new ReadEventOptions();
  }
};

struct WriteEventCtx : public EventCtx {
  WriteEventCtx() : EventCtx() {
    eos = new WriteEventOptions();
  }
};

struct TimeoutEventCtx : public EventCtx {
  TimeoutEventCtx() : EventCtx() {
    eos = new TimeoutEventOptions();
  }
};

struct TimerImpl {
  TimerImpl(Reactor* reactor) : reactor_(reactor) {}
  virtual ~TimerImpl() = default;
  virtual int start(Period period) = 0;
  virtual int snooze(Period period) = 0;
  virtual int stop() = 0;

protected:
  Reactor* reactor_;
};

}
