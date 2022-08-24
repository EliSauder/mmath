//
// Created by Elijah Sauder on 4/26/22.
//

#ifndef MMATH_SRC_MMATH_MMATH_MATRIXALGORITHMS_HPP_
#define MMATH_SRC_MMATH_MMATH_MATRIXALGORITHMS_HPP_

#include <exception>

#include "mmath_concepts.hpp"
#include "mmath_matrix.hpp"
#include "mmath_matrixeo.hpp"

namespace mmath {

    template<expression T>
    T trace(const matrix_base<T>& m) {
        if (m.is_empty()) {
            throw std::invalid_argument("Matrix must not be empty to calculate trace");
        }
        if (!m.is_square()) {
            throw std::invalid_argument("Matrix must be square to calculate trace");
        }

        T sum = T{ };

        // TODO: Add from both ends of matrix to improve speed
        for (int i = 1; i <= m.size_row(); ++i) {
            sum = sum + m(i, i);
        }

        return sum;
    }

    template<expression T>
    T tr(const matrix_base<T>& m) {
        return trace(m);
    }

    // TODO: Call modifying version of function to reduce code duplicity
    template<expression T>
    matrix_base<T> transpose(const matrix_base<T>& m) {
        matrix_base<T> new_data{ m.size_row(), m.size_col() };

        for (int i = 1; i <= m.size_row(); ++i) {
            for (int j = 1; j <= m.size_col(); ++j) {
                new_data(j, i) = m(i, j);
            }
        }

        return new_data;
    }

    template<expression T>
    matrix_base<T>& triangular(matrix_base<T>& m, bool is_upper) {
        if (!m.is_square()) {
            throw std::invalid_argument("Matrix must be square");
        }

        for (unsigned int i = 1; i <= m.size_row(); i++) {
            for (unsigned int j = 1; j < i; j++) {
                if (is_upper) {
                    m | matrix_row{ i } - m(i, 1) * matrix_row{ j - 1 } >> matrix_row{ i };
                    continue;
                }
                m | matrix_column{ i } - m(i, 1) * matrix_column{ j - 1 } >> matrix_column{ i };
            }

            m | matrix_row{ i } / m(i, i) >> matrix_row{ i };
        }
    }

    template<expression T>
    matrix_base<T> triangular(const matrix_base<T>& m, bool is_upper) {
        matrix_base<T> new_data = m;
        triangular(new_data, is_upper);
        return new_data;
    }

    template<expression T>
    matrix_base<T>& upper_triangular(matrix_base<T>& m) {
        return triangular(m, true);
    }

    template<expression T>
    matrix_base<T> upper_triangular(const matrix_base<T>& m) {
        return triangular(m, true);
    }

    template<expression T>
    matrix_base<T>& lower_triangular(matrix_base<T>& m) {
        return triangular(m, false);
    }

    template<expression T>
    matrix_base<T> lower_triangular(const matrix_base<T>& m) {
        return triangular(m, false);
    }

}  // namespace mmath

#endif  // MMATH_SRC_MMATH_MMATH_MATRIXALGORITHMS_HPP_
