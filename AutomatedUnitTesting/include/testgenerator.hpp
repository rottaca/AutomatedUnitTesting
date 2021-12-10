#pragma once

#include <tuple>
#include <iostream>
#include <utility>

#include "helper.hpp"
#include "evaluation.hpp"

namespace aut {
namespace detail {

template<typename T>
struct parse_signature: public parse_signature<decltype(&T::operator())> {};

template<typename RetType, typename ... Args>
struct parse_signature<RetType(Args...)> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;
};

template<typename RetType, typename ... Args>
struct parse_signature<RetType(*)(Args...)> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;
};


template<typename ClassType, typename RetType, typename ... Args>
struct parse_signature<RetType(ClassType::*)(Args...)> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;
};

template<typename ClassType, typename RetType, typename ... Args>
struct parse_signature<RetType(ClassType::*)(Args...) const> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;
};

template<typename ...Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}

template<typename T, auto sz>
void print_array(const std::array<T, sz>& arr) {
    for (size_t i = 0; i < sz; i++) {
        std::cout << arr.at(i);
        if (i < (sz-1)) std::cout << ", ";
    }
    std::cout << std::endl;
}

template<typename ...T, size_t... Is>
void print_arg_candidates_impl(const std::tuple<T...>& tuple, std::index_sequence<Is...>) {
    ((std::cout << "Values for Argument " << Is << ": ", print_array(std::get<Is>(tuple))), ...);
}

template<typename ...T>
void print_arg_candidates(const std::tuple<T...>& tuple) {
    print_arg_candidates_impl(tuple, std::make_index_sequence<sizeof...(T)>{});
}

template<typename Func, typename Arg0, typename ... Args>
void exec_tests(Func& func, bool debug_prints, Arg0 arg0, Args ...  argVals) {
    // TODO not very nice
    for (const auto v : arg0) {
        auto const newF = [v, func](auto &&... args) {return func(v, args...); };
        exec_tests(newF, debug_prints, argVals...);
    }
}
template<typename Func>
void exec_tests(Func& func, bool debug_prints) {
    if (debug_prints) std::cout << "-----" << std::endl;
    auto const res = func();
    if (debug_prints) std::cout << "-----" << std::endl;

    if (!res.is_valid()) {
        std::cout << "FAILED, output = " << res << std::endl;
    }
    else {
        if (debug_prints) std::cout << "PASSED, output = " << res << std::endl;
    }
    if (debug_prints) std::cout << "-----" << std::endl;
}

template<typename Func, typename ...T, size_t... Is>
void call_with_arg_candidates_impl(Func& func, const std::tuple<T...>& tuple, std::index_sequence<Is...>, bool debug_prints) {
    exec_tests(func, debug_prints, std::get<Is>(tuple) ...);
}

template<typename Func, typename ...T>
void call_with_arg_candidates(Func& func, const std::tuple<T...>& tuple, bool debug_prints) {
    call_with_arg_candidates_impl(func, tuple, std::make_index_sequence<sizeof...(T)>{}, debug_prints);
}

template<typename Func, typename RetType, typename T>
struct gen_testcases;

template<typename Func, typename RetType, template<typename...> typename C, typename... Args>
struct gen_testcases<Func, RetType, C<Args...>> {
    gen_testcases(Func& func, bool debug_prints) {
        constexpr auto arg_value_candidates = std::make_tuple(evaluate<Args>::valid_border_values ...);
        const size_t num_tests = (std::size(evaluate<Args>::valid_border_values) * ...);
        std::cout << "Generating " << num_tests << " tests!" << std::endl;
        if (debug_prints) {
            std::cout << "Valid border values per argument are: " << std::endl;
            print_arg_candidates(arg_value_candidates);
        }

        call_with_arg_candidates(func, arg_value_candidates, debug_prints);
    }
}; 
}

template<typename Func>
struct test_func {
    using func_def = detail::parse_signature<Func>;
    using ret_type = typename func_def::return_type;
    using arg_types = typename func_def::arg_types;


    test_func(Func& func, bool debug_prints=false) {
        static_assert(is_constrained<ret_type>, "Function must have a constrained return type!");
        detail::gen_testcases<Func, ret_type, arg_types>{func, debug_prints};
    }   
};

}