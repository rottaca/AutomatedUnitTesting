#pragma once

#include <tuple>

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
get_func_def(RetType(*)(Args...))->get_func_def<RetType, Args...>;


template<auto func, typename T = decltype(func)>
struct generate_tests {
    using ret_type = typename decltype(get_func_def(func))::return_type;
    using arg_types = typename decltype(get_func_def(func))::arg_types;


    generate_tests(){}
};

}