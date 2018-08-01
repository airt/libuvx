#ifndef UVX_UTILITIES_H
#define UVX_UTILITIES_H

#include "coroutine.h"
#include <memory>
#include <uv.h>

#define HANDLE_INIT(handle_type)                                               \
  uv_##handle_type##_t handle;                                                 \
  internal::handle_data_t data;                                                \
  handle.data = &data;

namespace co {

using buf_t = uv_buf_t;

using buf_ptr = std::unique_ptr<buf_t, std::function<void(buf_t *)>>;

buf_ptr make_buf(size_t len);

namespace internal {

struct handle_data_t {
  coroutine_id_t coroutine_id;
  handle_data_t() : coroutine_id(running_coroutine_id()) {}
};

void handle_callback(uv_handle_t *handle);

} // end namespace internal

} // end namespace co

#endif // UVX_UTILITIES_H
