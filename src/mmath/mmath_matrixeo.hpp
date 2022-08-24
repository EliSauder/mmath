//
// Created by Elijah Sauder on 4/26/22.
//

#ifndef MMATH_SRC_MMATH_MMATH_MATRIXEO_HPP_
#define MMATH_SRC_MMATH_MMATH_MATRIXEO_HPP_

#include <cstddef>
#include <optional>

#include "mmath_concepts.hpp"
#include "mmath_matrix.hpp"

namespace mmath {

    namespace meotypes {
        enum class matrix_axis_type { ROW, COLUMN };

        template<meotypes::matrix_axis_type T>
        class matrix_axis {
            size_t axis;

        public:
            [[maybe_unused]]
            explicit matrix_axis(size_t axis) : axis(axis) {
            };

            size_t get_axis() {
                return axis;
            }

            [[maybe_unused]]
            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<meotypes::matrix_axis_type T>
        class matrix_axis_added {
        private:
            size_t axis1;
            size_t axis2;
            bool _is_sub;

        public:
            matrix_axis_added(matrix_axis<T> a, matrix_axis<T> b) : axis1(a.get_axis()),
                                                                    axis2(b.get_axis()),
                                                                    _is_sub(false) {
            }

            matrix_axis_added(matrix_axis<T> a, matrix_axis<T> b, bool is_sub) : axis1(a.get_axis()),
                                                                                 axis2(b.get_axis()),
                                                                                 _is_sub(is_sub) {
            }

            size_t get_axis1() {
                return axis1;
            }

            size_t get_axis2() {
                return axis2;
            }

            bool is_sub() {
                return _is_sub;
            }

            [[maybe_unused]]
            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<meotypes::matrix_axis_type T, typename S>
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
                if (multiplicand) {
                    return std::move(*multiplicand);
                }

                throw std::runtime_error("Value has already been accessed");
            }

            size_t get_axis() {
                return axis;
            }

            [[maybe_unused]]
            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<meotypes::matrix_axis_type T, typename S>
        class matrix_axis_added_generic {
        private:
            matrix_axis_multiplied<T, S> multiply_axis;
            size_t axis;
            bool axis_is_a;
            bool mul_axis_is_a;
            bool _is_sub;

        public:
            matrix_axis_added_generic(meotypes::matrix_axis_multiplied<T, S> a, matrix_axis<T> b, bool is_sub = false)
                    : multiply_axis(std::move(a)),
                      axis(b.get_axis()),
                      mul_axis_is_a(true),
                      axis_is_a(false),
                      _is_sub(is_sub) {
            }

            matrix_axis_added_generic(matrix_axis<T> a, matrix_axis_multiplied<T, S> b, bool is_sub = false)
                    : multiply_axis(std::move(b)),
                      axis(a.get_axis()),
                      mul_axis_is_a(false),
                      axis_is_a(true),
                      _is_sub(is_sub) {
            }

            size_t get_axis() {
                return axis;
            }

            bool is_axis_a() {
                return axis_is_a;
            }

            bool is_sub() {
                return _is_sub;
            }

            bool get_is_mul_axis_a() {
                return mul_axis_is_a;
            }

            [[maybe_unused]]
            consteval matrix_axis_type get_type() {
                return T;
            }

            matrix_axis_multiplied<T, S> get_multiplied_axis() {
                return std::move(multiply_axis);
            }
        };

        template<meotypes::matrix_axis_type T>
        class matrix_axis_switch {
            size_t op_axis1;
            size_t op_axis2;
            size_t axis_result;
            size_t axis_swap;
            bool _is_sub = false;

        public:
            matrix_axis_switch(meotypes::matrix_axis<T> axis1, matrix_axis<T> axis2) : op_axis1(axis1.get_axis()),
                                                                                       op_axis2(0),
                                                                                       axis_swap(axis2.get_axis()),
                                                                                       axis_result(0) {
            }

            matrix_axis_switch(meotypes::matrix_axis_added<T> op1, matrix_axis<T> op2) : op_axis1(op1.get_axis1()),
                                                                                         op_axis2(op1.get_axis2()),
                                                                                         axis_result(op2.get_axis()),
                                                                                         axis_swap(0),
                                                                                         _is_sub(op1.is_sub()) {
                if (op_axis1 != axis_result && op_axis2 != axis_result) {
                    throw std::logic_error("One of the rows in the addition must be the destination axis");
                }
            }

            matrix_axis_switch(meotypes::matrix_axis_switch<T> op1, matrix_axis<T> op2) : op_axis1(op1.op_axis1),
                                                                                          op_axis2(op1.op_axis2),
                                                                                          axis_result(op1.axis_result),
                                                                                          axis_swap(op2.get_axis()),
                                                                                          _is_sub(op1._is_sub) {
                if (op1.get_axis_swap() != 0) {
                    throw std::logic_error("Multiple swap operations not supported");
                }
            }

            size_t get_op_axis1() {
                return op_axis1;
            }

            size_t get_op_axis2() {
                return op_axis2;
            }

            size_t get_axis_result() {
                return axis_result;
            }

            size_t get_axis_swap() {
                return axis_swap;
            }

            bool is_sub() {
                return _is_sub;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };

        template<meotypes::matrix_axis_type T, typename S>
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
            matrix_axis_switch_generic(meotypes::matrix_axis_multiplied<T, S> op1, matrix_axis<T> op2)
                    : op_axis1(op1.get_axis()),
                      op_axis2(0),
                      axis_result(op2.get_axis()),
                      axis_swap(0),
                      multiplicand(std::move(op1.get_multiplicand())),
                      mul_axis_is_1(true) {
                if (axis_result != op_axis1) {
                    throw std::logic_error("Elementary Multiplication must go in an addition/subtraction or get sent "
                                           "back to source row");
                }
            }

            matrix_axis_switch_generic(meotypes::matrix_axis_added_generic<T, S> op1, matrix_axis<T> b)
                    : axis_result(b.get_axis()),
                      axis_swap(0),
                      mul_axis_is_1(op1.get_is_mul_axis_a()),
                      _is_sub(op1.is_sub()) {
                matrix_axis_multiplied<T, S> axis_mul = op1.get_multiplied_axis();
                multiplicand = std::move(axis_mul.get_multiplicand());

                if (op1.is_axis_a()) {
                    op_axis1 = op1.get_axis();
                    op_axis2 = axis_mul.get_axis();
                }
                else {
                    op_axis1 = axis_mul.get_axis();
                    op_axis2 = op1.get_axis();
                }

                if (axis_result != (op1.is_axis_a() ? op_axis1 : op_axis2)) {
                    throw std::logic_error("The non-multiplied axis and the destination row must match");
                }
            }

            matrix_axis_switch_generic(meotypes::matrix_axis_switch_generic<T, S> op1, matrix_axis<T> b)
                    : op_axis1(op1.op_axis1),
                      op_axis2(op1.op_axis2),
                      axis_result(op1.axis_result),
                      axis_swap(b.get_axis()),
                      _is_sub(op1.is_sub()),
                      mul_axis_is_1(op1.mul_axis_is_1),
                      multiplicand(std::move(op1.multiplicand)) {
                if (op1.get_axis_swap() != 0) {
                    throw std::logic_error("Multiple swap operations not supported");
                }
            }

            size_t get_axis_swap() {
                return axis_swap;
            }

            size_t get_axis_result() {
                return axis_result;
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

            bool is_sub() {
                return _is_sub;
            }

            consteval matrix_axis_type get_type() {
                return T;
            }
        };
    }  // namespace meotypes

    namespace impl {

        template<expression T, expression S>
        void axis_operation(matrix_base<T>& matrix,
                            size_t axis_a,
                            size_t axis_b,
                            size_t axis_result,
                            size_t axis_swap,
                            bool is_sub,
                            bool mul_is_a,
                            std::optional<S> multiplicand,
                            meotypes::matrix_axis_type axis_type) {
            // If all we have been provided is data for a swap operation, and the swap
            // rows are the same, return
            if (axis_a != 0 && axis_b == 0 && axis_swap != 0 && !multiplicand && axis_a == axis_swap) {
                return;
            }

            if (axis_b == 0 && !mul_is_a) {
                throw std::invalid_argument("If the multiplied axis is not a, b cannot be 0");
            }
            if (axis_a == 0) {
                throw std::invalid_argument("axis_a must be nonzero");
            }
            if ((axis_b != 0 || multiplicand) && axis_result == 0) {
                throw std::invalid_argument("If axis are being added or multiplied, axis_result must be "
                                            "provided");
            }

            if (axis_result == 0) {
                axis_result = axis_a;
            }

            bool is_column = axis_type == meotypes::matrix_axis_type::COLUMN;

            size_t bound_to_exceed = is_column ? matrix.size_col() : matrix.size_row();
            if (axis_a > bound_to_exceed || axis_b > bound_to_exceed || axis_result > bound_to_exceed ||
                axis_swap > bound_to_exceed) {
                throw std::out_of_range("One of the axis requested lies outside the matrix");
            }

            size_t loop_bound = is_column ? matrix.size_row() : matrix.size_col();

            for (int i = 1; i <= loop_bound; ++i) {
                T value = std::move(matrix(axis_a, i, is_column));

                // Perform multiplication with a if specified;
                if (mul_is_a && multiplicand) {
                    value = (*multiplicand) * value;
                }

                // Perform addition/subtraction with b
                if (axis_b != 0) {
                    // Get the value of b as a multiplied value or just the straight value
                    T value_b = !mul_is_a && multiplicand
                                ? (*multiplicand) * std::move(matrix(axis_b, i, is_column))
                                : matrix(axis_b, i, is_column);

                    value = is_sub ? std::move(value) - std::move(value_b) : std::move(value) + std::move(value_b);
                }

                // Swap or place values in result
                if (axis_swap != 0) {
                    matrix(axis_result, i, is_column) = std::move(matrix(axis_swap, i, is_column));
                    matrix(axis_swap, i, is_column) = std::move(value);
                }
                else {
                    matrix(axis_result, i, is_column) = std::move(value);
                }
            }
        }
    }  // namespace impl

#pragma region "| Operator"

    template<expression T, meotypes::matrix_axis_type W>
    matrix_base<T>& operator|(matrix_base<T>& m, meotypes::matrix_axis_switch<W> axis) {
        if (m.is_empty()) {
            throw std::invalid_argument("Elementary operations cannot be performed on an empty matrix.");
        }

        impl::axis_operation<T, int>(m,
                                     axis.get_op_axis1(),
                                     axis.get_op_axis2(),
                                     axis.get_axis_result(),
                                     axis.get_axis_swap(),
                                     axis.is_sub(),
                                     true,
                                     std::nullopt,
                                     axis.get_type());

        return m;
    }

    template<expression T, meotypes::matrix_axis_type W>
    matrix_base<T> operator|(const matrix_base<T>& m, meotypes::matrix_axis_switch<W> axis) {
        matrix_base<T> new_matrix = m;
        return new_matrix | std::move(axis);
    }

    template<expression T, meotypes::matrix_axis_type W, expression S>
    matrix_base<T>& operator|(matrix_base<T>& m, meotypes::matrix_axis_switch_generic<W, S> axis) {
        if (m.is_empty()) {
            throw std::invalid_argument("Elementary operations cannot be performed on an empty matrix.");
        }

        impl::axis_operation<T, S>(m,
                                   axis.get_op_axis1(),
                                   axis.get_op_axis2(),
                                   axis.get_axis_result(),
                                   axis.get_axis_swap(),
                                   axis.is_sub(),
                                   axis.is_mul_axis1(),
                                   std::move(axis.get_multiplicand()),
                                   axis.get_type());
        return m;
    }

    template<expression T, meotypes::matrix_axis_type W, expression S>
    matrix_base<T> operator|(const matrix_base<T>& m, meotypes::matrix_axis_switch_generic<W, S> axis) {
        matrix_base<T> new_matrix = m;
        return new_matrix | std::move(axis);
    }

#pragma endregion "| Operator"

#pragma region "<=> Operator"

    template<meotypes::matrix_axis_type T>
    meotypes::matrix_axis_switch<T> operator<=>(meotypes::matrix_axis<T> a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_switch<T>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T>
    meotypes::matrix_axis_switch<T> operator<=>(meotypes::matrix_axis_switch<T> a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_switch<T>{ std::move(a), std::move(b) };
    }

#pragma endregion "<=> Operator"

#pragma region ">> Operator"

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_switch_generic<T, S> operator>>(meotypes::matrix_axis_multiplied<T, S> a,
                                                          meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_switch_generic<T, S>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T>
    meotypes::matrix_axis_switch<T> operator>>(meotypes::matrix_axis_added<T> a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_switch<T>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_switch_generic<T, S> operator>>(meotypes::matrix_axis_added_generic<T, S> a,
                                                          meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_switch_generic<T, S>{ std::move(a), std::move(b) };
    }

#pragma endregion ">> Operator"

#pragma region "+ and - Operators"

    template<meotypes::matrix_axis_type T>
    meotypes::matrix_axis_added<T> operator+(meotypes::matrix_axis<T> a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_added<T>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_added_generic<T, S> operator+(meotypes::matrix_axis_multiplied<T, S> a,
                                                        meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_added_generic<T, S>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_added_generic<T, S> operator+(meotypes::matrix_axis<T> a,
                                                        meotypes::matrix_axis_multiplied<T, S> b) {
        return meotypes::matrix_axis_added_generic<T, S>{ std::move(b), std::move(a) };
    }

    template<meotypes::matrix_axis_type T>
    meotypes::matrix_axis_added<T> operator-(meotypes::matrix_axis<T> a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_added<T>{ std::move(a), std::move(b), true };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_added_generic<T, S> operator-(meotypes::matrix_axis_multiplied<T, S> a,
                                                        meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_added_generic<T, S>{ std::move(a), std::move(b), true };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_added_generic<T, S> operator-(meotypes::matrix_axis<T> a,
                                                        meotypes::matrix_axis_multiplied<T, S> b) {
        return meotypes::matrix_axis_added_generic<T, S>{ std::move(a), std::move(b), true };
    }

#pragma endregion "+ and - Operators"

#pragma region "* Operator"

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_multiplied<T, S> operator*(S a, meotypes::matrix_axis<T> b) {
        return meotypes::matrix_axis_multiplied<T, S>{ std::move(a), std::move(b) };
    }

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_multiplied<T, S> operator*(meotypes::matrix_axis<T> a, S b) {
        return meotypes::matrix_axis_multiplied<T, S>{ std::move(b), std::move(a) };
    }

#pragma endregion "* Operator"

#pragma region "/ Operator"

    template<meotypes::matrix_axis_type T, typename S>
    meotypes::matrix_axis_multiplied<T, S> operator/(meotypes::matrix_axis<T> a, S b) {
        return meotypes::matrix_axis_multiplied<T, S>{ 1 / std::move(b), std::move(a) };
    }

#pragma endregion "/ Operator"

    using matrix_row = meotypes::matrix_axis<meotypes::matrix_axis_type::ROW>;
    using matrix_column = meotypes::matrix_axis<meotypes::matrix_axis_type::COLUMN>;

    namespace literals {

        matrix_row operator ""_R(unsigned long long t) {
            return matrix_row{ t };
        }

        matrix_column operator ""_C(unsigned long long t) {
            return matrix_column{ t };
        }

    }  // namespace literals

}  // namespace mmath

#endif  // MMATH_SRC_MMATH_MMATH_MATRIXEO_HPP_
