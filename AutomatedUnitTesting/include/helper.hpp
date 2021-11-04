#pragma once

#include<cmath>

namespace aut {
template<typename T, T tolerance = 1e-4>
constexpr inline bool float_equal(const T& t1, const T& t2) {
    return std::abs(t1 - t2) < tolerance;
}

template<auto T>
struct error_print;

}