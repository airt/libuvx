# xuv

[![Build Status][build-badge]][build-status]

coroutine + libuv

## Code Example

### Basic

```cxx
#include <xuv>

int main() {

  auto c = co::create([] {
    puts("2");
    co::yield();
    puts("4");
  });

  puts("1");
  co::resume(c);
  puts("3");
  co::resume(c);
  puts("5");

}
```

### More

- [cat](examples/cat.cc)

## Build and Test

```bash
git submodule update --init --recursive
```

run scripts

```bash
scripts/build.sh
scripts/test.sh
```

or run within container

```bash
docker build -t xuv-test .
docker run -it xuv-test
```

[build-badge]: https://img.shields.io/travis/airt/xuv.svg
[build-status]: https://travis-ci.org/airt/xuv
