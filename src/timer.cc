#include "timer.h"
#include "coroutine.h"
#include "utilities.h"
#include <uv.h>

namespace co {

auto callback = reinterpret_cast<uv_timer_cb>(internal::handle_callback);

void sleep(uint64_t timeout) {
  HANDLE_INIT(timer);
  uv_timer_init(uv_default_loop(), &handle);
  uv_timer_start(&handle, callback, timeout, 0);
  wait();
  uv_timer_stop(&handle);
}

} // end namespace co
