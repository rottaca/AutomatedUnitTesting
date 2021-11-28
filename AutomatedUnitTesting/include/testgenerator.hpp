#pragma once

#include <tuple>
#include <iostream>

#include "helper.hpp"
#include "evaluation.hpp"

namespace aut {
namespace detail {

template<typename T>
struct parse_signature {
    static_assert(Bool<false, T>::value, "Provided type is not a valid function!");
};

template<typename RetType, typename ... Args>
struct parse_signature<RetType(Args...)> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;

    parse_signature(RetType(*)(Args...)){}
};

template<typename RetType, typename ... Args>
parse_signature(RetType(*)(Args...))->parse_signature<RetType(Args...)>;

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
void exec_tests(Func func, Arg0 arg0, Args ...  argVals) {
    for (const auto v : arg0) {
        auto const newF = [v, func](auto &&... args) {return func(v, args...); };
        exec_tests(newF, argVals...);
    }
}
template<typename Func>
void exec_tests(Func func) {
    std::cout << "-----" << std::endl;
    auto const res = func();
    std::cout << "-----" << std::endl;

    if (!res.is_valid()) {
        std::cout << "FAILED, output = " << res << std::endl;
    }
    else {
        std::cout << "PASSED, output = " << res << std::endl;
    }
    std::cout << "-----" << std::endl;
}

template<auto func, typename ...T, size_t... Is>
void call_with_arg_candidates_impl(const std::tuple<T...>& tuple, std::index_sequence<Is...>) {
    exec_tests(func, std::get<Is>(tuple) ...);
}

template<auto func, typename ...T>
void call_with_arg_candidates(const std::tuple<T...>& tuple) {
    call_with_arg_candidates_impl<func>(tuple, std::make_index_sequence<sizeof...(T)>{});
}

template<auto func, typename RetType, typename T>
struct gen_testcases;

template<auto func, typename RetType, template<typename...> typename C, typename... Args>
struct gen_testcases<func, RetType, C<Args...>> {
    gen_testcases() {
        constexpr auto arg_value_candidates = std::make_tuple(evaluate<Args>::valid_border_values ...);
        const size_t num_tests = (std::size(evaluate<Args>::valid_border_values) * ...);
        std::cout << "Generating " << num_tests << " tests!" << std::endl;
        std::cout << "Valid border values per argument are: " << std::endl;
        print_arg_candidates(arg_value_candidates);

        call_with_arg_candidates<func>(arg_value_candidates);
    }
}; 
}

template<auto func>
struct test_func {
    using func_def = decltype(detail::parse_signature(func));
    using ret_type = typename func_def::return_type;
    using arg_types = typename func_def::arg_types;

    test_func() {
        static_assert(is_constrained<ret_type>, "Function must have a constrained return type!");
        detail::gen_testcases<func, ret_type, arg_types>{};
    }   
};

}