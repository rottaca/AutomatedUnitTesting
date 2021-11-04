#pragma once

#include "constraint_proxy.hpp"

namespace aut {

template<typename A, typename B> requires is_constrained<A> && is_constrained<B>
struct _and : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;
};

template<typename A, typename B> requires is_constrained<A> && is_constrained<B>
struct _or : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;
};

template<typename A> requires is_constrained<A>
struct _not : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;
};

}