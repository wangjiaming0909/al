#pragma once

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <atomic>
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

enum class Event : int {
  TIMEOUT = 0x01,
  READ = 0x02,
  WRITE = 0x04,
  LISTEN = 0x08,
  CONNECT = 0x10
};

class EventMap;
struct Reactor {
  Reactor(ReactorImpl* impl);
  ~Reactor();
  //TODO add param, LOOP_ONCE, EXIT_ON_EMPTY...
  /// @brief starting up reactor, until error or stop called
  /// @retval 0 if succeeded, -1 if error
  /// @note wont return if no pending events, use stop
  int runSync();
  /// @brief start reactor async
  /// @retval 0
  int runAsync();
  /// @brief stop the event loop
  /// @retval 0 if stop succeeded or already stopped
  /// @retval -1 if error occurred
  int stop();

  /// @brief register an event into reactor
  /// @param fd, for read/write events, fd should be the connected fd,
  ///            otherwise, fd is not used
  /// @param eos, the opts for this event
  /// @retval the ctx for this registerd event
  /// @retval NULL if register failed, check errno
  EventCtx* register_event(int fd, const EventOptions& eos);
  /// @brief unregister a event from reactor, if succeeded, ctx is deleted
  /// @param ctx, the ret pointer when register_event
  /// @retval 0 if success, -1 for ctx not found
  int unregister_event(EventCtx* ctx);

private:
  EventMap* em_;
  ReactorImpl* impl_;
  std::unique_ptr<std::thread> thd_;
  enum class State {UNKNOWN = 0, STARTED = 1, STOPPED = 2, ERROR = 3, BREAK = 4};
  std::atomic<State> state_;
  std::mutex empty_events_lock_;
  std::condition_variable empty_events_cond_;
};

}
