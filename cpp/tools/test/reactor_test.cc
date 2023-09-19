#include <arpa/inet.h>
#include <gtest/gtest.h>
#include "event2/bufferevent.h"
#include "reactor/reactor.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <thread>

struct DefaultEventHandler : public reactor::EventHandler {
  DefaultEventHandler(reactor::Reactor* reactor) : reactor(reactor) {}
  virtual void handle_accept(int fd) override {
    LOG(INFO) << "accepted new fd: " << fd;
    fds.push_back(fd);
  }
  virtual void handle_event(int fd, int what) override {
    if (what & BEV_EVENT_CONNECTED) {
      LOG(INFO) << " fd connected";
    } else {
      LOG(INFO) << "error happened: " << what << " err: " << strerror(errno);
    }
  }
  virtual void handle_read(void *buffer, size_t len) override {
    LOG(INFO) << "handle read: ";
  }
  virtual void handle_write(void *buffer, size_t) override {
    LOG(INFO) << "handle write: ";
  }
  virtual void handle_timeout() override {
    LOG(INFO) << "handle timeout: ";
  }
  std::vector<int> fds;
  reactor::Reactor* reactor;
};

TEST(reactor, normal) {
  using namespace reactor;

  EventReactorImpl* impl = new EventReactorImpl{};
  Reactor r{impl};
  ListenEventOptions leo;

  auto cb = [](int, void*) {
    LOG(INFO) << "normal cb";
  };
  auto err_cb = [](void*) {
    LOG(ERROR) << "err cb";
  };

  sockaddr_in sa{};
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr("127.0.0.1");
  sa.sin_port = 9999;

  std::shared_ptr<EventHandler> handler{new DefaultEventHandler{&r}};

  EventReactorImpl::new_listen_event_opt(leo, handler, 0, 10, (sockaddr *)&sa,
                                         sizeof(sockaddr_in));

  int fd = r.register_event(0, &leo);
  r.unregister_event(fd, Event::LISTEN);
  auto run = [&]() { r.runSync(); };
  std::thread t{run};

  ConnectEventOptions ceo;
  EventReactorImpl::new_connect_event_opt(ceo, handler, (sockaddr *)&sa,
                                          sizeof(sockaddr_in), BEV_OPT_CLOSE_ON_FREE);

  r.register_event(0, &ceo);

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2s);

  r.stop();
  t.join();
}
