#include "reactor.h"
#include "reactor_impl.h"
#include <glog/logging.h>
#include <thread>

namespace reactor {

Reactor::Reactor(ReactorImpl *impl) : impl_(impl), em_(new EventMap{}) {}

Reactor::~Reactor() {
  delete em_;
  delete impl_;
}

int Reactor::runSync() { return impl_->runSync(); }

int Reactor::runAsync() {
  auto run = [&]() {
    LOG(INFO) << "reactor started";
    while (0 == runSync()) {
    }
    LOG(INFO) << "reactor stopped";
  };
  if (thd_) {
    thd_->join();
  }
  thd_.reset(new std::thread{run});
  return 0;
}

int Reactor::stop() { return impl_->stop(); }

int Reactor::brk() { return impl_->brk(); }

int Reactor::register_event(int fd, const EventOptions &eos) {
  EventCtx *ret = nullptr;
  switch (eos.e_type) {
  case Event::LISTEN:
    ret = impl_->register_listen_event(fd, (const ListenEventOptions &)eos);
    if (ret)
      LOG(INFO) << "register listen event: " << ret->fd;
    break;
  case Event::CONNECT:
    ret = impl_->register_connect_event(fd, (const ConnectEventOptions &)eos);
    if (ret)
      LOG(INFO) << "register connect event: " << ret->fd;
    break;
  case Event::READ:
    LOG(INFO) << "register read event: " << fd;
    ret = impl_->register_read_event(fd, (const ReadEventOptions &)eos);
    break;
  case Event::WRITE: {
    LOG(INFO) << "register write event: " << fd;
    ret = impl_->register_write_event(fd, (const WriteEventOptions &)eos);
    break;
  }
  case Event::TIMEOUT:
    ret = impl_->register_timeout_event(fd, (const TimeoutEventOptions &)eos);
    if (ret) LOG(INFO) << "register timeout event: " << ret->fd;
    break;
  }
  if (ret) {
    fd = ret->fd;
    em_->add_event(fd, eos.e_type, ret);
    return fd;
  } else {
    LOG(ERROR) << "register event failed: " << strerror(errno);
  }
  return -1;
}

int Reactor::unregister_event(int fd, Event e) {
  LOG(INFO) << "unregister e: " << int(e) << " for fd: " << fd;
  EventCtx *ctx = em_->remove_event(fd, e);
  if (ctx) delete ctx;
  return 0;
}

} // namespace reactor
