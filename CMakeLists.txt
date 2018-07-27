cmake_minimum_required(VERSION 3.12)

project(xuv)

set(CMAKE_CXX_STANDARD 17)

# libraries

add_subdirectory(libs/libuv)

include_directories(libs/libuv/include)

add_library(
  xuv
  src/lib.h
  src/lib.cc
)

# executables

add_executable(xuv-exec examples/main.cc)

target_link_libraries(xuv-exec xuv uv)

# unit-tests

enable_testing()

add_subdirectory(libs/googletest/googletest)

add_executable(
  run-unit-tests
  tests/lib-test.cc
)

target_link_libraries(run-unit-tests xuv uv gtest gtest_main)

add_test(unit-tests run-unit-tests)
