#pragma once
#include "reactor.h"
#include <unordered_map>

namespace reactor {

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

struct EventCtx {
  EventCtx() : eos(nullptr) {}
  virtual ~EventCtx() { delete eos; }
  EventOptions* eos;
  int fd;
  void* ec;
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

}
