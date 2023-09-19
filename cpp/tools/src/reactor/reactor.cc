#include "reactor.h"
#include "reactor_impl.h"
#include <glog/logging.h>

namespace reactor {

Reactor::Reactor(ReactorImpl* impl) :impl_(impl), em_(new EventMap{}) { }

Reactor::~Reactor() {
  delete em_;
  delete impl_;
}

int Reactor::runSync() {
  return impl_->runSync();
}

int Reactor::runAsync() {
}
int Reactor::stop() {
  return impl_->stop();
}
int Reactor::register_event(int fd, const EventOptions *eos) {
  EventCtx* ret = nullptr;
  switch (eos->e_type) {
  case Event::LISTEN: {
    ret = impl_->register_listen_event(fd, (const ListenEventOptions*)eos);
    break;
  }
  case Event::CONNECT: {
    ret = impl_->register_connect_event(fd, (const ConnectEventOptions*)eos);
    break;
  }
  case Event::READ:
  case Event::WRITE: {
    break;
  }
  case Event::TIMEOUT: {
  }
  }
  if (ret) {
    fd = ret->fd;
    em_->add_event(fd, eos->e_type, ret);
    return 0;
  }
  return -1;
}

int Reactor::unregister_event(int fd, Event e) {
  int ret;
  EventCtx* ctx = em_->get_ctx(fd, e);
  if (!ctx) return -1;
  switch (e) {
    case Event::LISTEN:
      ret = impl_->unregister_listen_event(fd, (ListenEventCtx*)ctx);
      break;
    default:
      break;
  }
  return ret;
}

} // namespace reactor
