//
// Created by Elijah Sauder on 4/18/22.
//
//#include "catch/catch_test_macros.hpp"
#include <exception>
#include <catch2/catch_all.hpp>
#include "mmath/mmath.hpp"

template<typename T>
void
inline
matrix_constructor_test(size_t rows, size_t cols, bool is_empty, bool is_identity, bool is_square, bool use_entry_value,
                        T entry_value) {
    mmath::matrix_base<T> matrix = use_entry_value ? mmath::matrix_base<T>{rows, cols, entry_value}
                                                   : mmath::matrix_base<T>{rows, cols};
    INFO("Matrix Size: " << rows << "x" << cols << ", use_entry_value: " << use_entry_value << ", entry_value: "
                         << (int) entry_value);
    CHECK(matrix.size_row() == rows);
    CHECK(matrix.size_col() == cols);
    CHECK(matrix.is_empty() == is_empty);
    CHECK(matrix.is_identity() == is_identity);
    CHECK(matrix.is_square() == is_square);

    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            CHECK(matrix(i, j) == entry_value);
        }
    }
}

TEMPLATE_TEST_CASE("Matrix Creation", "[matrix_base][template]", signed char, unsigned char, wchar_t, char8_t,
                   char16_t, char32_t, signed short int, unsigned short int, signed int, unsigned int, signed long int,
                   unsigned long int, signed long long int, unsigned long long int, float, double, long double) {
    SECTION("Default Initialization") {
        mmath::matrix_base<TestType> default_init_int{};

        CHECK(default_init_int.size_col() == 0);
        CHECK(default_init_int.size_row() == 0);
        CHECK(default_init_int.is_square());
        CHECK(default_init_int.is_identity()); // Vacuously true
        CHECK(default_init_int.is_empty());
    }

    SECTION("Empty Matrices") {
        mmath::matrix_base<TestType> matrix{{},
                                            {},
                                            {}};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{{1},
                                              {},
                                              {}};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{{}};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{{1}};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{0,
                                              0};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{1, 1};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{0, 3};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 3);

        matrix = mmath::matrix_base<TestType>{1, 3};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 3);

        matrix = mmath::matrix_base<TestType>{3, 0};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{3, 1};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 1);
    }

    SECTION("Checkout Out of Bounds Access") {
        mmath::matrix_base<int> matrix{3,
                                       3};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK_THROWS_AS(matrix(0, 0), std::out_of_range);
        CHECK_THROWS_AS(matrix(0, 1), std::out_of_range);
        CHECK_THROWS_AS(matrix(1, 0), std::out_of_range);
        CHECK_THROWS_AS(matrix(4, 4), std::out_of_range);
        CHECK_THROWS_AS(matrix(1, 4), std::out_of_range);
        CHECK_THROWS_AS(matrix(4, 1), std::out_of_range);
        CHECK_NOTHROW(matrix(1, 1));
        CHECK_NOTHROW(matrix(1, 2));
        CHECK_NOTHROW(matrix(1, 3));
        CHECK_NOTHROW(matrix(2, 1));
        CHECK_NOTHROW(matrix(2, 2));
        CHECK_NOTHROW(matrix(2, 3));
        CHECK_NOTHROW(matrix(3, 1));
        CHECK_NOTHROW(matrix(3, 2));
        CHECK_NOTHROW(matrix(3, 3));
    }

    SECTION("Initializer List Validation - 3x3") {
        mmath::matrix_base<int> matrix{{5, 6, 7},
                                       {1, 3, 2},
                                       {7, 8, 0}};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.is_square());
        CHECK_FALSE(matrix.is_identity());
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix(1, 1) == 5);
        CHECK(matrix(1, 2) == 6);
        CHECK(matrix(1, 3) == 7);
        CHECK(matrix(2, 1) == 1);
        CHECK(matrix(2, 2) == 3);
        CHECK(matrix(2, 3) == 2);
        CHECK(matrix(3, 1) == 7);
        CHECK(matrix(3, 2) == 8);
        CHECK(matrix(3, 3) == 0);
    }

    SECTION("Initializer List Validation - 3x3 Identity") {
        mmath::matrix_base<TestType> matrix{{1, 0, 0},
                                            {0, 1, 0},
                                            {0, 0, 1}};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.is_square());
        CHECK(matrix.is_identity());
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix(1, 1) == 1);
        CHECK(matrix(1, 2) == 0);
        CHECK(matrix(1, 3) == 0);
        CHECK(matrix(2, 1) == 0);
        CHECK(matrix(2, 2) == 1);
        CHECK(matrix(2, 3) == 0);
        CHECK(matrix(3, 1) == 0);
        CHECK(matrix(3, 2) == 0);
        CHECK(matrix(3, 3) == 1);
    }

    SECTION("Initializer List Validation - 3x2 !Identity") {
        mmath::matrix_base<TestType> matrix{{1, 0},
                                            {0, 1},
                                            {0, 0}};
        CHECK(matrix.size_col() == 2);
        CHECK(matrix.size_row() == 3);
        CHECK_FALSE(matrix.is_square());
        CHECK_FALSE(matrix.is_identity());
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix(1, 1) == 1);
        CHECK(matrix(1, 2) == 0);
        CHECK(matrix(2, 1) == 0);
        CHECK(matrix(2, 2) == 1);
        CHECK(matrix(3, 1) == 0);
        CHECK(matrix(3, 2) == 0);
    }

    SECTION("Initializer List Validation - 2x3 !Identity") {
        mmath::matrix_base<TestType> matrix{{1, 0, 0},
                                            {0, 1, 0}};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 2);
        CHECK_FALSE(matrix.is_square());
        CHECK_FALSE(matrix.is_identity());
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix(1, 1) == 1);
        CHECK(matrix(1, 2) == 0);
        CHECK(matrix(1, 3) == 0);
        CHECK(matrix(2, 1) == 0);
        CHECK(matrix(2, 2) == 1);
        CHECK(matrix(2, 3) == 0);
    }

    SECTION("Matrix Constructors") {

        matrix_constructor_test<TestType>(0, 0, true, true, true, false, 0);
        matrix_constructor_test<TestType>(1, 1, false, false, true, false, 0);
        matrix_constructor_test<TestType>(1, 10, false, false, false, false, 0);
        matrix_constructor_test<TestType>(10, 1, false, false, false, false, 0);
        matrix_constructor_test<TestType>(10, 10, false, false, true, false, 0);

        matrix_constructor_test<TestType>(0, 0, true, true, true, true, 1);
        matrix_constructor_test<TestType>(1, 1, false, true, true, true, 1);
        matrix_constructor_test<TestType>(1, 10, false, false, false, true, 1);
        matrix_constructor_test<TestType>(10, 1, false, false, false, true, 1);
        matrix_constructor_test<TestType>(10, 10, false, false, true, true, 1);

    }
}
