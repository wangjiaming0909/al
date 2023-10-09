#include "event2/bufferevent.h"
#include "event2/event.h"
#include "event2/thread.h"
#include "reactor/reactor.h"
#include "reactor/event_reactor_impl.h"
#include "uv.h"
#include <arpa/inet.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
using namespace std::chrono_literals;

struct DefaultEventHandler
    : public reactor::EventHandler,
      public std::enable_shared_from_this<DefaultEventHandler> {
  DefaultEventHandler(reactor::Reactor *reactor) : reactor(reactor) {}
  virtual void handle_accept(int fd) override {
    LOG(INFO) << "accepted new fd: " << fd;
    fds.push_back(fd);
    auto* weos = reactor::EventReactorImpl::new_write_event_opt(shared_from_this());
    auto weos_ptr = std::shared_ptr<reactor::EventOptions>(weos);
    ASSERT_EQ(fd, reactor->register_event(fd, *weos)->fd);
  }
  virtual void handle_event(int fd, int what) override {
    if (what & BEV_EVENT_CONNECTED) {
      LOG(INFO) << " fd connected";
    } else {
      LOG(INFO) << "error happened: " << what << " err: " << strerror(errno);
    }
  }
  virtual void handle_read(void *buffer, size_t len) override {
    LOG(INFO) << "handle read: " << (char*)buffer;
  }
  virtual void handle_write(const char *&buffer, size_t* len) override {
    if (bytes_written < bytes_to_write) {
      buffer = "123456789";
      *len = 10;
      bytes_written += 10;
    }
  }
  virtual void handle_timeout() override { LOG(INFO) << "handle timeout: "; }
  std::vector<int> fds;
  reactor::Reactor *reactor;
  int bytes_written = 0;
  static const int bytes_to_write = 100;
};
using namespace reactor;
std::shared_ptr<Reactor> create_reactor_and_run() {
  EventReactorImpl* impl = new EventReactorImpl{};
  Reactor*r = new Reactor{impl};
  r->runAsync();
  return std::shared_ptr<Reactor>{r};
}

TEST(reactor, normal) {
  using namespace reactor;

  EventReactorImpl* impl = new EventReactorImpl{};
  Reactor r{impl};
  EventOptions *leo;

  auto cb = [](int, void*) {
    LOG(INFO) << "normal cb";
  };
  auto err_cb = [](void*) {
    LOG(ERROR) << "err cb";
  };

  sockaddr_in sa{};
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr("127.0.0.1");
  sa.sin_port = 10001;

  std::shared_ptr<EventHandler> handler{new DefaultEventHandler{&r}};

  leo = EventReactorImpl::new_listen_event_opt(handler, 0, 10, (sockaddr *)&sa,
                                               sizeof(sockaddr_in));
  auto leo_ptr_guard = std::shared_ptr<EventOptions>(leo);

  auto ctx = r.register_event(0, *leo);
  ASSERT_TRUE(ctx != nullptr);
  ASSERT_TRUE(ctx->fd != -1);
  auto run = [&]() {
    while (0 == r.runSync()) {
    }
  };
  std::thread t{run};

  EventOptions *ceo;
  ceo = EventReactorImpl::new_connect_event_opt(
      handler, (sockaddr *)&sa, sizeof(sockaddr_in), BEV_OPT_CLOSE_ON_FREE);

  auto ceo_ptr_guard = std::shared_ptr<EventOptions>(ceo);

  r.register_event(0, *ceo);

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2s);

  r.stop();
  t.join();
}

TEST(reactor, timer) {
  using namespace reactor;
  Period timeout = 1s;
  auto r = create_reactor_and_run();

  auto handler = std::shared_ptr<EventHandler>(new DefaultEventHandler{r.get()});
  Timer::Options opts{handler, timeout};
  auto timer = Timer::create(opts, new EventTimerImpl{r.get()});
  timer->start(timeout);
  std::this_thread::sleep_for(2s);
  ASSERT_EQ(0, r->stop());
}

TEST(reactor, libevent) {
  evthread_use_pthreads();
  auto* base = event_base_new();
  int a = 0;
  auto func = [&] () {
    event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
    a = -1;
  };

  std::thread t{func};

  std::this_thread::sleep_for(100ms);

  event_base_loopexit(base, 0);
  std::this_thread::sleep_for(1000ms);
  ASSERT_EQ(a, -1);
  t.join();
}

static void uv_timer_cb_1(uv_timer_t* timer) {
  return;
}

static void uv_async_stop_cb(uv_async_t* async) {
  uv_stop(async->loop);
}

TEST(reactor, uv) {
  uv_loop_t* loop = uv_loop_new();
  uv_loop_init(loop);

  uv_timer_t* timer = new uv_timer_t();
  uv_timer_init(loop, timer);
  uv_timer_start(timer, uv_timer_cb_1, 1000, 1);

  auto func = [&]() {
    LOG(INFO) << "uv started";
    uv_run(loop, UV_RUN_DEFAULT);
    LOG(INFO) << "uv stopped";
  };

  std::thread t{func};

  std::this_thread::sleep_for(100ms);
  uv_stop(loop);// uv_stop from other threads is useless
  LOG(INFO) << "uv stop called";

  uv_async_t* async = new uv_async_t();
  uv_async_init(loop, async, uv_async_stop_cb);
  std::this_thread::sleep_for(100ms);
  uv_async_send(async); // this will stop the event loop
  LOG(INFO) << "uv async stop sent";
  t.join();
}
