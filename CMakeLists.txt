cmake_minimum_required(VERSION 3.0)

project(xuv)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libraries

add_subdirectory(libs/libuv)

include_directories(libs/libuv/include)

include_directories(include)

set(
  SOURCES
  include/xuv
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

add_library(xuv ${SOURCES})

# executables

add_executable(xuv-exec examples/main.cc)

target_link_libraries(xuv-exec xuv uv)

add_executable(cat examples/cat.cc)

target_link_libraries(cat xuv uv)

# tests

enable_testing()

add_subdirectory(libs/googletest/googletest)

set(
  TESTS
  tests/lib-test.cc
  tests/fs-test.cc
  tests/timer-test.cc
  tests/coroutine-test.cc
)

add_executable(run-tests ${TESTS})

target_include_directories(run-tests PRIVATE src)

target_link_libraries(run-tests xuv uv gtest gtest_main)

add_test(tests run-tests)
