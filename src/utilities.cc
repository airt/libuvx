#include "utilities.h"

namespace co {

buf_ptr make_buf(size_t len) {
  auto ptr = new uv_buf_t{new char[len], len};
  auto dtr = [](buf_t *buf) {
    delete[] buf->base;
    delete buf;
  };
  return {ptr, dtr};
}

namespace internal {

void handle_callback(uv_handle_t *handle) {
  auto *data = reinterpret_cast<handle_data_t *>(handle->data);
  notify(data->coroutine_id);
}

} // end namespace internal

} // end namespace co
