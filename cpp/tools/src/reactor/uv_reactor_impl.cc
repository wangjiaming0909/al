#include "uv_reactor_impl.h"
#include <glog/logging.h>

namespace reactor {
UVReactorImpl::UVReactorImpl() : loop_(nullptr) {
  loop_ = (uv_loop_t *)calloc(1, sizeof(uv_loop_t));
  uv_loop_init(loop_);
}

UVReactorImpl::~UVReactorImpl() {
  uv_loop_close(loop_);
  free(loop_);
}

int UVReactorImpl::runSync() {
  int ret;
  while (1) {
    ret = uv_run(loop_, UV_RUN_DEFAULT);
    if (ret) break;
    std::unique_lock<std::mutex> lock{alive_lock_};
    while (!uv_loop_alive(loop_)) {
      alive_cond_.wait(lock);
    }
  }
  return ret;
}

TimeoutEventCtx* UVReactorImpl::register_timeout_event(int, const TimeoutEventOptions& teos) {
  auto ctx = std::make_unique<TimeoutEventCtx>();
  if (!ctx)
    return nullptr;

  auto te_cb = [](uv_timer_t* timer) {
    auto *c = (TimeoutEventCtx *)timer->data;
    auto handler = c->eos->handler.lock();
    if (handler) {
      handler->handle_timeout();
    }
  };

  auto *et = new uv_timer_t();
  if (!et)
    return nullptr;
  uv_timer_init(loop_, et);
  et->data = ctx.get();
  get_usecs(teos.timeout);
  uv_timer_start(et, te_cb, get_millisecs(teos.timeout), 1);
  if (0 != uv_timer_start(et, te_cb, get_millisecs(teos.timeout), 1)) {
    LOG(ERROR) << "add timer event failed: " << strerror(errno);
    return nullptr;
  }

  ctx->ec = et;
  *ctx->eos = teos;
  ctx->fd = -1;
  ctx->ec_deleter = [](void *ec) {
    if (ec) {
      auto *et = (uv_timer_t *)ec;
      uv_timer_stop(et);
      delete et;
    }
  };

  return ctx.release();
}
}
