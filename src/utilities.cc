#include "utilities.h"

namespace co {

unique_buf_t make_unique_buf(size_t len) {
  return {
    new uv_buf_t{new char[len], len},
    [](buf_t *buf) {
      delete[] buf->base;
      delete buf;
    }
  };
}

namespace internal {

void handle_callback(uv_handle_t *handle) {
  auto *data = reinterpret_cast<handle_data_t *>(handle->data);
  notify(data->coroutine_id);
}

} // end namespace internal

} // end namespace co
