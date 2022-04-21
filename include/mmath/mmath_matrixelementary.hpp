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

        enum class matrix_axis_type {
            ROW,
            COLUMN
        };

        /*
         * CLASSES
         */

        template<matrix_axis_type T>
        class matrix_axis {
            size_t axis;
        public:
            matrix_axis(size_t axis) : axis(axis) { };

            size_t get_axis() {
                return axis;
            }

            matrix_axis_type get_type() {
                return T;
            }
        };

        template<matrix_axis_type T>
        class matrix_axis_added {
        private:
            size_t axis1;
            size_t axis2;
        public:
            matrix_axis_added(matrix_axis<T> a, matrix_axis<T> b) : axis1(a.get_axis()), axis2(b.get_axis()) { }

            size_t get_axis1() {
                return axis1;
            }

            size_t get_axis2() {
                return axis2;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<matrix_axis_type T, typename S>
        class matrix_axis_multiplied {
        private:
            std::optional<S> multiplicand;
            size_t axis;
        public:
            matrix_axis_multiplied(S a, matrix_axis<T> b) : multiplicand(std::move(a)), axis(b.get_axis()) {
                if (multiplicand.value() == 0) {
                    throw std::invalid_argument("Elementary multiply operation cannot be by 0.");
                }
            }

            S get_multiplicand() {
                if (multiplicand)
                    return std::move(*multiplicand);
                throw std::runtime_error("Value has already been accessed");
            }

            size_t get_axis() {
                return axis;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<matrix_axis_type T, typename S>
        class matrix_axis_added_generic {
        private:
            matrix_axis_multiplied<T, S> multiply_axis;
            size_t axis;
            bool mul_axis_is_a;
        public:
            matrix_axis_added_generic(matrix_axis_multiplied<T, S> a, matrix_axis <T> b) : multiply_axis(std::move(a)),
                                                                                           axis(b.get_axis()),
                                                                                           mul_axis_is_a(true) {
            }

            matrix_axis_added_generic(matrix_axis <T> a, matrix_axis_multiplied<T, S> b) : multiply_axis(std::move(a)),
                                                                                           axis(b.get_axis()),
                                                                                           mul_axis_is_a(false) {
            }

            size_t get_axis() {
                return axis;
            }

            bool get_is_mul_axis_a() {
                return mul_axis_is_a;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }

            matrix_axis_multiplied<T, S> get_multiplied_axis() {
                return std::move(multiply_axis);
            }
        };

        template<matrix_axis_type T>
        class matrix_axis_switch {
            size_t axis_from;
            size_t axis_to;
            size_t op_axis1;
            size_t op_axis2;
        public:
            matrix_axis_switch(matrix_axis<T> axis1, matrix_axis<T> axis2) : axis_from(axis1.get_axis()),
                                                                             axis_to(axis2.get_axis()),
                                                                             op_axis1(0),
                                                                             op_axis2(0) {
            }

            matrix_axis_switch(matrix_axis_added<T> op1, matrix_axis<T> op2) : axis_from(0), axis_to(op2.get_axis()),
                                                                               op_axis1(op1.get_axis1()),
                                                                               op_axis2(op1.get_axis2()) {
                if (op_axis1 != axis_to && op_axis2 != axis_to) {
                    throw std::logic_error("One of the rows in the addition must be the destination axis");
                }
            }

            size_t get_axis_from() {
                return axis_from;
            }

            size_t get_axis_to() {
                return axis_to;
            }

            size_t get_op_axis1() {
                return op_axis1;
            }

            size_t get_op_axis2() {
                return op_axis2;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<matrix_axis_type T, typename S>
        class matrix_axis_switch_generic {
        private:
            size_t axis_to;
            size_t op_axis1;
            size_t op_axis2;
            bool mul_axis_is_1;
            S multiplicand;
        public:
            matrix_axis_switch_generic(matrix_axis_multiplied<T, S> op1, matrix_axis<T> op2) : axis_to(op2.get_axis()),
                                                                                               op_axis1(op1.get_axis()),
                                                                                               op_axis2(0),
                                                                                               multiplicand(std::move(
                                                                                                       op1.get_multiplicand())),
                                                                                               mul_axis_is_1(false) {
                if (axis_to != op_axis1)
                    throw std::logic_error(
                            "Elementary Multiplication must go in an addition/subtraction or get sent back to source row");
            }

            matrix_axis_switch_generic(matrix_axis_added_generic<T, S> op1, matrix_axis<T> b) : axis_to(b.get_axis()),
                                                                                                op_axis1(
                                                                                                        op1.get_axis()),
                                                                                                op_axis2(0),
                                                                                                mul_axis_is_1(
                                                                                                        op1.get_is_mul_axis_a()) {
                if (axis_to != op_axis1)
                    throw std::logic_error("The non-multiplied axis and the destination row must match");

                matrix_axis_multiplied<T, S> axis_mul = op1.get_multiplied_axis();
                op_axis2 = axis_mul.get_axis();
                multiplicand = std::move(axis_mul.get_multiplicand());
            }

            size_t get_axis_to() {
                return axis_to;
            }

            size_t get_op_axis1() {
                return op_axis1;
            }

            size_t get_op_axis2() {
                return op_axis2;
            }

            S get_multiplicand() {
                return std::move(multiplicand);
            }

            bool is_mul_axis1() {
                return mul_axis_is_1;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        /*
         * >> OPERATORS
         */

        template<matrix_axis_type T>
        matrix_axis_switch<T> operator<=>(matrix_axis<T> a, matrix_axis<T> b) {
            return matrix_axis_switch<T>{ a, b };
        }

        template<matrix_axis_type T, typename S>
        matrix_axis_switch_generic<T, S> operator>>(matrix_axis_multiplied<T, S> a, matrix_axis<T> b) {
            return matrix_axis_switch_generic < T, S > { a, b };
        }

        template<matrix_axis_type T>
        matrix_axis_switch<T> operator>>(matrix_axis_added<T> a, matrix_axis<T> b) {
            return matrix_axis_switch<T>{ a, b };
        }

        template<matrix_axis_type T, typename S>
        matrix_axis_switch_generic<T, S> operator>>(matrix_axis_added_generic<T, S> a, matrix_axis<T> b) {
            return matrix_axis_switch_generic < T, S > { a, b };
        }

        /*
         * + OPERATORS
         */

        template<matrix_axis_type T>
        matrix_axis_added<T> operator+(matrix_axis<T> a, matrix_axis<T> b) {
            return matrix_axis_added<T>{ a, b };
        }

        template<matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator+(matrix_axis_multiplied<T, S> a, matrix_axis<T> b) {
            return matrix_axis_added_generic<T, S>{ a, b };
        }

        template<matrix_axis_type T, typename S>
        matrix_axis_added_generic<T, S> operator+(matrix_axis<T> a, matrix_axis_multiplied<T, S> b) {
            return matrix_axis_added_generic<T, S>{ b, a };
        }

        /*
         * * OPERATORS
         */

        template<matrix_axis_type T, typename S>
        matrix_axis_multiplied<T, S> operator*(S a, matrix_axis<T> b) {
            return matrix_axis_multiplied<T, S>{ std::move(a), b };
        }

        /*
         * LITERALS
         */

    }

    using matrix_row = m_eo::matrix_axis<m_eo::matrix_axis_type::ROW>;
    using matrix_column = m_eo::matrix_axis<m_eo::matrix_axis_type::COLUMN>;

    matrix_row operator ""_R(unsigned long long t) {
        return matrix_row{ t };
    }

    matrix_column operator ""_C(unsigned long long t) {
        return matrix_column{ t };
    }
}

#endif //MMATH_MMATH_MATRIXELEMENTARY_HPP
