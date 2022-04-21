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
    mmath::matrix_base<T> matrix = use_entry_value ? mmath::matrix_base<T>{ rows, cols, entry_value }
                                                   : mmath::matrix_base<T>{ rows, cols };
    INFO("Matrix Size: " << rows << "x" << cols << ", use_entry_value: " << use_entry_value << ", entry_value: "
                         << (int)entry_value);
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

TEMPLATE_TEST_CASE("Matrix Creation", "[matrix_base][template]", signed char, // NOLINT(cert-err58-cpp)
                   unsigned char, wchar_t, char8_t, char16_t, char32_t, signed short int, unsigned short int,
                   signed int, unsigned int, signed long int, unsigned long int, signed long long int,
                   unsigned long long int, float, double, long double) {
    SECTION("Default Initialization") {
        mmath::matrix_base<TestType> default_init_int{};

        CHECK(default_init_int.size_col() == 0);
        CHECK(default_init_int.size_row() == 0);
        CHECK(default_init_int.is_square());
        CHECK(default_init_int.is_identity()); // Vacuously true
        INFO(".is_identity() should be vacuously true");
        CHECK(default_init_int.is_zero());
        INFO(".is_zero() should be vacuously true");
        CHECK(default_init_int.is_empty());
    }

    SECTION("Empty Matrices") {
        mmath::matrix_base<TestType> matrix{{},
                                            {},
                                            {}};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{{ 1 },
                                              {},
                                              {}};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{{}};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{{ 1 }};
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{};
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{ 0,
                                               0 };
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{ 1, 1 };
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 1);

        matrix = mmath::matrix_base<TestType>{ 0, 3 };
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 0);
        CHECK(matrix.size_col() == 3);

        matrix = mmath::matrix_base<TestType>{ 1, 3 };
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 1);
        CHECK(matrix.size_col() == 3);

        matrix = mmath::matrix_base<TestType>{ 3, 0 };
        CHECK(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 0);

        matrix = mmath::matrix_base<TestType>{ 3, 1 };
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.size_col() == 1);
    }

    SECTION("Checkout Out of Bounds Access") {
        mmath::matrix_base<int> matrix{ 3,
                                        3 };
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

    SECTION("Zero Matrix") {
        mmath::matrix_base<TestType> matrix_a{ 1, 3 };
        CHECK(matrix_a.is_zero());
        CHECK_FALSE(matrix_a.is_empty());
        CHECK_FALSE(matrix_a.is_identity());
        CHECK(matrix_a(1, 1) == TestType{});
        CHECK(matrix_a(1, 2) == TestType{});
        CHECK(matrix_a(1, 3) == TestType{});

        mmath::matrix_base<TestType> matrix_b{ 3, 3 };
        CHECK(matrix_b.is_zero());
        CHECK_FALSE(matrix_b.is_empty());
        CHECK_FALSE(matrix_b.is_identity());
        CHECK(matrix_b(1, 1) == TestType{});
        CHECK(matrix_b(1, 2) == TestType{});
        CHECK(matrix_b(1, 3) == TestType{});
        CHECK(matrix_b(2, 1) == TestType{});
        CHECK(matrix_b(2, 2) == TestType{});
        CHECK(matrix_b(2, 3) == TestType{});
        CHECK(matrix_b(3, 1) == TestType{});
        CHECK(matrix_b(3, 2) == TestType{});
        CHECK(matrix_b(3, 3) == TestType{});

        mmath::matrix_base<TestType> matrix_c{{ 1 },
                                              { 0, 1 },
                                              { 0, 0, 1 }};
        CHECK_FALSE(matrix_c.is_zero());
        CHECK_FALSE(matrix_c.is_empty());
        CHECK(matrix_c.is_identity());
        //CHECK(matrix_b(1, 1) == TestType{});
        CHECK(matrix_c(1, 2) == TestType{});
        CHECK(matrix_c(1, 3) == TestType{});
        CHECK(matrix_c(2, 1) == TestType{});
        //CHECK(matrix_b(2, 2) == TestType{});
        CHECK(matrix_c(2, 3) == TestType{});
        CHECK(matrix_c(3, 1) == TestType{});
        CHECK(matrix_c(3, 2) == TestType{});
        //CHECK(matrix_b(3, 3) == TestType{});
    }

    SECTION("Initializer List Validation - 3x3") {
        mmath::matrix_base<int> matrix{{ 5, 6, 7 },
                                       { 1, 3, 2 },
                                       { 7, 8, 0 }};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.is_square());
        CHECK_FALSE(matrix.is_zero());
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

    SECTION("Initializer List Validation - 3x3 Missing Columns") {
        mmath::matrix_base<int> matrix{{ 5, 6, 7 },
                                       { 1, 3 },
                                       { 7, 8 }};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.is_square());
        CHECK_FALSE(matrix.is_zero());
        CHECK_FALSE(matrix.is_identity());
        CHECK_FALSE(matrix.is_empty());
        CHECK(matrix(1, 1) == 5);
        CHECK(matrix(1, 2) == 6);
        CHECK(matrix(1, 3) == 7);
        CHECK(matrix(2, 1) == 1);
        CHECK(matrix(2, 2) == 3);
        CHECK(matrix(2, 3) == 0);
        CHECK(matrix(3, 1) == 7);
        CHECK(matrix(3, 2) == 8);
        CHECK(matrix(3, 3) == 0);
    }

    SECTION("Initializer List Validation - 3x3 Identity") {
        mmath::matrix_base<TestType> matrix{{ 1, 0, 0 },
                                            { 0, 1, 0 },
                                            { 0, 0, 1 }};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 3);
        CHECK(matrix.is_square());
        CHECK(matrix.is_identity());
        CHECK_FALSE(matrix.is_zero());
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
        mmath::matrix_base<TestType> matrix{{ 1, 0 },
                                            { 0, 1 },
                                            { 0, 0 }};
        CHECK(matrix.size_col() == 2);
        CHECK(matrix.size_row() == 3);
        CHECK_FALSE(matrix.is_zero());
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
        mmath::matrix_base<TestType> matrix{{ 1, 0, 0 },
                                            { 0, 1, 0 }};
        CHECK(matrix.size_col() == 3);
        CHECK(matrix.size_row() == 2);
        CHECK_FALSE(matrix.is_zero());
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

TEST_CASE("Matrix Comparison") {
    SECTION("Equality - int") {
        mmath::matrix_base<int> matrix_a{};
        mmath::matrix_base<int> matrix_b{};
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 0, 0 };
        matrix_b = mmath::matrix_base<int>{ 0, 0 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 0, 3 };
        matrix_b = mmath::matrix_base<int>{ 0, 3 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 3, 0 };
        matrix_b = mmath::matrix_base<int>{ 3, 0 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 3, 3 };
        matrix_b = mmath::matrix_base<int>{ 3, 3 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 4, 3 };
        matrix_b = mmath::matrix_base<int>{ 4, 3 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{ 3, 4 };
        matrix_b = mmath::matrix_base<int>{ 3, 4 };
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{{ 0, 0, 0 },
                                           { 0, 0, 0 }};
        matrix_b = mmath::matrix_base<int>{{ 0, 0, 0 },
                                           { 0, 0, 0 }};
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{{ 1, 0, 0 },
                                           { 0, 1, 0 }};
        matrix_b = mmath::matrix_base<int>{{ 1, 0, 0 },
                                           { 0, 1, 0 }};
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);

        matrix_a = mmath::matrix_base<int>{{ 5, 6, 7 },
                                           { 1, 3, 2 },
                                           { 7, 8, 0 }};
        matrix_b = mmath::matrix_base<int>{{ 5, 6, 7 },
                                           { 1, 3, 2 },
                                           { 7, 8, 0 }};
        CHECK(matrix_a == matrix_b);
        CHECK_FALSE(matrix_a != matrix_b);
    }
}

TEST_CASE("Matrix Addition") {

    SECTION("Empty Matrix") {
        mmath::matrix_base<int> matrix_a{};
        mmath::matrix_base<int> matrix_b{};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{};
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 0, 10 };
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 10, 7 };
        matrix_b = mmath::matrix_base<int>{};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::invalid_argument);
    }

    SECTION("Different Sizes") {
        mmath::matrix_base<int> matrix_a{{ 1 },
                                         { 2 },
                                         { 3 }};
        mmath::matrix_base<int> matrix_b{{ 1 },
                                         { 2 }};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::domain_error);

        matrix_a = mmath::matrix_base<int>{{ 1, 2, 3 },
                                           { 3, 5, 3 }};
        matrix_b = mmath::matrix_base<int>{{ 1, 2 },
                                           { 3, 5 }};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::domain_error);

        matrix_a = mmath::matrix_base<int>{{ 1, 2, 3 },
                                           { 3, 5, 3 }};
        matrix_b = mmath::matrix_base<int>{{ 1, 2 }};
        CHECK_THROWS_AS(matrix_a + matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b + matrix_a, std::domain_error);
    }

    SECTION("Addition Result Validation") {
        mmath::matrix_base<int> matrix_int_a{{ 1, 2 },
                                             { 3, 4 }};
        mmath::matrix_base<int> matrix_int_b{{ 1, -2 },
                                             { 3, 4 }};

        REQUIRE_NOTHROW(matrix_int_a + matrix_int_b);
        REQUIRE_NOTHROW(matrix_int_b + matrix_int_a);

        auto result_int1 = matrix_int_a + matrix_int_b;
        auto result_int2 = matrix_int_b + matrix_int_a;

        CHECK(result_int1(1, 1) == 2);
        CHECK(result_int1(1, 2) == 0);
        CHECK(result_int1(2, 1) == 6);
        CHECK(result_int1(2, 2) == 8);

        CHECK(result_int2(1, 1) == 2);
        CHECK(result_int2(1, 2) == 0);
        CHECK(result_int2(2, 1) == 6);
        CHECK(result_int2(2, 2) == 8);

        using namespace Catch::literals;

        mmath::matrix_base<double> matrix_double_a{{ 1.15,   -2.25 },
                                                   { 3.1415, 4.999 }};
        mmath::matrix_base<double> matrix_double_b{{ 1.863,   2.75 },
                                                   { 3.66666, 4.123 }};

        REQUIRE_NOTHROW(matrix_double_a + matrix_double_b);
        REQUIRE_NOTHROW(matrix_double_b + matrix_double_a);

        auto result_double1 = matrix_double_a + matrix_double_b;
        auto result_double2 = matrix_double_b + matrix_double_a;

        CHECK(result_double1(1, 1) == 3.013_a);
        CHECK(result_double1(1, 2) == 0.5_a);
        CHECK(result_double1(2, 1) == 6.80816_a);
        CHECK(result_double1(2, 2) == 9.122_a);

        CHECK(result_double2(1, 1) == 3.013_a);
        CHECK(result_double2(1, 2) == 0.5_a);
        CHECK(result_double2(2, 1) == 6.80816_a);
        CHECK(result_double2(2, 2) == 9.122_a);
    }
}

TEST_CASE("Matrix Subtraction") {

    SECTION("Empty Matrix") {
        mmath::matrix_base<int> matrix_a{};
        mmath::matrix_base<int> matrix_b{};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{};
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 0, 10 };
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 10, 7 };
        matrix_b = mmath::matrix_base<int>{};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::invalid_argument);
    }

    SECTION("Different Sizes") {
        mmath::matrix_base<int> matrix_a{{ 1 },
                                         { 2 },
                                         { 3 }};
        mmath::matrix_base<int> matrix_b{{ 1 },
                                         { 2 }};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::domain_error);

        matrix_a = mmath::matrix_base<int>{{ 1, 2, 3 },
                                           { 3, 5, 3 }};
        matrix_b = mmath::matrix_base<int>{{ 1, 2 },
                                           { 3, 5 }};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::domain_error);

        matrix_a = mmath::matrix_base<int>{{ 1, 2, 3 },
                                           { 3, 5, 3 }};
        matrix_b = mmath::matrix_base<int>{{ 1, 2 }};
        CHECK_THROWS_AS(matrix_a - matrix_b, std::domain_error);
        CHECK_THROWS_AS(matrix_b - matrix_a, std::domain_error);
    }

    SECTION("Subtraction Result Validation") {
        mmath::matrix_base<int> matrix_int_a{{ 1, 2 },
                                             { 3, 4 }};
        mmath::matrix_base<int> matrix_int_b{{ -1, 2 },
                                             { -3, -4 }};

        REQUIRE_NOTHROW(matrix_int_a - matrix_int_b);
        REQUIRE_NOTHROW(matrix_int_b - matrix_int_a);

        auto result_int1 = matrix_int_a - matrix_int_b;
        auto result_int2 = matrix_int_b - matrix_int_a;

        CHECK(result_int1(1, 1) == 2);
        CHECK(result_int1(1, 2) == 0);
        CHECK(result_int1(2, 1) == 6);
        CHECK(result_int1(2, 2) == 8);

        CHECK(result_int2(1, 1) == -2);
        CHECK(result_int2(1, 2) == -0);
        CHECK(result_int2(2, 1) == -6);
        CHECK(result_int2(2, 2) == -8);

        using namespace Catch::literals;

        mmath::matrix_base<double> matrix_double_a{{ 1.15,   -2.25 },
                                                   { 3.1415, 4.999 }};
        mmath::matrix_base<double> matrix_double_b{{ -1.863,   -2.75 },
                                                   { -3.66666, -4.123 }};

        REQUIRE_NOTHROW(matrix_double_a - matrix_double_b);
        REQUIRE_NOTHROW(matrix_double_b - matrix_double_a);

        auto result_double1 = matrix_double_a - matrix_double_b;
        auto result_double2 = matrix_double_b - matrix_double_a;

        CHECK(result_double1(1, 1) == 3.013_a);
        CHECK(result_double1(1, 2) == 0.5_a);
        CHECK(result_double1(2, 1) == 6.80816_a);
        CHECK(result_double1(2, 2) == 9.122_a);

        CHECK(result_double2(1, 1) == -3.013_a);
        CHECK(result_double2(1, 2) == -0.5_a);
        CHECK(result_double2(2, 1) == -6.80816_a);
        CHECK(result_double2(2, 2) == -9.122_a);
    }
}

template<typename T>
void compare_matrix_elements(const mmath::matrix_base<T>& a, const mmath::matrix_base<T>& b, size_t row, size_t col,
                             T value, bool approximate) {
    if (approximate) {
        CHECK(a(row, col) == b(row, col));
        CHECK(a(row, col) == Catch::Approx(value));
        return;
    }
    CHECK(a(row, col) == b(row, col));
    CHECK(a(row, col) == value);
}

template<typename T>
void compare_matrix_elements(const mmath::matrix_base<T>& a, const mmath::matrix_base<T>& b, size_t row, size_t col,
                             T value) {
    compare_matrix_elements<T>(a, b, row, col, value, false);
}

TEST_CASE("Matrix Multiplication") {

    SECTION("Empty Matrix") {
        mmath::matrix_base<int> matrix_a{};
        mmath::matrix_base<int> matrix_b{};
        CHECK_THROWS_AS(matrix_a * matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b * matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{};
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a * matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b * matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 0, 10 };
        matrix_b = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a * matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b * matrix_a, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 10, 7 };
        matrix_b = mmath::matrix_base<int>{};
        CHECK_THROWS_AS(matrix_a * matrix_b, std::invalid_argument);
        CHECK_THROWS_AS(matrix_b * matrix_a, std::invalid_argument);
    }

    SECTION("Multiplication Size Checks") {
        mmath::matrix_base<int> matrix_incorrect_a{{ 32, 1, 60, 2 }};
        mmath::matrix_base<int> matrix_incorrect_b{{ 32, 1 }};

        CHECK_THROWS_AS(matrix_incorrect_a * matrix_incorrect_b, std::domain_error);

        mmath::matrix_base<int> matrix_correct_a{{ 32, 1, 60, 2 }};
        mmath::matrix_base<int> matrix_correct_b{{ 32 },
                                                 { 1 },
                                                 { 60 },
                                                 { 2 }};

        CHECK_NOTHROW(matrix_correct_a * matrix_correct_b);
    }

    SECTION("Constant Multiplication") {
        mmath::matrix_base<int> matrix_int{{ 1,  2, 3 },
                                           { 4,  5, 6 },
                                           { -2, 1, 0 }};
        auto result_int_int1 = 2 * matrix_int;
        auto result_int_int2 = matrix_int * 2;
        auto result_int_double1 = 2.1 * matrix_int;
        auto result_int_double2 = matrix_int * 2.1;
        auto result_int_01 = 0 * matrix_int;
        auto result_int_02 = matrix_int * 0;

        compare_matrix_elements(result_int_int1, result_int_int2, 1, 1, 2);
        compare_matrix_elements(result_int_int1, result_int_int2, 1, 2, 4);
        compare_matrix_elements(result_int_int1, result_int_int2, 1, 3, 6);
        compare_matrix_elements(result_int_int1, result_int_int2, 2, 1, 8);
        compare_matrix_elements(result_int_int1, result_int_int2, 2, 2, 10);
        compare_matrix_elements(result_int_int1, result_int_int2, 2, 3, 12);
        compare_matrix_elements(result_int_int1, result_int_int2, 3, 1, -4);
        compare_matrix_elements(result_int_int1, result_int_int2, 3, 2, 2);
        compare_matrix_elements(result_int_int1, result_int_int2, 3, 3, 0);

        compare_matrix_elements(result_int_double1, result_int_double2, 1, 1, 2);
        compare_matrix_elements(result_int_double1, result_int_double2, 1, 2, 4);
        compare_matrix_elements(result_int_double1, result_int_double2, 1, 3, 6);
        compare_matrix_elements(result_int_double1, result_int_double2, 2, 1, 8);
        compare_matrix_elements(result_int_double1, result_int_double2, 2, 2, 10);
        compare_matrix_elements(result_int_double1, result_int_double2, 2, 3, 12);
        compare_matrix_elements(result_int_double1, result_int_double2, 3, 1, -4);
        compare_matrix_elements(result_int_double1, result_int_double2, 3, 2, 2);
        compare_matrix_elements(result_int_double1, result_int_double2, 3, 3, 0);

        compare_matrix_elements(result_int_01, result_int_02, 1, 1, 0);
        compare_matrix_elements(result_int_01, result_int_02, 1, 2, 0);
        compare_matrix_elements(result_int_01, result_int_02, 1, 3, 0);
        compare_matrix_elements(result_int_01, result_int_02, 2, 1, 0);
        compare_matrix_elements(result_int_01, result_int_02, 2, 2, 0);
        compare_matrix_elements(result_int_01, result_int_02, 2, 3, 0);
        compare_matrix_elements(result_int_01, result_int_02, 3, 1, 0);
        compare_matrix_elements(result_int_01, result_int_02, 3, 2, 0);
        compare_matrix_elements(result_int_01, result_int_02, 3, 3, 0);

        mmath::matrix_base<double> matrix_double{{ 11.14,  2,    3.1415 },
                                                 { 4.3333, 5.5,  6.6666 },
                                                 { -2.67,  1.53, 0.1 }};
        auto result_double_int1 = 2 * matrix_double;
        auto result_double_int2 = matrix_double * 2;
        auto result_double_double1 = 2.1 * matrix_double;
        auto result_double_double2 = matrix_double * 2.1;
        auto result_double_01 = 0 * matrix_double;
        auto result_double_02 = matrix_double * 0;

        compare_matrix_elements(result_double_int1, result_double_int2, 1, 1, 22.28, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 1, 2, 4., true);
        compare_matrix_elements(result_double_int1, result_double_int2, 1, 3, 6.2830, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 2, 1, 8.6666, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 2, 2, 11., true);
        compare_matrix_elements(result_double_int1, result_double_int2, 2, 3, 13.3332, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 3, 1, -5.34, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 3, 2, 3.06, true);
        compare_matrix_elements(result_double_int1, result_double_int2, 3, 3, 0.2, true);

        compare_matrix_elements(result_double_double1, result_double_double2, 1, 1, 23.394, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 1, 2, 4.2, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 1, 3, 6.59715, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 2, 1, 9.09993, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 2, 2, 11.55, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 2, 3, 13.99986, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 3, 1, -5.607, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 3, 2, 3.213, true);
        compare_matrix_elements(result_double_double1, result_double_double2, 3, 3, 0.21, true);

        compare_matrix_elements(result_double_01, result_double_02, 1, 1, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 1, 2, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 1, 3, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 2, 1, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 2, 2, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 2, 3, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 3, 1, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 3, 2, 0.);
        compare_matrix_elements(result_double_01, result_double_02, 3, 3, 0.);
    }

    SECTION("Multiplication Result Validation") {
        mmath::matrix_base<int> matrix_incorrect_a{{ 32, 1, 60, 2 }};
        mmath::matrix_base<int> matrix_incorrect_b{{ 32, 1 }};

        CHECK_THROWS_AS(matrix_incorrect_a * matrix_incorrect_b, std::domain_error);

        mmath::matrix_base<int> matrix_int_a{{ 32,  1,  60 },
                                             { 2,   -5, 3 },
                                             { -34, 21, 4 },
                                             { 43,  21, 19 }};
        mmath::matrix_base<int> matrix_int_b{{ -23, 67 },
                                             { 5,   3 },
                                             { 7,   129 }};

        REQUIRE_NOTHROW(matrix_int_a * matrix_int_b);
        CHECK_THROWS_AS(matrix_int_b * matrix_int_a, std::domain_error);

        auto result = matrix_int_a * matrix_int_b;

        CHECK(result.size_col() == 2);
        CHECK(result.size_row() == 4);
        CHECK_FALSE(result.is_zero());
        CHECK_FALSE(result.is_identity());
        CHECK_FALSE(result.is_square());
        CHECK_FALSE(result.is_empty());

        CHECK(result(1, 1) == -311);
        CHECK(result(1, 2) == 9887);
        CHECK(result(2, 1) == -50);
        CHECK(result(2, 2) == 506);
        CHECK(result(3, 1) == 915);
        CHECK(result(3, 2) == -1699);
        CHECK(result(4, 1) == -751);
        CHECK(result(4, 2) == 5395);
    }
}

TEST_CASE("Matrix Powers") {
    SECTION("Invalid Entries") {
        mmath::matrix_base<int> matrix_a{};
        CHECK_THROWS_AS(matrix_a ^ 2, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(matrix_a ^ 2, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 0, 10 };
        CHECK_THROWS_AS(matrix_a ^ 2, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 10, 7 };
        CHECK_THROWS_AS(matrix_a ^ 2, std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 10, 10 };
        CHECK_THROWS_AS(matrix_a ^ -1, std::invalid_argument);
    }

    SECTION("Zero Power") {
        mmath::matrix_base<int> matrix{{ 1, 2, 3 },
                                       { 3, 4, 5 },
                                       { 5, 6, 7 }};

        CHECK_NOTHROW(matrix ^ 0);

        auto result = matrix ^ 0;

        CHECK_FALSE(result.is_zero());
        CHECK(result.is_square());
        CHECK(result.is_identity());
        CHECK_FALSE(result.is_empty());

        CHECK(result(1, 1) == 1);
        CHECK(result(1, 2) == 0);
        CHECK(result(1, 3) == 0);
        CHECK(result(2, 1) == 0);
        CHECK(result(2, 2) == 1);
        CHECK(result(2, 3) == 0);
        CHECK(result(3, 1) == 0);
        CHECK(result(3, 2) == 0);
        CHECK(result(3, 3) == 1);
    }

    SECTION("First Power") {
        mmath::matrix_base<int> matrix{{ 1, 2, 3 },
                                       { 3, 4, 5 },
                                       { 5, 6, 7 }};

        CHECK_NOTHROW(matrix ^ 1);

        auto result = matrix ^ 1;

        CHECK_FALSE(result.is_zero());
        CHECK(result.is_square());
        CHECK_FALSE(result.is_identity());
        CHECK_FALSE(result.is_empty());

        CHECK(result(1, 1) == 1);
        CHECK(result(1, 2) == 2);
        CHECK(result(1, 3) == 3);
        CHECK(result(2, 1) == 3);
        CHECK(result(2, 2) == 4);
        CHECK(result(2, 3) == 5);
        CHECK(result(3, 1) == 5);
        CHECK(result(3, 2) == 6);
        CHECK(result(3, 3) == 7);
    }

    SECTION("Other Power") {
        mmath::matrix_base<int> matrix{{ 1, 2, 3 },
                                       { 3, 4, 5 },
                                       { 5, 6, 7 }};

        CHECK_NOTHROW(matrix ^ 3);

        auto result = matrix ^ 3;

        CHECK_FALSE(result.is_zero());
        CHECK(result.is_square());
        CHECK_FALSE(result.is_identity());
        CHECK_FALSE(result.is_empty());

        CHECK(result(1, 1) == 276);
        CHECK(result(1, 2) == 360);
        CHECK(result(1, 3) == 444);
        CHECK(result(2, 1) == 516);
        CHECK(result(2, 2) == 672);
        CHECK(result(2, 3) == 828);
        CHECK(result(3, 1) == 756);
        CHECK(result(3, 2) == 984);
        CHECK(result(3, 3) == 1212);
    }
}

TEST_CASE("Matrix Trace") {

    SECTION("Empty Matrix") {
        mmath::matrix_base<int> matrix_a{};
        CHECK_THROWS_AS(mmath::tr(matrix_a), std::invalid_argument);
        CHECK_THROWS_AS(matrix_a.tr(), std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{{},
                                           {},
                                           {}};
        CHECK_THROWS_AS(mmath::tr(matrix_a), std::invalid_argument);
        CHECK_THROWS_AS(matrix_a.tr(), std::invalid_argument);

        matrix_a = mmath::matrix_base<int>{ 0, 10 };
        CHECK_THROWS_AS(mmath::tr(matrix_a), std::invalid_argument);
        CHECK_THROWS_AS(matrix_a.tr(), std::invalid_argument);
    }

    SECTION("Size Checks") {
        mmath::matrix_base<int> matrix_incorrect_a{{ 32, 1, 60, 2 }};
        mmath::matrix_base<int> matrix_incorrect_b{{ 32, 1 }};
        mmath::matrix_base<int> matrix_incorrect_c{{ 32 },
                                                   { 1 },
                                                   { 60 },
                                                   { 2 }};

        CHECK_THROWS_AS(mmath::tr(matrix_incorrect_a), std::invalid_argument);
        CHECK_THROWS_AS(mmath::tr(matrix_incorrect_b), std::invalid_argument);
        CHECK_THROWS_AS(mmath::tr(matrix_incorrect_c), std::invalid_argument);
        CHECK_THROWS_AS(matrix_incorrect_a.tr(), std::invalid_argument);
        CHECK_THROWS_AS(matrix_incorrect_b.tr(), std::invalid_argument);
        CHECK_THROWS_AS(matrix_incorrect_c.tr(), std::invalid_argument);

    }

    SECTION("Trace Validation") {
        mmath::matrix_base<int> matrix_int{{ 12, 34, 56 },
                                           { 32, 51, 2 },
                                           { 1,  2,  3 }};

        REQUIRE_NOTHROW(mmath::tr(matrix_int));

        CHECK(mmath::tr(matrix_int) == 66);
        CHECK(matrix_int.tr() == 66);

        mmath::matrix_base<double> matrix_double{{ 12.12,  34.34,  56.56 },
                                                 { 32.12,  51.11,  2.2222 },
                                                 { 1.1111, 2.2222, 3.3333 }};

        REQUIRE_NOTHROW(mmath::tr(matrix_double));

        using namespace Catch::literals;
        CHECK(mmath::tr(matrix_double) == 66.5633_a);
        CHECK(matrix_double.tr() == 66.5633_a);
    }
}

TEMPLATE_TEST_CASE("make_identity_matrix", "[matrix_base][template]", signed char, // NOLINT(cert-err58-cpp)
                   unsigned char, wchar_t, char8_t, char16_t, char32_t, signed short int, unsigned short int,
                   signed int, unsigned int, signed long int, unsigned long int, signed long long int,
                   unsigned long long int, float, double, long double) {
    SECTION("Empty Matrix") {
        mmath::matrix_base<TestType> matrix = mmath::make_identity_matrix<TestType>(0);
        CHECK(matrix.is_empty());
    }

    SECTION("1x1 Identity") {
        mmath::matrix_base<TestType> matrix = mmath::make_identity_matrix<TestType>(1);
        CHECK(matrix.is_identity());
    }

    SECTION("1-100 Identity") {
        for (int i = 1; i <= 100; ++i) {
            mmath::matrix_base<TestType> matrix = mmath::make_identity_matrix<TestType>(i);
            CHECK(matrix.is_identity());
        }
    }
}

TEST_CASE("Matrix Transpose") {

    mmath::matrix_base<int> m{{ 1, 2, 3 },
                              { 4, 5, 6 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> n{{ 1, 4, 7 },
                              { 2, 5, 8 },
                              { 3, 6, 9 }};

    auto result1 = m.transpose();
    auto result2 = mmath::transpose(m);

    CHECK(result1(1, 1) == n(1, 1));
    CHECK(result2(1, 1) == n(1, 1));
    CHECK(result1(1, 2) == n(1, 2));
    CHECK(result2(1, 2) == n(1, 2));
    CHECK(result1(1, 3) == n(1, 3));
    CHECK(result2(1, 3) == n(1, 3));
    CHECK(result1(2, 1) == n(2, 1));
    CHECK(result2(2, 1) == n(2, 1));
    CHECK(result1(2, 2) == n(2, 2));
    CHECK(result2(2, 2) == n(2, 2));
    CHECK(result1(2, 3) == n(2, 3));
    CHECK(result2(2, 3) == n(2, 3));
    CHECK(result1(3, 1) == n(3, 1));
    CHECK(result2(3, 1) == n(3, 1));
    CHECK(result1(3, 2) == n(3, 2));
    CHECK(result2(3, 2) == n(3, 2));
    CHECK(result1(3, 3) == n(3, 3));
    CHECK(result2(3, 3) == n(3, 3));
}

TEST_CASE("Matrix Elementary Operations - Invalid Operations") {
    using namespace mmath;

    mmath::matrix_base<int> m{ 3, 0 };
    CHECK_THROWS_AS(m | 1_R <=> 2_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 1_C <=> 2_C, std::invalid_argument);

    m = mmath::matrix_base<int>{{ 1, 2, 3 },
                                { 4, 5, 6 },
                                { 7, 8, 9 }};

    // Check swapping out of bounds
    CHECK_THROWS_AS(m | 4_R <=> 1_R, std::out_of_range);
    CHECK_THROWS_AS(m | 1_R <=> 4_R, std::out_of_range);
    CHECK_THROWS_AS(m | 4_C <=> 1_C, std::out_of_range);
    CHECK_THROWS_AS(m | 1_C <=> 4_C, std::out_of_range);

    CHECK_THROWS_AS(m | 2 * 4_R >> 4_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 4_R + 1_R >> 1_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 1_R + 4_R >> 4_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2_R + 2 * 4_R >> 2_R, std::out_of_range);
    CHECK_THROWS_AS(m | 4_R + 2 * 1_R >> 4_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 4_R - 1_R >> 1_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 1_R - 4_R >> 4_R, std::out_of_range);
    CHECK_THROWS_AS(m | 2_R - 2 * 4_R >> 2_R, std::out_of_range);
    CHECK_THROWS_AS(m | 4_R - 2 * 1_R >> 4_R, std::out_of_range);

    CHECK_THROWS_AS(m | 2 * 4_C >> 4_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 4_C + 1_C >> 1_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 1_C + 4_C >> 4_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2_C + 2 * 4_C >> 2_C, std::out_of_range);
    CHECK_THROWS_AS(m | 4_C + 2 * 1_C >> 4_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 4_C - 1_C >> 1_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2 * 1_C - 4_C >> 4_C, std::out_of_range);
    CHECK_THROWS_AS(m | 2_C - 2 * 4_C >> 2_C, std::out_of_range);
    CHECK_THROWS_AS(m | 4_C - 2 * 1_C >> 4_C, std::out_of_range);

    // Check Sending results to incorrect row
    CHECK_THROWS_AS(m | 2 * 1_R >> 2_R, std::logic_error);
    CHECK_THROWS_AS(m | 2 * 1_R + 2_R >> 1_R, std::logic_error);
    CHECK_THROWS_AS(m | 2_R + 2 * 1_R >> 1_R, std::logic_error);
    CHECK_THROWS_AS(m | 2 * 1_C >> 2_C, std::logic_error);
    CHECK_THROWS_AS(m | 2 * 1_C + 2_C >> 1_C, std::logic_error);
    CHECK_THROWS_AS(m | 2_C + 2 * 1_C >> 1_C, std::logic_error);
    CHECK_THROWS_AS(m | 2 * 1_R - 2_R >> 1_R, std::logic_error);
    CHECK_THROWS_AS(m | 2_R - 2 * 1_R >> 1_R, std::logic_error);
    CHECK_THROWS_AS(m | 2 * 1_C - 2_C >> 1_C, std::logic_error);
    CHECK_THROWS_AS(m | 2_C - 2 * 1_C >> 1_C, std::logic_error);


    // Check multiplying by zero
    CHECK_THROWS_AS(m | 0 * 1_R >> 1_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 1_R * 0 >> 1_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 2_R + 0 * 1_R >> 2_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 0 * 1_R + 2_R >> 2_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 1_C * 0 >> 1_C, std::invalid_argument);
    CHECK_THROWS_AS(m | 0 * 1_C >> 1_C, std::invalid_argument);
    CHECK_THROWS_AS(m | 2_C + 0 * 1_C >> 2_C, std::invalid_argument);
    CHECK_THROWS_AS(m | 0 * 1_C + 2_C >> 2_C, std::invalid_argument);
    CHECK_THROWS_AS(m | 2_R - 0 * 1_R >> 2_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 0 * 1_R - 2_R >> 2_R, std::invalid_argument);
    CHECK_THROWS_AS(m | 2_C - 0 * 1_C >> 2_C, std::invalid_argument);
    CHECK_THROWS_AS(m | 0 * 1_C - 2_C >> 2_C, std::invalid_argument);
}

TEST_CASE("Matrix Elementary Operations - Swap") {
    mmath::matrix_base<int> m{{ 1, 2, 3 },
                              { 4, 5, 6 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> n{{ 4, 5, 6 },
                              { 1, 2, 3 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> o{{ 2, 1, 3 },
                              { 5, 4, 6 },
                              { 8, 7, 9 }};

    using namespace mmath;
    auto swapped = m | 1_R <=> 2_R;
    CHECK(swapped == n);
    swapped = m | 2_R <=> 1_R;
    CHECK(swapped == n);

    auto swapped_C = m | 1_C <=> 2_C;
    CHECK(swapped_C == o);
    swapped_C = m | 2_C <=> 1_C;
    CHECK(swapped_C == o);
}

TEST_CASE("Matrix Elementary Operations - Multiply") {
    mmath::matrix_base<int> m{{ 1, 2, 3 },
                              { 4, 5, 6 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> result_R{{ 2, 4, 6 },
                                     { 4, 5, 6 },
                                     { 7, 8, 9 }};

    mmath::matrix_base<int> result_C{{ 2,  2, 3 },
                                     { 8,  5, 6 },
                                     { 14, 8, 9 }};

    using namespace mmath;
    auto multiplied = m | 2 * 1_R >> 1_R;
    CHECK(multiplied == result_R);

    multiplied = m | 1_R * 2 >> 1_R;
    CHECK(multiplied == result_R);

    auto multiplied_C = m | 2 * 1_C >> 1_C;
    CHECK(multiplied_C == result_C);

    multiplied_C = m | 1_C * 2 >> 1_C;
    CHECK(multiplied_C == result_C);
}

TEST_CASE("Matrix Elementary Operations - Add/Sub") {
    mmath::matrix_base<int> m{{ 1, 2, 3 },
                              { 4, 5, 6 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> result_r_add{{ 1, 2, 3 },
                                         { 5, 7, 9 },
                                         { 7, 8, 9 }};
    mmath::matrix_base<int> result_r_sub{{ 1, 2, 3 },
                                         { 3, 3, 3 },
                                         { 7, 8, 9 }};

    mmath::matrix_base<int> result_c_add{{ 1, 3,  3 },
                                         { 4, 9,  6 },
                                         { 7, 15, 9 }};
    mmath::matrix_base<int> result_c_sub{{ 1, 1, 3 },
                                         { 4, 1, 6 },
                                         { 7, 1, 9 }};

    using namespace mmath;
    auto add_r = m | 1_R + 2_R >> 2_R;
    auto sub_r = m | 2_R - 1_R >> 2_R;

    auto add_c = m | 1_C + 2_C >> 2_C;
    auto sub_c = m | 2_C - 1_C >> 2_C;

    CHECK(add_r == result_r_add);
    CHECK(sub_r == result_r_sub);
    CHECK(add_c == result_c_add);
    CHECK(sub_c == result_c_sub);
}

TEST_CASE("Matrix Elementary Operations - Multiply Add/Sub") {
    mmath::matrix_base<int> m{{ 1, 2, 3 },
                              { 4, 5, 6 },
                              { 7, 8, 9 }};

    mmath::matrix_base<int> result_r_add{{ 1, 2,  3 },
                                         { 7, 11, 15 },
                                         { 7, 8,  9 }};
    mmath::matrix_base<int> result_r_sub{{ 1, 2,  3 },
                                         { 1, -1, -3 },
                                         { 7, 8,  9 }};

    mmath::matrix_base<int> result_c_add{{ 1, 4,  3 },
                                         { 4, 13, 6 },
                                         { 7, 22, 9 }};
    mmath::matrix_base<int> result_c_sub{{ 1, 0,  3 },
                                         { 4, -3, 6 },
                                         { 7, -6, 9 }};

    using namespace mmath;
    auto add_r = m | 3 * 1_R + 2_R >> 2_R;
    auto sub_r = m | 2_R - 3 * 1_R >> 2_R;

    auto add_c = m | 2 * 1_C + 2_C >> 2_C;
    auto sub_c = m | 2_C - 2 * 1_C >> 2_C;

    CHECK(add_r == result_r_add);
    CHECK(sub_r == result_r_sub);
    CHECK(add_c == result_c_add);
    CHECK(sub_c == result_c_sub);

    add_r = m | 1_R * 3 + 2_R >> 2_R;
    sub_r = m | 2_R - 1_R * 3 >> 2_R;

    add_c = m | 1_C * 2 + 2_C >> 2_C;
    sub_c = m | 2_C - 1_C * 2 >> 2_C;

    CHECK(add_r == result_r_add);
    CHECK(sub_r == result_r_sub);
    CHECK(add_c == result_c_add);
    CHECK(sub_c == result_c_sub);
}
