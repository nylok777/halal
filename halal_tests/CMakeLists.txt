cmake_minimum_required(VERSION 4.0)
project(halal_tests)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest-main)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(..)

add_executable(halal_tests_run dynamic_matrix_test.cpp)
target_link_libraries(halal_tests_run gtest gtest_main)