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
  //TODO add param, LOOP_ONCE, EXIT_ON_EMPTY...
  /// @brief starting up reactor, until error or stop called
  /// @note runSync won't return even if no fds polling
  virtual int runSync() override;
  /// @brief start reactor async
  /// @retval 0 if start succeeded or already started
  /// @retval -1 error occurred
  virtual int runAsync() override;
  /// @brief stop the event loop
  /// @retval 0 if stop succeeded or already stopped
  /// @retval -1 if error occurred
  virtual int stop() override;
  virtual int brk() override;
  virtual int register_event(int fd, const EventOptions& eos) override;
  virtual int unregister_event(int fd, Event) override;

private:
  EventMap* em_;
  ReactorImpl* impl_;
  std::unique_ptr<std::thread> thd_;
  enum class State {UNKNOWN = 0, STARTED = 1, STOPPED = 2, ERROR = 3};
  State state_;
};

}
