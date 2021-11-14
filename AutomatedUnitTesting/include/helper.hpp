#pragma once

#include<cmath>

namespace aut {
template<typename T, T tolerance = 1e-4>
constexpr inline bool float_equal(const T& t1, const T& t2) {
    return std::abs(t1 - t2) < tolerance;
}


template<bool val, typename DepType>
struct Bool {
    static const bool value = val;
};

template<typename T>
struct error_print;

template<auto T>
struct error_print_val;

}