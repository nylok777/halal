cmake_minimum_required(VERSION 4.0)
project(halal_tests)
set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(lib/googletest-main)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(..)
include_directories(../utils)
include_directories(../interfaces-and-templates)
include_directories(../solvers)
include_directories(../problems)

FetchContent_MakeAvailable(googletest)
add_executable(halal_tests_run dynamic_matrix_test.cpp
        genetic_jobshop_test.cpp
        ../problems/JobShopProblem.cpp
        ../problems/TravelingSalesmanProblem.cpp
        traveling_salesman_test.cpp
)
target_link_libraries(halal_tests_run gtest gtest_main)