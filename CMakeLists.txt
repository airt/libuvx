cmake_minimum_required(VERSION 3.0)

project(xuv)

set(CMAKE_CXX_STANDARD 17)

# libraries

add_subdirectory(libs/libuv)

include_directories(libs/libuv/include)

include_directories(include)

set(
  SOURCES
  include/xuv
  src/lib.h
  src/lib.cc
)

add_library(xuv ${SOURCES})

# executables

add_executable(xuv-exec examples/main.cc)

target_link_libraries(xuv-exec xuv uv)

# tests

enable_testing()

add_subdirectory(libs/googletest/googletest)

set(
  TESTS
  tests/lib-test.cc
)

add_executable(run-tests ${TESTS})

target_include_directories(run-tests PRIVATE src)

target_link_libraries(run-tests xuv uv gtest gtest_main)

add_test(tests run-tests)
