//
// Created by Elijah Sauder on 4/17/22.
//

#ifndef UNTITLED_MMATH_H
#define UNTITLED_MMATH_H

#include <concepts>
#include <vector>
#include <ostream>
#include <queue>
#include <type_traits>
#include <limits>
#include <cmath>

#include "mmath_concepts.hpp"
#include "mmath_matrixelementary.hpp"

namespace mmath {

    template<expression T>
    class matrix_base {

    private:
        size_t num_row, num_col;
        std::vector<T> data;

    protected:
        matrix_base(std::vector<T> data, size_t num_row, size_t num_col) : data(std::move(data)),
                                                                           num_row(num_row),
                                                                           num_col(num_col) {
            if (this->num_col * this->num_row != this->data.size()) {
                throw std::invalid_argument("The data provided must match the rows and cols provided.");
            }
        }

    public:
        matrix_base() : num_row(0), num_col(0), data() {};

        matrix_base(size_t rows, size_t cols) : num_row(rows), num_col(cols), data(rows * cols, 0) {};

        [[maybe_unused]]
        matrix_base(size_t rows, size_t cols, T default_value) : num_row(rows), num_col(cols),
                                                                 data(rows * cols, default_value) {};

        [[maybe_unused]]
        matrix_base(std::initializer_list<std::initializer_list<T>> il) : num_row(il.size()), num_col(0) {
            for (auto& i: il) num_col = i.size() > num_col ? i.size() : num_col;

            data.reserve(num_col * num_row);

            for (auto& x: il) {
                data.insert(data.end(), x.begin(), x.end());
                size_t number_missing_col = num_col - x.size();
                for (int i = 0; i < number_missing_col; ++i) data.insert(data.end(), T{});
            }
        };

    public:
        T operator()(size_t row, size_t column) const {
            if (row == 0 || column == 0 || row > num_row || column > num_col)
                throw std::out_of_range("Requested element lies outside of the Matrix.");
            return data[(num_col * (row - 1)) + (column - 1)];
        }

        T& operator()(size_t row, size_t column) {
            if (row == 0 || column == 0 || row > num_row || column > num_col)
                throw std::out_of_range("Requested element lies outside of the Matrix.");
            return data[(num_col * (row - 1)) + (column - 1)];
        }

    protected:
        T operator()(size_t row, size_t column, bool invert) const {
            return invert ? (*this)(column, row) : (*this)(row, column);
        }

        T& operator()(size_t row, size_t column, bool invert) {
            return invert ? (*this)(column, row) : (*this)(row, column);
        }

    public:

        matrix_base<T> operator+(const matrix_base<T>& b) const {
            if (this->is_empty() || b.is_empty())
                throw std::invalid_argument("Matrices used in addition cannot be empty");
            return add_sub_matrix(b, true);
        }

        matrix_base<T> operator-(const matrix_base<T>& b) const {
            if (this->is_empty() || b.is_empty())
                throw std::invalid_argument("Matrices used in subtraction cannot be empty");
            return add_sub_matrix(b, false);
        }

        matrix_base<T> operator*(numeric auto b) const {
            if (this->is_empty()) throw std::invalid_argument("Matrices used in multiplication cannot be empty");
            matrix_base<T> result{this->data, this->num_row, this->num_col};
            for (T& i: result.data) i = i * b;
            return result;
        }

        matrix_base<T> operator*(const matrix_base<T>& b) const {
            if (this->is_empty() || b.is_empty())
                throw std::invalid_argument("Matrices used in multiplication cannot be empty");
            if (this->num_col != b.num_row)
                throw std::domain_error("Matrix a must have the same number of columns as there are rows in matrix b");

            int number_entries_to_multiply = this->num_col;

            matrix_base<T> result{this->num_row, b.num_col};

            for (int i = 1; i <= this->num_row; i++) {
                for (int j = 1; j <= b.num_col; j++) {
                    // Multiply each value in the row (a) with each value in the column (b)
                    for (int k = 1; k <= number_entries_to_multiply; k++)
                        result(i, j) += (*this)(i, k) * b(k, j);
                }
            }

            return result;
        }

        matrix_base<T> operator^(std::integral auto b) const;


    public:
        [[nodiscard]]
        T tr() const {
            if (this->is_empty()) {
                throw std::invalid_argument("Matrix must not be empty to calculate trace");
            }
            if (!this->is_square()) {
                throw std::invalid_argument("Matrix must be square to calculate trace");
            }

            T sum = T{};

            for (int i = 1; i <= num_row; ++i) {
                sum = sum + (*this)(i, i);
            }

            return sum;
        }

        // Row operations
    private:
        void swap_axis(matrix_base<T>& new_data, size_t axis_from, size_t axis_to, m_eo::matrix_axis_type axis_type) {
            if (axis_to == axis_from) return;

            bool is_column = axis_type == m_eo::matrix_axis_type::COLUMN;

            size_t bound_to_exceed = is_column ? num_col : num_row;

            if (axis_from > bound_to_exceed || axis_to > bound_to_exceed)
                throw std::out_of_range("One of the axis requested lies outside the matrix");

            size_t loop_bound = is_column ? num_row : num_col;

            for (int i = 1; i <= loop_bound; ++i) {
                T old_value = std::move(new_data(axis_to, i, is_column));
                new_data(axis_to, i, is_column) = new_data(axis_from, i, is_column);
                new_data(axis_from, i, is_column) = std::move(old_value);
            }
        }

        void axis_add(matrix_base<T>& new_data, size_t axis_to, size_t axis_a, size_t axis_b,
                      m_eo::matrix_axis_type axis_type, bool is_sub = false) {


            bool is_column = axis_type == m_eo::matrix_axis_type::COLUMN;

            size_t bound_to_exceed = is_column ? num_col : num_row;

            // axis_to doesn't need to be checked as it is equal to either a or b
            if (axis_b > bound_to_exceed || axis_a > bound_to_exceed)
                throw std::out_of_range("One of the axis requested lies outside the matrix");

            size_t loop_bound = is_column ? num_row : num_col;

            for (int i = 1; i <= loop_bound; ++i) {
                new_data(axis_to, i, is_column) = is_sub ? new_data(axis_a, i, is_column) -
                                                           new_data(axis_b, i, is_column) :
                                                  new_data(axis_a, i, is_column) +
                                                  new_data(axis_b, i, is_column);

            }
        }

        template<expression S>
        void axis_multiply(matrix_base<T>& new_data, size_t axis_from, size_t axis_to, S&& multiplicand,
                           m_eo::matrix_axis_type axis_type) {
            if (multiplicand == 1) return;

            bool is_column = axis_type == m_eo::matrix_axis_type::COLUMN;

            size_t bound_to_exceed = is_column ? num_col : num_row;

            // axis_to doesn't need to be checked as it is equal to either a or b
            if (axis_to > bound_to_exceed)
                throw std::out_of_range("One of the axis requested lies outside the matrix");

            size_t loop_bound = is_column ? num_row : num_col;

            for (int i = 1; i <= loop_bound; ++i) {
                new_data(axis_to, i, is_column) = new_data(axis_from, i, is_column) * multiplicand;
            }
        }

        template<expression S>
        void axis_multiply_add(matrix_base<T>& new_data, size_t axis_to, size_t axis_a, size_t axis_b, S&& multiplicand,
                               bool mul_a,
                               m_eo::matrix_axis_type axis_type, bool is_sub = false) {


            bool is_column = axis_type == m_eo::matrix_axis_type::COLUMN;

            size_t bound_to_exceed = is_column ? num_col : num_row;

            // axis_to doesn't need to be checked as it is equal to either a or b
            if (axis_b > bound_to_exceed || axis_a > bound_to_exceed)
                throw std::out_of_range("One of the axis requested lies outside the matrix");

            size_t loop_bound = is_column ? num_row : num_col;

            for (int i = 1; i <= loop_bound; ++i) {

                if (is_sub) {
                    if (mul_a) {
                        new_data(axis_to, i, is_column) =
                                (multiplicand * new_data(axis_a, i, is_column)) - new_data(axis_b, i, is_column);
                        continue;
                    }
                    new_data(axis_to, i, is_column) =
                            new_data(axis_a, i, is_column) - (multiplicand * new_data(axis_b, i, is_column));
                    continue;
                }

                if (mul_a) {
                    new_data(axis_to, i, is_column) =
                            (multiplicand * new_data(axis_a, i, is_column)) + new_data(axis_b, i, is_column);
                    continue;
                }

                new_data(axis_to, i, is_column) =
                        new_data(axis_a, i, is_column) + (multiplicand * new_data(axis_b, i, is_column));
            }
        }

    public:

        template<m_eo::matrix_axis_type W>
        mmath::matrix_base<T> operator|(m_eo::matrix_axis_switch<W> axis) {
            if (this->is_empty()) {
                throw std::invalid_argument("Elementary operations cannot be performed on an empty matrix.");
            }

            matrix_base<T> new_data{this->data, this->num_row, this->num_col};
            if (axis.get_axis_from() != 0) {
                this->swap_axis(new_data, axis.get_axis_from(), axis.get_axis_to(), axis.get_type());
                return new_data;
            }

            axis_add(new_data, axis.get_axis_to(), axis.get_op_axis1(), axis.get_op_axis2(), axis.get_type(),
                     axis.is_sub());

            return new_data;
        }

        template<m_eo::matrix_axis_type W, expression S>
        mmath::matrix_base<T> operator|(m_eo::matrix_axis_switch_generic<W, S> axis) {
            if (this->is_empty()) {
                throw std::invalid_argument("Elementary operations cannot be performed on an empty matrix.");
            }

            matrix_base<T> new_data{this->data, this->num_row, this->num_col};
            if (axis.get_op_axis2() == 0) {

                axis_multiply(new_data, axis.get_op_axis1(), axis.get_axis_to(), axis.get_multiplicand(),
                              axis.get_type());
                return new_data;
            }

            axis_multiply_add(new_data, axis.get_axis_to(), axis.get_op_axis1(), axis.get_op_axis2(),
                              axis.get_multiplicand(), axis.is_mul_axis1(), axis.get_type(), axis.is_sub());
            return new_data;
        }

    public:
        [[nodiscard]]
        matrix_base<T> transpose() const {
            matrix_base<T> new_data{num_col, num_row};

            for (int i = 1; i <= num_row; ++i) {
                for (int j = 1; j <= num_col; ++j) {
                    new_data(j, i) = std::move((*this)(i, j));
                }
            }

            return new_data;
        }

    public:
        [[nodiscard]]
        bool is_square() const {
            return num_row == num_col;
        }

        [[maybe_unused]] [[nodiscard]]
        bool is_identity() const {
            if (!is_square()) return false;

            for (int i = 1; i <= num_row; ++i) {
                for (int j = 1; j <= num_col; ++j) {
                    if (i != j && (*this)(i, j) != 0) return false;
                    if (i == j && (*this)(i, j) != 1) return false;
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
            for (auto& i: this->data) {
                if (i != 0) return false;
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

                } else if (this->data[i] != m.data[i]) {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const matrix_base<T>& m) {
            return !((*this) == (m));
        }

    private:
        [[nodiscard]]
        matrix_base<T> add_sub_matrix(const matrix_base<T>& b, bool add_sub) const {
            if (this->num_col != b.num_col || this->num_row != b.num_row)
                throw std::domain_error("Matrices must be the same size");

            std::vector<T> new_data(num_col * num_row);
            for (int i = 0; i < data.size(); i++)
                new_data[i] = add_sub ? this->data[i] + b.data[i] : this->data[i] - b.data[i];

            return matrix_base<T>{std::move(new_data), num_row, num_col};
        }
    };

    template<expression T>
    matrix_base<T> operator*(numeric auto a, const matrix_base<T>& b) {
        return b * a;
    }

    template<expression T>
    matrix_base<T> make_identity_matrix(size_t size) {
        matrix_base<T> result{size, size};

        for (int i = 1; i <= size; ++i) {
            result(i, i) = 1;
        }

        return result;
    }

    template<expression T>
    matrix_base<T> matrix_base<T>::operator^(std::integral auto b) const {
        if (!is_square())
            throw std::invalid_argument("Matrix must be square to raise it to a power.");
        if (b < 0)
            throw std::invalid_argument("Power cannot be less than 1");

        if (b == 0)
            return make_identity_matrix<T>(this->num_row);

        matrix_base<T> result{this->data, this->num_row, this->num_col};

        for (int i = 1; i < b; ++i)
            result = result * (*this);

        return result;
    }

    template<expression T>
    [[maybe_unused]]
    T tr(const matrix_base<T>& matrix) {
        return matrix.tr();
    }

    template<expression T>
    matrix_base<T> transpose(const matrix_base<T>& m) {
        return m.transpose();
    }

    template<expression_printable T>
    std::ostream& operator<<(std::ostream& os, const matrix_base<T>& matrix) {
        for (int i = 1; i <= matrix.size_row(); i++) {
            for (int j = 1; j <= matrix.size_col(); j++) {
                os << matrix(i, j);
                if (j != matrix.size_col()) os << " ";
            }
            if (i != matrix.size_row()) os << std::endl;
        }

        return os;
    }
}

#endif //UNTITLED_MMATH_H
