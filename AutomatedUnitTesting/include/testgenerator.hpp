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


template<auto func, typename ... Args>
struct create_test_instances {
    static constexpr std::tuple<decltype(evaluate<Args>::valid_border_values)...> arg_value_candidates = std::make_tuple(evaluate<Args>::valid_border_values ... );
    //static const std::vector<std::tuple<Args...>> test_args = std::make_tuple(Args...);
};


template<typename ...Args>
void printer(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}

template<typename ...T, auto ...Sz>
void printer(std::array<T, Sz>... args) {
    constexpr auto f = [](auto arr) {
        for (auto v : arr) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    };
    
    (f(args),...);
}

template<typename ...T, size_t... Is>
void printTupleImpl(const std::tuple<T...>& tuple, std::index_sequence<Is...>) {
    ((std::cout << "Arg " << Is << ": ", printer(std::get<Is>(tuple))), ...);
}

template<typename ...T>
void printTuple(const std::tuple<T...>& tuple) {
    printTupleImpl(tuple, std::make_index_sequence<sizeof...(T)>{});
}



template<auto func, typename ...T, size_t... Is>
void instantiate_tests(const std::tuple<T...>& tuple, std::index_sequence<Is...>) {



    ((std::cout << "Arg " << Is << ": ", printer(std::get<Is>(tuple))), ...);
}

template<auto func, typename ...T>
void instantiate_tests(const std::tuple<T...>& tuple) {
    printTupleImpl(tuple, std::make_index_sequence<sizeof...(T)>{});
}


template<auto func, typename RetType, typename T>
struct gen_testcases;

template<auto func, typename RetType, template<typename...> typename C, typename... Args>
struct gen_testcases<func, RetType, C<Args...>> {
    gen_testcases() {
        static constexpr auto arg_value_candidates = std::make_tuple(evaluate<Args>::valid_border_values ...);

        std::cout << "Valid border values per argument: " << std::endl;
        printTuple(arg_value_candidates);

        instantiate_tests<func>(arg_value_candidates);

        //auto const result = func(Args{ evaluate<Args>::valid_border_values[0] } ...);

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