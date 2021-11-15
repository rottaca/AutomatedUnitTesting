#pragma once

#include <tuple>
#include <iostream>

#include "helper.hpp"
#include "evaluation.hpp"

namespace aut {

template<typename T>
struct get_func_def {
    static_assert(Bool<false, T>::value, "Provided type is not a valid function!");
};

template<typename RetType, typename ... Args>
struct get_func_def<RetType(Args...)> {
    using return_type = RetType;
    using arg_types = std::tuple<Args...>;

    get_func_def(RetType(*)(Args...)){}
};

template<typename RetType, typename ... Args>
get_func_def(RetType(*)(Args...))->get_func_def<RetType(Args...)>;


template<typename ... Args>
struct create_test_instances {};

template<typename Arg0, typename ... Args>
struct create_test_instances<Arg0, Args...> {
    //static const std::tuple<> args = { evaluate<Args>::valid_border_values[0] };
};

template<typename Arg0>
struct create_test_instances<Arg0> {
    static const std::tuple<Arg0> args = { evaluate<Arg0>::valid_border_values[0] };
};

template<typename ...Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}

template<auto func, typename RetType, typename T>
struct gen_testcases;

template<auto func, typename RetType, template<typename...> typename C, typename... Args>
struct gen_testcases<func, RetType, C<Args...>> {
    gen_testcases() {
        
        //const auto res = std::apply([](auto&&... args) {return func((evaluate<args>::valid_border_values[0] << ...)); }, Args{});

        std::cout << "inputs: " << std::endl;
        printer(Args{ evaluate<Args>::valid_border_values[0] } ...);

        auto const result = func(Args{ evaluate<Args>::valid_border_values[0] } ...);

        //std::cout << "input: " << evaluate<Arg0>::valid_border_values[0] << (evaluate<Args>::valid_border_values[0] << ...) << std::endl;
        std::cout << "output:" << std::endl;
        std::cout << result << std::endl;
    }
}; 


template<auto func, typename T = decltype(func)>
struct test_func {
    using ret_type = typename decltype(get_func_def(func))::return_type;
    using arg_types = typename decltype(get_func_def(func))::arg_types;

    test_func() {
        static_assert(is_constrained<ret_type>, "Function must have a constrained return type!");

        gen_testcases<func, ret_type, arg_types>{};

        //const auto results = std::apply(func, args);
    }   
};

}