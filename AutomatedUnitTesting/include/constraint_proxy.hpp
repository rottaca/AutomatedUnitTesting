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

    template<typename T> requires Numeric<T>
    struct constraint_proxy {
        using value_type = std::remove_cvref_t<T>;

        constexpr constraint_proxy(const T& t) noexcept(std::is_nothrow_constructible_v<T>) : m_t(t) {}
        constraint_proxy() noexcept(std::is_nothrow_constructible_v<T>) = default;
        ~constraint_proxy() = default;

        constraint_proxy(constraint_proxy<T>&& other) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
        constraint_proxy<T>& operator=(constraint_proxy<T>&& other) noexcept(std::is_nothrow_move_assignable_v<T>) = default;

        constraint_proxy(const constraint_proxy<T>& other) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
        constraint_proxy<T>& operator=(const constraint_proxy<T>& other) noexcept(std::is_nothrow_copy_assignable_v<T>) = default;


        constexpr inline auto& operator++() {
            m_t++;
            return *this;
        }
        constexpr inline auto& operator++(int) {
            constraint_proxy<T> tmp(*this);
            operator++();
            return tmp;
        }

        constexpr inline auto& operator--() {
            m_t--;
            return *this;
        }
        constexpr inline auto& operator--(int) {
            constraint_proxy<T> tmp(*this);
            operator--();
            return tmp;
        }

        // Allow implciit cast to type T 
        constexpr operator T() const {
            return m_t;
        }

        T m_t;
    };


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