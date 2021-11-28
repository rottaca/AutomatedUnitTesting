#pragma once

#include<cmath>
#include <chrono>
#include <algorithm>
#include <numeric>

namespace aut {
template<typename T, T tolerance = 1e-4>
constexpr inline bool float_equal(const T& t1, const T& t2) {
    return std::abs(t1 - t2) < tolerance;
}


using Duration = std::chrono::duration<double, std::milli>;
template<typename Func, size_t iterations = 10000>
void measure_runtime(Func&& func) {
    std::array< Duration, iterations> durations;
    for (size_t i = 0; i < iterations; i++)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        func();
        auto t2 = std::chrono::high_resolution_clock::now();

        durations[i] = t2 - t1;
    }
    const Duration avgDuration = std::accumulate(durations.begin(), durations.end(), Duration{ 0 }) / iterations;
    const auto minmax = std::minmax_element(durations.begin(), durations.end());
    const Duration minDuration = *minmax.first;
    const Duration maxDuration = *minmax.second;

    std::cout << "Measured runtime over " << iterations << " iterations:" << std::endl;
    std::cout << "Average: " << avgDuration.count() << " ms" << std::endl;
    std::cout << "Minimum: " << minDuration.count() << " ms" << std::endl;
    std::cout << "Maximum: " << maxDuration.count() << " ms" << std::endl;
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