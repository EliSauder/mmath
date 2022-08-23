cmake_minimum_required(VERSION 3.24)
project(mmath LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_library(mmath INTERFACE src/mmath/mmath.hpp src/mmath/mmath_concepts.hpp src/mmath/mmath_matrixelementary.hpp)
target_include_directories(mmath INTERFACE include/)

option(MMATH_FETCHCONTENT_CATCH2 "Whether or not to use fetchcontent instead of find_package" OFF)

if (MMATH_FETCHCONTENT_CATCH2)
    Include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.1.0 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)
else()
    find_package(Catch2 3 REQUIRED)
endif()

add_executable(mmath_tests test/mmath/mmath_test.cpp)
target_link_libraries(mmath_tests PRIVATE mmath)
target_link_libraries(mmath_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(mmath_tests)
