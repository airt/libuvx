#include "loop.h"

#include <uv.h>
#include "coroutine.h"

namespace co {

namespace {

void prepare_callback(uv_prepare_t *handle) {
  co::dispatch();
  if (co::finished()) {
    uv_prepare_stop(handle);
  }
}

} // end anonymous namespace

int run() {
  uv_prepare_t prepare_handle;
  uv_prepare_init(uv_default_loop(), &prepare_handle);
  uv_prepare_start(&prepare_handle, prepare_callback);
  return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

} // end namespace co
