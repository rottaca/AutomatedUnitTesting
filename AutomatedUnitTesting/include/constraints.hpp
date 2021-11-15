#pragma once

#include "constraint_proxy.hpp"
#include <array>
#include <iostream>

namespace aut {
    template<auto THRESHOLD, typename T = decltype(THRESHOLD)> requires is_numeric_and_same_type<THRESHOLD, T>
        struct less : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t < THRESHOLD; }
    };

    template<auto THRESHOLD, typename T> requires Numeric<T>
    std::ostream& operator<<(std::ostream& os, const less<THRESHOLD, T>& data)
    {
        os << data.m_t << " (< " << THRESHOLD << " )";
        return os;
    }

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct greater : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t > THRESHOLD; }
    };

    template<auto THRESHOLD, typename T> requires Numeric<T>
    std::ostream& operator<<(std::ostream& os, const greater<THRESHOLD, T>& data)
    {
        os << data.m_t << " (> " << THRESHOLD << " )";
        return os;
    }

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct greater_eq : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t >= THRESHOLD; }
    };

    template<auto THRESHOLD, typename T> requires Numeric<T>
    std::ostream& operator<<(std::ostream& os, const greater_eq<THRESHOLD, T>& data)
    {
        os << data.m_t << " (>= " << THRESHOLD << " )";
        return os;
    }

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct less_eq : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t <= THRESHOLD; }
    };

    template<auto THRESHOLD, typename T> requires Numeric<T>
    std::ostream& operator<<(std::ostream& os, const less_eq<THRESHOLD, T>& data)
    {
        os << data.m_t << " (<= " << THRESHOLD << " )";
        return os;
    }

    template<auto MIN, auto MAX, typename T = decltype(MIN)> requires is_numeric_and_same_type<MIN, T>&&
        is_numeric_and_same_type<MAX, T>
        struct in_range : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t >= MIN && this->m_t <= MAX; }
    };

    template<auto MIN, auto MAX, typename T> requires Numeric<T>
    std::ostream& operator<<(std::ostream& os, const in_range<MIN, MAX, T>& data)
    {
        os << data.m_t << " (in [" << MIN << ", " << MAX << "])";
        return os;
    }

    template<auto Option0, auto ... Options>
        struct one_of : public constraint_proxy<decltype(Option0)> {
        using constraint_proxy<decltype(Option0)>::constraint_proxy;


        constexpr bool is_valid() const { 
            constexpr std::array<decltype(Option0), 1+sizeof...(Options)> options = {Option0, Options...};

            for (const auto o : options) {
                if (this->m_t == o) return true;
            }
            return false;
        }
    };

    template<auto Option0, auto ... Options>
    std::ostream& operator<<(std::ostream& os, const one_of<Option0, Options...>& data)
    {
        os << data.m_t << " (one of [" << Option0;
        ((os << ", " << Options), ...);
        os << "])";

        return os;
    }
}