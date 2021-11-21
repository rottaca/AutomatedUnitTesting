#pragma once

#include "constraints.hpp"
#include "constraint_combiner.hpp"
#include <array>
#include <algorithm>

namespace aut {

template<auto values>
constexpr auto remove_duplicates() {
    constexpr auto new_sz = [] {
        size_t sz = 0;
        for (size_t i = 0; i < std::size(values); i++)
        {
            bool duplicate = false;
            for (size_t j = 0; j < i; j++)
            {
                if (values[i] == values[j]) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) sz++;
        }
        return sz;
    }();

    std::array<typename decltype(values)::value_type, new_sz> unified{};
    size_t curr_sz = 0;
    for (const auto v : values) {
        bool duplicate = false;
        for (size_t i = 0; i < curr_sz; i++) {
            if (unified[i] == v) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) unified[curr_sz++] = v;
    }
    return unified;
}

template<typename A, typename B> requires is_constrained<A>&& is_constrained<B>
constexpr auto border_value_union() {
    constexpr auto values_a = evaluate<A>::valid_border_values;
    constexpr auto values_b = evaluate<B>::valid_border_values;

    std::array<typename A::value_type, std::size(values_a)+std::size(values_b)> merged_values{};
    size_t sz = 0;
    for (const auto va : values_a) {
        merged_values[sz] = va;
        sz++;
    }
    for (const auto vb : values_b) {
        merged_values[sz] = vb;
        sz++;
    }
    return merged_values;
}

template<typename A, typename B> requires is_constrained<A>&& is_constrained<B>
    constexpr auto border_value_intersect() {
        constexpr auto values_a = evaluate<A>::valid_border_values;
        constexpr auto values_b = evaluate<B>::valid_border_values;

        constexpr auto cnt = [values_a, values_b]() constexpr {
            size_t sz = 0;
            for (const auto va : values_a) {
                if (!A{ va }.is_valid()) continue;
                if (!B{ va }.is_valid()) continue;
                sz++;
            }
            for (const auto vb : values_b) {
                if (!A{ vb }.is_valid()) continue;
                if (!B{ vb }.is_valid()) continue;
                sz++;
            }
            return sz;
        } ();

        std::array<typename A::value_type, cnt> merged_values{};
        size_t sz = 0;
        for (const auto va : values_a) {
            if (!A{ va }.is_valid()) continue;
            if (!B{ va }.is_valid()) continue;
            merged_values[sz] = va;
            sz++;
        }
        for (const auto vb : values_b) {
            if (!A{ vb }.is_valid()) continue;
            if (!B{ vb }.is_valid()) continue;
            merged_values[sz] = vb;
            sz++;
        }
        return merged_values;
    }


template<typename T>
struct evaluate {
    static_assert(std::is_same_v<T, void>, "No evaluation implementation available for the provided type.");
};

template<auto THRESHOLD, typename T>
struct evaluate<less<THRESHOLD, T>> {
    using value_type = T;
    static constexpr std::array<T, 1> valid_border_values { THRESHOLD  - 1 };
};

template<auto THRESHOLD, typename T>
struct evaluate<greater<THRESHOLD, T>> {
    using value_type = T;
    static constexpr std::array<T, 1> valid_border_values { THRESHOLD +1 };
};

template<auto THRESHOLD, typename T>
struct evaluate<greater_eq<THRESHOLD, T>> {
    using value_type = T;
    static constexpr std::array<T, 1> valid_border_values { THRESHOLD };
};

template<auto THRESHOLD, typename T>
struct evaluate<less_eq<THRESHOLD, T>> {
    using value_type = T;
    static constexpr std::array<T, 1> valid_border_values { THRESHOLD };
};

template<auto MIN, auto MAX, typename T>
struct evaluate<in_range<MIN, MAX, T>> {
    using value_type = T;
    static constexpr std::array<T, 2> valid_border_values { MIN, MAX };
};

template<auto Option0, auto ... Options>
struct evaluate<one_of<Option0, Options...>> {
    using value_type = decltype(Option0);
    static constexpr std::array<decltype(Option0), (1+sizeof...(Options))> valid_border_values { Option0, Options... };
};

template<typename A, typename B>
struct evaluate<_and<A, B>> {
    using value_type = typename A::value_type;
    static constexpr auto valid_border_values = remove_duplicates<border_value_intersect<A, B>()>();
};

template<typename A, typename B>
struct evaluate<_or<A, B>> {
    using value_type = typename A::value_type;
    static constexpr auto valid_border_values = remove_duplicates<border_value_union<A, B>()>();
};

}

