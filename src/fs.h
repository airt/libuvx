#ifndef XUV_FS_H
#define XUV_FS_H

#include <uv.h>
#include "utilities.h"

namespace co {

namespace fs {

using file_t = uv_file;

using stat_t = uv_stat_t;

file_t open(const char *path, int flags, int mode = 0);

int close(file_t file);

int read(file_t file, const buf_t bufs[], uint nbufs, int64_t offset);

int write(file_t file, const buf_t bufs[], uint nbufs, int64_t offset);

int stat(const char *path, stat_t *stat);

int lstat(const char *path, stat_t *stat);

int fstat(file_t file, stat_t *stat);

} // end namespace fs

} // end namespace co

#endif // XUV_FS_H
