#pragma once
#include "reactor.h"
#include <event2/event.h>

namespace reactor {
struct EventOptions;
struct ListenEventOptions;
struct ConnectEventOptions;
struct WriteEventOptions;
struct ReadEventOptions;
struct TimeoutEventOptions;

struct EventCtx;
struct ListenEventCtx;
struct ConnectEventCtx;
struct WriteEventCtx;
struct ReadEventCtx;
struct TimeoutEventCtx;

struct EventReactorImpl : public ReactorImpl {
  EventReactorImpl();
  virtual ~EventReactorImpl();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;

  static EventOptions *
  new_listen_event_opt(std::shared_ptr<EventHandler> handler, int listen_flags,
                       int backlog, const sockaddr *sa, int socklen);
  virtual ListenEventCtx *
  register_listen_event(int fd, const ListenEventOptions *eos) override;
  virtual int unregister_listen_event(int, ListenEventCtx *ctx) override;

  static EventOptions *
  new_connect_event_opt(std::shared_ptr<EventHandler> handler,
                        const sockaddr *sa, int socklen, int flags);
  virtual ConnectEventCtx *
  register_connect_event(int, const ConnectEventOptions *) override;
  virtual int unregister_connect_event(int, ConnectEventCtx *ctx) override;

  static EventOptions* new_read_event_opt(std::shared_ptr<EventHandler> handler);
  virtual ReadEventCtx* register_read_event(int, const ReadEventOptions*) override;
  virtual int unregister_read_event(int, ReadEventCtx*) override;

  static EventOptions* new_write_event_opt(std::shared_ptr<EventHandler> handler);
  virtual WriteEventCtx* register_write_event(int, const WriteEventOptions*) override;
  virtual int unregister_write_event(int, WriteEventCtx*) override;

  virtual TimeoutEventCtx* register_timeout_event(int, const TimeoutEventOptions*) override;
  virtual int unregister_timeout_event(int, TimeoutEventCtx*) override;
private:
  event_base* base_;
};

struct EventTimerImpl : public TimerImpl, public std::enable_shared_from_this<EventTimerImpl> {
  EventTimerImpl(Reactor* rector);
  virtual int start(Period period) override;
  virtual int snooze(Period period) override;
  virtual int stop() override;

  private:
};

}
