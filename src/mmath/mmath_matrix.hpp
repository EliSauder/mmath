//
// Created by Elijah Sauder on 4/26/22.
//

#ifndef MMATH_SRC_MMATH_MMATH_MATRIX_HPP_
#define MMATH_SRC_MMATH_MMATH_MATRIX_HPP_

#include <cmath>
#include <cstddef>
#include <exception>
#include <limits>
#include <ostream>
#include <vector>

#include "mmath_concepts.hpp"

namespace mmath {

    template<expression T>
    class matrix_base {
    private:
        size_t num_row, num_col;
        std::vector<T> data;

    protected:
        matrix_base(std::vector<T> data, size_t num_row, size_t num_col)
                : data(std::move(data)), num_row(num_row), num_col(num_col) {
            if (this->num_col * this->num_row != this->data.size()) {
                throw std::invalid_argument("The data provided must match the rows and cols provided.");
            }
        }

    public:
        matrix_base() : num_row(0), num_col(0), data() {
        };

        matrix_base(size_t rows, size_t cols) : num_row(rows), num_col(cols), data(rows * cols, 0) {
        };

        [[maybe_unused]]
        matrix_base(size_t rows, size_t cols, T default_value)
                : num_row(rows), num_col(cols), data(rows * cols, default_value) {
        };

        [[maybe_unused]]
        matrix_base(std::initializer_list<std::initializer_list<T>> il)
                : num_row(il.size()), num_col(0) {
            for (auto& i : il) {
                num_col = i.size() > num_col ? i.size() : num_col;
            }

            data.reserve(num_col * num_row);

            for (auto& x : il) {
                data.insert(data.end(), x.begin(), x.end());
                size_t number_missing_col = num_col - x.size();
                for (int i = 0; i < number_missing_col; ++i) {
                    data.insert(data.end(), T{ });
                }
            }
        };

    public:
        T operator()(size_t row, size_t column) const {
            if (row == 0 || column == 0 || row > num_row || column > num_col) {
                throw std::out_of_range("Requested element lies outside of the Matrix.");
            }

            return data[(num_col * (row - 1)) + (column - 1)];
        }

        T& operator()(size_t row, size_t column) {
            if (row == 0 || column == 0 || row > num_row || column > num_col) {
                throw std::out_of_range("Requested element lies outside of the Matrix.");
            }

            return data[(num_col * (row - 1)) + (column - 1)];
        }

    public:
        T operator()(size_t row, size_t column, bool invert) const {
            return invert ? (*this)(column, row) : (*this)(row, column);
        }

        T& operator()(size_t row, size_t column, bool invert) {
            return invert ? (*this)(column, row) : (*this)(row, column);
        }

    public:
        [[nodiscard]]
        bool is_square() const {
            return num_row == num_col;
        }

        [[maybe_unused]]
        [[nodiscard]]
        bool is_identity() const {
            if (!is_square()) {
                return false;
            }

            for (int i = 1; i <= num_row; ++i) {
                for (int j = 1; j <= num_col; ++j) {
                    if (i != j && (*this)(i, j) != 0) {
                        return false;
                    }
                    if (i == j && (*this)(i, j) != 1) {
                        return false;
                    }
                }
            }

            return true;
        }

        [[nodiscard]]
        bool is_empty() const {
            return num_row == 0 || num_col == 0;
        }

        [[nodiscard]]
        bool is_zero() const {
            for (auto& i : this->data) {
                if (i != 0) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]]
        size_t size_col() const {
            return this->num_col;
        }

        [[nodiscard]]
        size_t size_row() const {
            return this->num_row;
        }

        bool operator==(const matrix_base<T>& m) const {
            if (this->size_row() != m.size_row() || this->size_col() != m.size_col()) {
                return false;
            }

            for (int i = 0; i < num_col * num_row; ++i) {
                if (std::is_floating_point<T>()) {
                    // TODO: Separate floating point comparison into separate header
                    if (std::abs(this->data[i] - m.data[i]) > std::numeric_limits<T>::epsilon()) {
                        return false;
                    }

                }
                else if (this->data[i] != m.data[i]) {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const matrix_base<T>& m) const {
            return !((*this) == (m));
        }
    };

    template<expression T>
    matrix_base<T> make_identity_matrix(size_t size) {
        matrix_base<T> result{ size, size };

        for (int i = 1; i <= size; ++i) {
            result(i, i) = 1;
        }

        return result;
    }

    template<expression T>
    matrix_base<T>& make_identity_matrix(matrix_base<T>& m) {
        if (!m.is_square()) {
            throw std::invalid_argument("Matrix must be a square to make it an identity matrix");
        }

        for (int i = 1; i <= m.size_row(); ++i) {
            for (int j = 1; j <= m.size_col(); ++j) {
                m(i, j) = i == j ? 1 : 0;
            }
        }

        return m;
    }

    namespace impl {
        template<expression T>
        matrix_base<T>& add_sub_matrix(matrix_base<T>& a, const matrix_base<T>& b, bool is_sub) {
            if (a.size_col() != b.size_col() || a.size_row() != b.size_row()) {
                throw std::domain_error("Matrices must be the same size");
            }

            for (int i = 1; i <= a.size_row(); i++) {
                for (int j = 1; j <= a.size_col(); j++) {
                    a(i, j) = is_sub ? std::move(a(i, j)) - b(i, j) : std::move(a(i, j)) + b(i, j);
                }
            }

            return a;
        }
    }  // namespace impl

    template<expression T>
    matrix_base<T>& operator+(matrix_base<T>& a, const matrix_base<T>& b) {
        if (a.is_empty() || b.is_empty()) {
            throw std::invalid_argument("Matrices used in addition cannot be empty");
        }
        return impl::add_sub_matrix(a, b, false);
    }

    template<expression T>
    matrix_base<T> operator+(const matrix_base<T>& a, const matrix_base<T>& b) {
        matrix_base<T> new_matrix = a;
        return new_matrix + b;
    }

    template<expression T>
    matrix_base<T>& operator-(matrix_base<T>& a, const matrix_base<T>& b) {
        if (a.is_empty() || b.is_empty()) {
            throw std::invalid_argument("Matrices used in subtraction cannot be empty");
        }
        return impl::add_sub_matrix(a, b, true);
    }

    template<expression T>
    matrix_base<T> operator-(const matrix_base<T>& a, const matrix_base<T>& b) {
        matrix_base<T> new_matrix = a;
        return new_matrix - b;
    }

    template<expression T>
    matrix_base<T>& operator*(matrix_base<T>& a, numeric auto b) {
        if (a.is_empty()) {
            throw std::invalid_argument("Matrices used in multiplication cannot be empty");
        }

        for (int i = 1; i <= a.size_row(); ++i) {
            for (int j = 1; j <= a.size_col(); ++j) {
                a(i, j) = std::move(a(i, j)) * b;
            }
        }

        return a;
    }

    template<expression T>
    matrix_base<T> operator*(const matrix_base<T>& a, numeric auto b) {
        matrix_base<T> new_matrix = a;
        return new_matrix * b;
    }

    template<expression T>
    matrix_base<T>& operator*(numeric auto a, matrix_base<T>& b) {
        if (b.is_empty()) {
            throw std::invalid_argument("Matrices used in multiplication cannot be empty");
        }

        for (int i = 1; i <= b.size_row(); ++i) {
            for (int j = 1; j <= b.size_col(); ++j) {
                b(i, j) = std::move(b(i, j)) * a;
            }
        }

        return b;
    }

    template<expression T>
    matrix_base<T> operator*(numeric auto a, const matrix_base<T>& b) {
        matrix_base<T> new_matrix = b;
        return a * new_matrix;
    }

    template<expression T>
    matrix_base<T> operator*(const matrix_base<T>& a, const matrix_base<T>& b) {
        if (a.is_empty() || b.is_empty()) {
            throw std::invalid_argument("Matrices used in multiplication cannot be empty");
        }
        if (a.size_col() != b.size_row()) {
            throw std::domain_error("Matrix a must have the same number of columns as there are rows in "
                                    "matrix b");
        }

        int number_entries_to_multiply = a.size_col();

        matrix_base<T> result{ a.size_row(), b.size_col() };

        // TODO: Implement faster algorithm for multiplication
        for (int i = 1; i <= a.size_row(); i++) {
            for (int j = 1; j <= b.size_col(); j++) {
                // Multiply each value in the row (a) with each value in the column (b)
                for (int k = 1; k <= number_entries_to_multiply; k++) {
                    result(i, j) += a(i, k) * b(k, j);
                }
            }
        }

        return result;
    }

    template<expression T>
    matrix_base<T>& operator^(matrix_base<T>& a, std::integral auto b) {
        if (!a.is_square()) {
            throw std::invalid_argument("Matrix must be square to raise it to a power.");
        }
        if (b < 0) {
            throw std::invalid_argument("Power cannot be less than 1");
        }

        if (b == 0) {
            return make_identity_matrix<T>(a);
        }

        matrix_base<T> tmp_copy = a;

        for (int i = 1; i < b; ++i) {
            a = a * tmp_copy;
        }

        return a;
    }

    template<expression T>
    matrix_base<T> operator^(const matrix_base<T>& a, std::integral auto b) {
        matrix_base<T> new_matrix = a;
        return new_matrix ^ b;
    }

}  // namespace mmath

#endif  // MMATH_SRC_MMATH_MMATH_MATRIX_HPP_
