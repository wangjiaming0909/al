#pragma once

#include <cstddef>
#include <memory>
namespace std {
class thread;
}
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

struct ReactorImpl;

class EventHandler {
public:
  EventHandler() = default;
  virtual ~EventHandler() = default;

  virtual void handle_accept(int fd) = 0;
  virtual void handle_event(int fd, int what) = 0;
  virtual void handle_read(void *buffer, size_t len) = 0;
  virtual void handle_write(const char *&bp, size_t*) = 0;
  virtual void handle_timeout() = 0;
};

enum class Event {
  TIMEOUT = 0x01,
  READ = 0x02,
  WRITE = 0x04,
  LISTEN = 0x08,
  CONNECT = 0x10
};

struct IReactor {
  IReactor() = default;
  virtual ~IReactor() = default;
  virtual int runSync() = 0;
  virtual int runAsync() = 0;
  virtual int stop() = 0;
  virtual int brk() = 0;
  virtual int register_event(int, const EventOptions &) = 0;
  virtual int unregister_event(int fd, Event) = 0;
};

class EventMap;
struct Reactor : public IReactor {
  Reactor(ReactorImpl* impl);
  ~Reactor();
  virtual int runSync() override;
  virtual int runAsync() override;
  virtual int stop() override;
  virtual int brk() override;
  virtual int register_event(int fd, const EventOptions& eos) override;
  virtual int unregister_event(int fd, Event) override;

private:
  EventMap* em_;
  ReactorImpl* impl_;
  std::unique_ptr<std::thread> thd_;
};

}
