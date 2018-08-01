#include <cstring>
#include <uvx>

void cat(const char *path) {

  auto check = [=](int code) {
    if (code < 0) {
      fprintf(stderr, "cat: %s: %s\n", path, uv_strerror(code));
      exit(code);
    }
  };

  co::create([=] {
    const co::fs::file_t STDIN = 0;

    auto file = strcmp(path, "-") == 0 ? STDIN : co::fs::open(path, O_RDONLY);
    check(file);

    auto buf = co::make_buf(256);
    int64_t offset = 0;

    while (true) {
      auto len = co::fs::read(file, &*buf, 1, file == STDIN ? -1 : offset);
      check(len);
      if (len == 0) {
        break;
      }

      buf->len = len;
      co::fs::write(1, &*buf, 1, -1);

      offset += len;
    }

    if (file != STDIN) {
      co::fs::close(file);
    }
  });

  co::run();
}

int main(int argc, char **argv) {

  if (argc < 2) {
    cat("-");
  }

  for (int i = 1; i < argc; i++) {
    cat(argv[i]);
  }
}
