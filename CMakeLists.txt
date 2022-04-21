cmake_minimum_required(VERSION 3.21)
project(mmath LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_library(mmath STATIC include/mmath/mmath.hpp include/mmath/mmath_concepts.hpp src/mmath/mmath.cpp include/mmath/mmath_matrixelementary.hpp)
target_include_directories(mmath PUBLIC include/)

find_package(Catch2 3 REQUIRED)

add_executable(mmath_tests test/mmath/mmath_test.cpp)
target_link_libraries(mmath_tests PRIVATE mmath)
target_link_libraries(mmath_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(mmath_tests)
