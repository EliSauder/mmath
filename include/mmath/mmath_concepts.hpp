//
// Created by Elijah Sauder on 4/19/22.
//

#ifndef MMATH_MMATH_CONCEPTS_IXX
#define MMATH_MMATH_CONCEPTS_IXX

#include <concepts>

namespace mmath {

    template<typename T>
    concept numeric = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, bool>;

    template<typename T, typename S>
    concept expression_base = numeric<S> &&
                              std::default_initializable<T> &&
                              std::copyable<T> && requires(T a) {
        { a * a };
        { a + a };
        { a - a };
    } && requires(T a, S b) {
        { a * b };
        { b * a };
        { a + b };
        { b + a };
        { b == a };
        { a == b };
        { b != a };
        { a != b };
    };

    template<typename T>
    concept expression =
    expression_base<T, signed char> &&
    expression_base<T, unsigned char> &&
    expression_base<T, wchar_t> &&
    expression_base<T, char8_t> &&
    expression_base<T, char16_t> &&
    expression_base<T, char32_t> &&
    expression_base<T, signed short int> &&
    expression_base<T, unsigned short int> &&
    expression_base<T, signed int> &&
    expression_base<T, unsigned int> &&
    expression_base<T, signed long int> &&
    expression_base<T, unsigned long int> &&
    expression_base<T, signed long long int> &&
    expression_base<T, unsigned long long int> &&
    expression_base<T, float> &&
    expression_base<T, double> &&
    expression_base<T, long double>;


    template<typename E, typename Se>
    concept subexpression = expression<E> && expression<Se> && requires(E a, Se b) {
        { a * b } -> std::convertible_to<E>;
        { b * a } -> std::convertible_to<E>;
        { a + b } -> std::convertible_to<E>;
        { b + a } -> std::convertible_to<E>;
        { a - b } -> std::convertible_to<E>;
        { b - a } -> std::convertible_to<E>;
    };

    template<typename T>
    concept expression_printable = expression<T> && requires(std::ostream& os, T a) {
        { os << a } -> std::convertible_to<std::ostream&>;
    };
}

#endif //MMATH_MMATH_CONCEPTS_IXX
