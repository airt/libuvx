#include "fs.h"

#include <uv.h>
#include "coroutine.h"
#include "utilities.h"

#define POST                                                                  \
  do {                                                                        \
    uv_fs_req_cleanup(&handle);                                               \
    return static_cast<int>(handle.result);                                   \
  } while (0)                                                                 \

namespace co {

namespace fs {

auto callback = reinterpret_cast<uv_fs_cb>(internal::handle_callback);

file_t open(const char *path, int flags, int mode) {
  HANDLE_INIT(fs);
  uv_fs_open(uv_default_loop(), &handle, path, flags, mode, callback);
  wait();
  POST;
}

int close(file_t file) {
  HANDLE_INIT(fs);
  uv_fs_close(uv_default_loop(), &handle, file, callback);
  wait();
  POST;
}

int read(file_t file, const buf_t bufs[], uint nbufs, int64_t offset) {
  HANDLE_INIT(fs);
  uv_fs_read(uv_default_loop(), &handle, file, bufs, nbufs, offset, callback);
  wait();
  POST;
}

int write(file_t file, const buf_t bufs[], uint nbufs, int64_t offset) {
  HANDLE_INIT(fs);
  uv_fs_write(uv_default_loop(), &handle, file, bufs, nbufs, offset, callback);
  wait();
  POST;
}

int stat(const char *path, stat_t *stat) {
  HANDLE_INIT(fs);
  uv_fs_stat(uv_default_loop(), &handle, path, callback);
  wait();
  *stat = handle.statbuf;
  POST;
}

int lstat(const char *path, stat_t *stat) {
  HANDLE_INIT(fs);
  uv_fs_lstat(uv_default_loop(), &handle, path, callback);
  wait();
  *stat = handle.statbuf;
  POST;
}

int fstat(file_t file, stat_t *stat) {
  HANDLE_INIT(fs);
  uv_fs_fstat(uv_default_loop(), &handle, file, callback);
  wait();
  *stat = handle.statbuf;
  POST;
}

} // end namespace fs

} // end namespace co
