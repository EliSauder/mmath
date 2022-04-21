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
    public:
        matrix_row_added(matrix_row a, matrix_row b) {
            // TODO: Implement
        }
    };

    template<typename S>
    class matrix_row_multiplied {
    public:
        matrix_row_multiplied(S a, matrix_row b) {
            // TODO: Implement
        }
    };

    template<typename S>
    class matrix_row_added_generic {
    public:
        matrix_row_added_generic(matrix_row_multiplied<S> a, matrix_row b) {
            // TODO: Implement
        }
    };

    class matrix_row_switch {
        size_t row1;
        size_t row2;
    public:
        matrix_row_switch(matrix_row row1, matrix_row row2) : row1(row1.get_row()), row2(row2.get_row()) {
        }

        matrix_row_switch(matrix_row_added op1, matrix_row op2) {
            // TODO: Implement
        }
    };

    template<typename S>
    class matrix_row_switch_generic {
    public:
        matrix_row_switch_generic(matrix_row_multiplied<S> op1, matrix_row op2) {
            // TODO: Implement
        }

        matrix_row_switch_generic(matrix_row_added_generic<S> a, matrix_row b) {
            // TODO: Implement
        }
    };

    /*
     * >> OPERATORS
     */

    matrix_row_switch operator>>(matrix_row a, matrix_row b) {
        return matrix_row_switch{a, a};
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
