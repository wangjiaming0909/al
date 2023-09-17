#include <arpa/inet.h>
#include <gtest/gtest.h>
#include "reactor/reactor.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <thread>

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

  EventReactorImpl::new_listen_event_opt(leo, cb, err_cb, 0, 10,
                                         (sockaddr *)&sa, sizeof(sockaddr_in));

  int fd = r.register_event(0, &leo);
  r.unregister_event(fd, Event::LISTEN);
  auto run = [&]() { r.runSync(); };
  std::thread t{run};



  r.stop();
  t.join();
}
