#pragma once

#include <type_traits>
#include <functional>
#include <concepts>

namespace aut {

    template <typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;

    template<typename T> requires Numeric<T>
    struct constraint_proxy;

    template<typename T, typename U = std::remove_cvref_t<T>>
    concept is_constrained = std::derived_from<U, constraint_proxy<typename U::value_type>>;

    template<typename T, typename U>
    concept at_least_one_constrained = is_constrained<T> || is_constrained<U>;

    template<auto VAL, typename T>
    concept is_numeric_and_same_type = Numeric<decltype(VAL)> && std::same_as<decltype(VAL), T>;

    /**
     * @brief Constraint base class which acts as a proxy for the internal data.
     * 
     * The constraint proxy class can be used similar to an object of type T. All common operators are overloaded to forward the
     * operation to the underlying data.
     * 
     * @tparam T Type of internal numeric value.
    */
    template<typename T> requires Numeric<T>
    struct constraint_proxy {
        using value_type = std::remove_cvref_t<T>;

        /**
         * @brief Constructor which create an instance of the proxy class.
         * 
         * The data is passed by reference, but internally stored by value.
         * @param t Data to be wrapped in the proxy class.
        */
        constexpr constraint_proxy(const T& t) noexcept(std::is_nothrow_constructible_v<T>) : m_t(t) {}

        /**
         * @brief Default-constructor is deleted, as an initialization of the wrapped data is necessary
        */
        constraint_proxy() = delete;

        constraint_proxy(constraint_proxy<T>&& other) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
        constraint_proxy<T>& operator=(constraint_proxy<T>&& other) noexcept(std::is_nothrow_move_assignable_v<T>) = default;

        constraint_proxy(const constraint_proxy<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
        constraint_proxy<T>& operator=(const constraint_proxy<T>& other) noexcept(std::is_nothrow_copy_assignable_v<T>) = default;


        /**
         * @brief Pre-increment operator
         * @return
        */
        constexpr inline auto& operator++() {
            ++m_t;
            return *this;
        }

        /**
         * @brief Post-increment operator
         * @return
        */
        constexpr inline auto& operator++(int) {
            constraint_proxy<T> tmp(*this);
            operator++();
            return tmp;
        }
        /**
         * @brief Pre-decrement operator
         * @return 
        */
        constexpr inline auto& operator--() {
            --m_t;
            return *this;
        }
        /**
         * @brief Post-decrement operator
         * @return
        */
        constexpr inline auto& operator--(int) {
            constraint_proxy<T> tmp(*this);
            operator--();
            return tmp;
        }

        /**
         * @brief Implicit cast operator in order to avoid manual 
         *        casting when using the proxy inside a calculation.
        */
        constexpr operator T() const {
            return m_t;
        }

        /**
         * @brief The wrapped data.
        */
        T m_t;
    };

/**
 * @brief Wrapper implementation for operations between proxy and non-proxy data.
 * 
 * The constexpr if statement is evaluated at compile time and
 * decides how the operation needs to be performed.
 * 
 * @tparam T2 Type of the left hand data for the operator
 * @tparam U2 Type of the left hand data for the operator
 * @tparam OP Template-template type which defines the operator. E.g. std::plus
 * @param lhs Const reference to the left data
 * @param rhs Const reference to the right data
 * @return Returns the result from executing "lhs OP rhs"
*/
template<typename T2, typename U2, template <typename> class OP>
constexpr inline auto op_wrapper(const T2& lhs, const U2& rhs) {
    if constexpr (is_constrained<T2>) {
        if constexpr (is_constrained<U2>) {
            OP<typename std::remove_cvref_t<T2>::value_type> op;
            return op(lhs.m_t, rhs.m_t);
        }
        else {
            OP<typename std::remove_cvref_t<T2>::value_type> op;
            return op(lhs.m_t, rhs);
        }
    }
    else {
        if constexpr (is_constrained<U2>) {
            OP<typename std::remove_cvref_t<U2>::value_type> op;
            return op(lhs, rhs.m_t);
        }
        else {
            OP<T2> op;
            return op(lhs, rhs);
        }
    }
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto operator+(const T2& lhs, const U2& rhs) {
    return op_wrapper < T2, U2, std::plus > (lhs, rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto operator-(const T2& lhs, const U2& rhs) {
    return op_wrapper < T2, U2, std::minus > (lhs, rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto operator*(const T2& lhs, const U2& rhs) {
    return op_wrapper < T2, U2, std::multiplies > (lhs, rhs);
}
template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto operator/(const T2& lhs, const U2& rhs) {
    return op_wrapper < T2, U2, std::divides > (lhs, rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto& operator+=(T2& lhs, const U2& rhs) {
    if constexpr (is_constrained<T2>) {
        if constexpr (is_constrained<U2>) {
            return lhs.m_t += rhs.m_t;
        }
        else {
            return lhs.m_t += rhs;
        }
    }
    else {
        if constexpr (is_constrained<U2>) {
            return lhs += rhs.m_t;
        }
        else {
            return lhs += rhs;
        }
    }
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto& operator-=(T2& lhs, const U2& rhs) {
    if constexpr (is_constrained<T2>) {
        if constexpr (is_constrained<U2>) {
            return lhs.m_t -= rhs.m_t;
        }
        else {
            return lhs.m_t -= rhs;
        }
    }
    else {
        if constexpr (is_constrained<U2>) {
            return lhs -= rhs.m_t;
        }
        else {
            return lhs -= rhs;
        }
    }
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto& operator*=(T2& lhs, const U2& rhs) {
    if constexpr (is_constrained<T2>) {
        if constexpr (is_constrained<U2>) {
            return lhs.m_t *= rhs.m_t;
        }
        else {
            return lhs.m_t *= rhs;
        }
    }
    else {
        if constexpr (is_constrained<U2>) {
            return lhs *= rhs.m_t;
        }
        else {
            return lhs *= rhs;
        }
    }
}
template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline auto& operator/=(T2& lhs, const U2& rhs) {
    if constexpr (is_constrained<T2>) {
        if constexpr (is_constrained<U2>) {
            return lhs.m_t /= rhs.m_t;
        }
        else {
            return lhs.m_t /= rhs;
        }
    }
    else {
        if constexpr (is_constrained<U2>) {
            return lhs /= rhs.m_t;
        }
        else {
            return lhs /= rhs;
        }
    }
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator==(T2&& lhs, U2&& rhs) {
    return op_wrapper < T2, U2, std::equal_to > (lhs, rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator!=(T2&& lhs, U2&& rhs) {
    return !(lhs == rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator<(T2&& lhs, U2&& rhs) {
    return op_wrapper < T2, U2, std::less > (lhs, rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator>(T2&& lhs, U2&& rhs) {
    return rhs < lhs;
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator>=(T2&& lhs, U2&& rhs) {
    return !(lhs < rhs);
}

template<typename T2, typename U2> requires at_least_one_constrained<T2, U2>
constexpr inline bool operator<=(T2&& lhs, U2&& rhs) {
    return !(lhs > rhs);
}
}