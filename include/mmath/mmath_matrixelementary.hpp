//
// Created by Elijah Sauder on 4/19/22.
//

#ifndef MMATH_MMATH_MATRIXELEMENTARY_HPP
#define MMATH_MMATH_MATRIXELEMENTARY_HPP

#include <cstddef>
#include <optional>

#include "mmath_concepts.hpp"

namespace mmath {

    enum class row_operation_type {
        ADD,
        SUB,
        MUL,
        TRANSFER
    };

    /*
     * CLASSES
     */

    class matrix_row {
        size_t row;
    public:
        matrix_row(size_t row) : row(row) {};

        size_t get_row() {
            return row;
        }
    };

    class matrix_row_added {
    private:
        size_t row1;
        size_t row2;
    public:
        size_t get_row1() {
            return row1;
        }

        size_t get_row2() {
            return row2;
        }

        matrix_row_added(matrix_row a, matrix_row b) : row1(a.get_row()), row2(b.get_row()) {}
    };

    template<typename S>
    class matrix_row_multiplied {
    private:
        std::optional<S> multiplicand;
        size_t row;
    public:
        matrix_row_multiplied(S a, matrix_row b) : multiplicand(std::move(a)), row(b.get_row()) {
            if (multiplicand.value() == 0) {
                throw std::invalid_argument("Elementary multiply operation cannot be by 0.");
            }
        }

        S get_multiplicand() {
            if (multiplicand)
                return std::move(*multiplicand);
            throw std::runtime_error("Value has already been accessed");
        }

        size_t get_row() {
            return row;
        }
    };

    template<typename S>
    class matrix_row_added_generic {
    private:
        matrix_row_multiplied<S> multiply_row;
        size_t row;
    public:
        matrix_row_added_generic(matrix_row_multiplied<S> a, matrix_row b) : multiply_row(std::move(a)),
                                                                             row(b.get_row()) {
        }

        size_t get_row() {
            return row;
        }

        matrix_row_multiplied<S> get_multiplied_row() {
            return std::move(multiply_row);
        }
    };

    class matrix_row_switch {
        size_t row_from;
        size_t row_to;
        size_t op_row1;
        size_t op_row2;
    public:
        matrix_row_switch(matrix_row row1, matrix_row row2) : row_from(row1.get_row()), row_to(row2.get_row()),
                                                              op_row1(0),
                                                              op_row2(0) {
        }

        matrix_row_switch(matrix_row_added op1, matrix_row op2) : row_from(0), row_to(op2.get_row()),
                                                                  op_row1(op1.get_row1()), op_row2(op1.get_row2()) {
            if (op_row1 != row_to && op_row2 != row_to) {
                throw std::invalid_argument("One of the rows in the addition must be the destination row");
            }
        }

        size_t get_row_from() {
            return row_from;
        }

        size_t get_row_to() {
            return row_to;
        }

        size_t get_op_row1() {
            return op_row1;
        }

        size_t get_op_row2() {
            return op_row2;
        }
    };

    template<typename S>
    class matrix_row_switch_generic {
    private:
        size_t row_to;
        size_t op_row1;
        size_t op_row2;
        S multiplicand;
    public:
        matrix_row_switch_generic(matrix_row_multiplied<S> op1, matrix_row op2) : row_to(op2.get_row()),
                                                                                  op_row1(op1.get_row()), op_row2(0),
                                                                                  multiplicand(std::move(
                                                                                          op1.get_multiplicand())) {
        }

        matrix_row_switch_generic(matrix_row_added_generic<S> op1, matrix_row b) : row_to(b.get_row()),
                                                                                   op_row1(op1.get_row()), op_row2(0) {
            matrix_row_multiplied<S> row_mul = op1.get_multiplied_row();
            op_row2 = row_mul.get_row();
            multiplicand = std::move(row_mul.get_multiplicand());
        }

        size_t get_row_to() {
            return row_to;
        }

        size_t get_op_row1() {
            return op_row1;
        }

        size_t get_op_row2() {
            return op_row2;
        }

        S get_multiplicand() {
            return std::move(multiplicand);
        }
    };

    /*
     * >> OPERATORS
     */

    matrix_row_switch operator<=>(matrix_row a, matrix_row b) {
        return matrix_row_switch{a, b};
    }

    template<typename S>
    matrix_row_switch_generic<S> operator>>(matrix_row_multiplied<S> a, matrix_row b) {
        return matrix_row_switch_generic<S>{a, b};
    }

    matrix_row_switch operator>>(matrix_row_added a, matrix_row b) {
        return matrix_row_switch{a, b};
    }

    template<typename S>
    matrix_row_switch_generic<S> operator>>(matrix_row_added_generic<S> a, matrix_row b) {
        return matrix_row_switch_generic<S>{a, b};
    }

    /*
     * + OPERATORS
     */

    matrix_row_added operator+(matrix_row a, matrix_row b) {
        return matrix_row_added{a, b};
    }

    template<typename S>
    matrix_row_added_generic<S> operator+(matrix_row_multiplied<S> a, matrix_row b) {
        return matrix_row_added_generic<S>{a, b};
    }

    template<typename S>
    matrix_row_added_generic<S> operator+(matrix_row a, matrix_row_multiplied<S> b) {
        return matrix_row_added_generic<S>{b, a};
    }

    /*
     * * OPERATORS
     */

    template<typename S>
    matrix_row_multiplied<S> operator*(S a, matrix_row b) {
        return matrix_row_multiplied<S>{std::move(a), b};
    }

    /*
     * LITERALS
     */

    matrix_row operator ""_R(unsigned long long t) {
        return matrix_row{t};
    }

}

#endif //MMATH_MMATH_MATRIXELEMENTARY_HPP
