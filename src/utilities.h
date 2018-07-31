#ifndef XUV_UTILITIES_H
#define XUV_UTILITIES_H

#include <uv.h>
#include <memory>
#include "coroutine.h"

#define HANDLE_INIT(handle_type)                                              \
  uv_##handle_type##_t handle;                                                \
  internal::handle_data_t data;                                               \
  handle.data = &data;                                                        \

namespace co {

using buf_t = uv_buf_t;

using unique_buf_t = std::unique_ptr<buf_t, std::function<void(buf_t *)>>;

unique_buf_t make_unique_buf(size_t len);

namespace internal {

struct handle_data_t {
  coroutine_id_t coroutine_id;
  handle_data_t() : coroutine_id(running_coroutine()) {}
};

void handle_callback(uv_handle_t *handle);

} // end namespace internal

} // end namespace co

#endif // XUV_UTILITIES_H
