cmake_minimum_required(VERSION 3.0)

project(uvx)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libraries

add_subdirectory(vendor/libuv)

include_directories(vendor/libuv/include)

include_directories(include)

set(
  SOURCES
  include/uvx
  src/lib.h
  src/lib.cc
  src/fs.h
  src/fs.cc
  src/loop.h
  src/loop.cc
  src/timer.h
  src/timer.cc
  src/context.h
  src/context.cc
  src/coroutine.h
  src/coroutine.cc
  src/utilities.h
  src/utilities.cc
)

add_library(uvx ${SOURCES})

# executables

add_executable(uvx-exec examples/main.cc)

target_link_libraries(uvx-exec uvx uv)

add_executable(cat examples/cat.cc)

target_link_libraries(cat uvx uv)

# tests

enable_testing()

add_subdirectory(vendor/googletest)

set(
  TESTS
  tests/lib-test.cc
  tests/fs-test.cc
  tests/timer-test.cc
  tests/coroutine-test.cc
)

add_executable(run-tests ${TESTS})

target_include_directories(run-tests PRIVATE src)

target_link_libraries(run-tests uvx uv gtest gtest_main)

add_test(tests run-tests)
