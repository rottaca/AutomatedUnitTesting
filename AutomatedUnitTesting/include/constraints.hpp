#pragma once

#include "constraint_proxy.hpp"
#include <array>

namespace aut {
    template<auto THRESHOLD, typename T = decltype(THRESHOLD)> requires is_numeric_and_same_type<THRESHOLD, T>
        struct less : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t < THRESHOLD; }
    };

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct greater : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t > THRESHOLD; }
    };

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct greater_eq : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t >= THRESHOLD; }
    };

    template<auto THRESHOLD, typename T = decltype(THRESHOLD)>  requires is_numeric_and_same_type<THRESHOLD, T>
        struct less_eq : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t <= THRESHOLD; }
    };

    template<auto MIN, auto MAX, typename T = decltype(MIN)> requires is_numeric_and_same_type<MIN, T>&&
        is_numeric_and_same_type<MAX, T>
        struct in_range : public constraint_proxy<T> {
        using constraint_proxy<T>::constraint_proxy;

        constexpr bool is_valid() const { return this->m_t >= MIN && this->m_t <= MAX; }
    };

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
}