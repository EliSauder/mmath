cmake_minimum_required(VERSION 3.21)
project(mmath LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_library(mmath INTERFACE include/mmath/mmath.hpp)
target_include_directories(mmath INTERFACE include/)

find_package(Catch2 REQUIRED)

add_executable(mmath_test test/mmath/mmath_test.cpp)
target_link_libraries(mmath_test PRIVATE mmath)
target_link_libraries(mmath_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(mmath_test)
