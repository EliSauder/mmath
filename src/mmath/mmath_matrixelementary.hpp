//
// Created by Elijah Sauder on 4/19/22.
//

#ifndef MMATH_MMATH_MATRIXELEMENTARY_HPP
#define MMATH_MMATH_MATRIXELEMENTARY_HPP

#include <cstddef>
#include <optional>

#include "mmath_concepts.hpp"

namespace mmath {
    namespace m_eo {

        enum class matrix_axis_type { ROW, COLUMN };

        /*
         * CLASSES
         */

        template <matrix_axis_type T>
        class matrix_axis {
            size_t axis;

          public:
            [[maybe_unused]] explicit matrix_axis(size_t axis)
                : axis(axis){};

            size_t get_axis() { return axis; }

            [[maybe_unused]] matrix_axis_type get_type() { return T; }
        };

        template <matrix_axis_type T>
        class matrix_axis_added {
          private:
            size_t axis1;
            size_t axis2;
            bool _is_sub;

          public:
            matrix_axis_added(matrix_axis<T> a, matrix_axis<T> b)
                : axis1(a.get_axis())
                , axis2(b.get_axis())
                , _is_sub(false) {}

            matrix_axis_added(matrix_axis<T> a, matrix_axis<T> b, bool is_sub)
                : axis1(a.get_axis())
                , axis2(b.get_axis())
                , _is_sub(is_sub) {}

            size_t get_axis1() { return axis1; }

            size_t get_axis2() { return axis2; }

            bool is_sub() { return _is_sub; }

            [[maybe_unused]] consteval matrix_axis_type get_type() { return T; }
        };

        template <matrix_axis_type T, typename S>
        class matrix_axis_multiplied {
          private:
            std::optional<S> multiplicand;
            size_t axis;

          public:
            matrix_axis_multiplied(S a, matrix_axis<T> b)
                : multiplicand(std::move(a))
                , axis(b.get_axis()) {
                if (multiplicand.value() == 0) {
                    throw std::invalid_argument("Elementary multiply operation cannot be by 0.");
                }
            }

            S get_multiplicand() {
                if (multiplicand)
                    return std::move(*multiplicand);
                throw std::runtime_error("Value has already been accessed");
            }

            size_t get_axis() { return axis; }

            [[maybe_unused]] consteval matrix_axis_type get_type() { return T; }
        };

        template <matrix_axis_type T, typename S>
        class matrix_axis_added_generic {
          private:
            matrix_axis_multiplied<T, S> multiply_axis;
            size_t axis;
            bool axis_is_a;
            bool mul_axis_is_a;
            bool _is_sub;

          public:
            matrix_axis_added_generic(matrix_axis_multiplied<T, S> a, matrix_axis<T> b, bool is_sub = false)
                : multiply_axis(std::move(a))
                , axis(b.get_axis())
                , mul_axis_is_a(true)
                , axis_is_a(false)
                , _is_sub(is_sub) {}

            matrix_axis_added_generic(matrix_axis<T> a, matrix_axis_multiplied<T, S> b, bool is_sub = false)
                : multiply_axis(std::move(b))
                , axis(a.get_axis())
                , mul_axis_is_a(false)
                , axis_is_a(true)
                , _is_sub(is_sub) {}

            size_t get_axis() { return axis; }

            bool is_axis_a() { return axis_is_a; }

            bool is_sub() { return _is_sub; }

            bool get_is_mul_axis_a() { return mul_axis_is_a; }

            [[maybe_unused]] consteval matrix_axis_type get_type() { return T; }

            matrix_axis_multiplied<T, S> get_multiplied_axis() { return std::move(multiply_axis); }
        };

        template <matrix_axis_type T>
        class matrix_axis_switch {
            size_t op_axis1;
            size_t op_axis2;
            size_t axis_result;
            size_t axis_swap;
            bool _is_sub = false;

          public:
            matrix_axis_switch(matrix_axis<T> axis1, matrix_axis<T> axis2)
                : op_axis1(axis1.get_axis())
                , op_axis2(0)
                , axis_swap(axis2.get_axis())
                , axis_result(0) {}

            matrix_axis_switch(matrix_axis_added<T> op1, matrix_axis<T> op2)
                : op_axis1(op1.get_axis1())
                , op_axis2(op1.get_axis2())
                , axis_result(op2.get_axis())
                , axis_swap(0)
                , _is_sub(op1.is_sub()) {
                if (op_axis1 != axis_result && op_axis2 != axis_result) {
                    throw std::logic_error("One of the rows in the addition must be the destination axis");
                }
            }

            matrix_axis_switch(matrix_axis_switch<T> op1, matrix_axis<T> op2)
                : op_axis1(op1.op_axis1)
                , op_axis2(op1.op_axis2)
                , axis_result(op1.axis_result)
                , axis_swap(op2.get_axis())
                , _is_sub(op1._is_sub) {
                if (op1.get_axis_swap() != 0) {
                    throw std::logic_error("Multiple swap operations not supported");
                }
            }

            size_t get_op_axis1() { return op_axis1; }

            size_t get_op_axis2() { return op_axis2; }

            size_t get_axis_result() { return axis_result; }

            size_t get_axis_swap() { return axis_swap; }

            bool is_sub() { return _is_sub; }

            consteval matrix_axis_type get_type() { return T; }
        };

        template <matrix_axis_type T, typename S>
        class matrix_axis_switch_generic {
          private:
            size_t op_axis1;
            size_t op_axis2;
            size_t axis_result;
            size_t axis_swap;
            bool mul_axis_is_1;
            S multiplicand;
            bool _is_sub = false;

          public:
            matrix_axis_switch_generic(matrix_axis_multiplied<T, S> op1, matrix_axis<T> op2)
                : op_axis1(op1.get_axis())
                , op_axis2(0)
                , axis_result(op2.get_axis())
                , axis_swap(0)
                , multiplicand(std::move(op1.get_multiplicand()))
                , mul_axis_is_1(true) {
                if (axis_result != op_axis1)
                    throw std::logic_error("Elementary Multiplication must go in an addition/subtraction or get sent "
                                           "back to source row");
            }

            matrix_axis_switch_generic(matrix_axis_added_generic<T, S> op1, matrix_axis<T> b)
                : axis_result(b.get_axis())
                , axis_swap(0)
                , mul_axis_is_1(op1.get_is_mul_axis_a())
                , _is_sub(op1.is_sub()) {
                matrix_axis_multiplied<T, S> axis_mul = op1.get_multiplied_axis();
                multiplicand = std::move(axis_mul.get_multiplicand());

                if (op1.is_axis_a()) {
                    op_axis1 = op1.get_axis();
                    op_axis2 = axis_mul.get_axis();
                } else {
                    op_axis1 = axis_mul.get_axis();
                    op_axis2 = op1.get_axis();
                }

                if (axis_result != (op1.is_axis_a() ? op_axis1 : op_axis2))
                    throw std::logic_error("The non-multiplied axis and the destination row must match");
            }

            matrix_axis_switch_generic(matrix_axis_switch_generic<T, S> op1, matrix_axis<T> b)
                : op_axis1(op1.op_axis1)
                , op_axis2(op1.op_axis2)
                , axis_result(op1.axis_result)
                , axis_swap(b.get_axis())
                , _is_sub(op1.is_sub())
                , mul_axis_is_1(op1.mul_axis_is_1)
                , multiplicand(std::move(op1.multiplicand)) {
                if (op1.get_axis_swap() != 0) {
                    throw std::logic_error("Multiple swap operations not supported");
                }
            }

            size_t get_axis_swap() { return axis_swap; }

            size_t get_axis_result() { return axis_result; }

            size_t get_op_axis1() { return op_axis1; }

            size_t get_op_axis2() { return op_axis2; }

            S get_multiplicand() { return std::move(multiplicand); }

            bool is_mul_axis1() { return mul_axis_is_1; }

            bool is_sub() { return _is_sub; }

            consteval matrix_axis_type get_type() { return T; }
        };

        /*
         * <=> OPERATORS
         */

        template <matrix_axis_type T>
        matrix_axis_switch<T> operator<=>(matrix_axis<T> a, matrix_axis<T> b) {
            return matrix_axis_switch<T>{a, b};
        }

        template <matrix_axis_type T>
        matrix_axis_switch<T> operator<=>(matrix_axis_switch<T> a, matrix_axis<T> b) {
            return matrix_axis_switch<T>{a, b};
        }

        /*
         * >> OPERATORS
         */

        template <matrix_axis_type T, typename S>
        matrix_axis_switch_generic<T, S> operator>>(matrix_axis_multiplied<T, S> a, matrix_axis<T> b) {
            return matrix_axis_switch_generic<T, S>{a, b};
        }

        template <matrix_axis_type T>
        matrix_axis_switch<T> operator>>(matrix_axis_added<T> a, matrix_axis<T> b) {
            return matrix_axis_switch<T>{a, b};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_switch_generic<T, S> operator>>(matrix_axis_added_generic<T, S> a, matrix_axis<T> b) {
            return matrix_axis_switch_generic<T, S>{a, b};
        }

        /*
         * + OPERATORS
         */

        template <matrix_axis_type T>
        matrix_axis_added<T> operator+(matrix_axis<T> a, matrix_axis<T> b) {
            return matrix_axis_added<T>{a, b};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator+(matrix_axis_multiplied<T, S> a, matrix_axis<T> b) {
            return matrix_axis_added_generic<T, S>{a, b};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator+(matrix_axis<T> a, matrix_axis_multiplied<T, S> b) {
            return matrix_axis_added_generic<T, S>{b, a};
        }

        template <matrix_axis_type T>
        matrix_axis_added<T> operator-(matrix_axis<T> a, matrix_axis<T> b) {
            return matrix_axis_added<T>{a, b, true};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator-(matrix_axis_multiplied<T, S> a, matrix_axis<T> b) {
            return matrix_axis_added_generic<T, S>{a, b, true};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator-(matrix_axis<T> a, matrix_axis_multiplied<T, S> b) {
            return matrix_axis_added_generic<T, S>{a, b, true};
        }

        /*
         * * OPERATORS
         */

        template <matrix_axis_type T, typename S>
        matrix_axis_multiplied<T, S> operator*(S a, matrix_axis<T> b) {
            return matrix_axis_multiplied<T, S>{std::move(a), b};
        }

        template <matrix_axis_type T, typename S>
        matrix_axis_multiplied<T, S> operator*(matrix_axis<T> a, S b) {
            return matrix_axis_multiplied<T, S>{std::move(b), a};
        }

        /*
         * / operator
         */

        template <matrix_axis_type T, typename S>
        matrix_axis_multiplied<T, S> operator/(matrix_axis<T> a, S b) {
            return matrix_axis_multiplied<T, S>{1 / std::move(b), a};
        }

        /*
         * LITERALS
         */

    }  // namespace m_eo

    using matrix_row = m_eo::matrix_axis<m_eo::matrix_axis_type::ROW>;
    using matrix_column = m_eo::matrix_axis<m_eo::matrix_axis_type::COLUMN>;

    matrix_row operator""_R(unsigned long long t) {
        return matrix_row{t};
    }

    matrix_column operator""_C(unsigned long long t) {
        return matrix_column{t};
    }
}  // namespace mmath

#endif  // MMATH_MMATH_MATRIXELEMENTARY_HPP
