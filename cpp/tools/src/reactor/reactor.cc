#include "reactor.h"
#include "reactor_impl.h"
#include <glog/logging.h>

namespace reactor {

Reactor::Reactor(ReactorImpl *impl) : impl_(impl), em_(new EventMap{}) {}

Reactor::~Reactor() {
  delete em_;
  delete impl_;
}

int Reactor::runSync() { return impl_->runSync(); }

int Reactor::runAsync() {}

int Reactor::stop() { return impl_->stop(); }

int Reactor::register_event(int fd, const EventOptions &eos) {
  EventCtx *ret = nullptr;
  switch (eos.e_type) {
  case Event::LISTEN:
    ret = impl_->register_listen_event(fd, (const ListenEventOptions &)eos);
    break;
  case Event::CONNECT:
    ret = impl_->register_connect_event(fd, (const ConnectEventOptions &)eos);
    break;
  case Event::READ:
    ret = impl_->register_read_event(fd, (const ReadEventOptions &)eos);
    break;
  case Event::WRITE: {
    ret = impl_->register_write_event(fd, (const WriteEventOptions &)eos);
    break;
  }
  case Event::TIMEOUT:
    // ret = impl_->register_timeout_event(fd, (const TimeoutEventOptions
    // *)eos);
    break;
  }
  if (ret) {
    fd = ret->fd;
    em_->add_event(fd, eos.e_type, ret);
    return fd;
  }
  return -1;
}

int Reactor::unregister_event(int fd, Event e) {
  EventCtx *ctx = em_->remove_event(fd, e);
  if (ctx) delete ctx;
  return 0;
}

} // namespace reactor
