#pragma once

#include "constraint_proxy.hpp"

namespace aut {

/**
 * @brief Combines two constraints with AND condition.
 * @tparam A 
 * @tparam B 
*/
template<typename A, typename B> requires is_constrained<A> && is_constrained<B>
struct _and : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;

    constexpr bool is_valid() const { return A{ this->m_t }.is_valid() && B{ this->m_t }.is_valid(); }
};

/**
 * @brief Combines two constraints with OR condition.
 * @tparam A
 * @tparam B
*/
template<typename A, typename B> requires is_constrained<A> && is_constrained<B>
struct _or : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;

    constexpr bool is_valid() const { return A{ this->m_t }.is_valid() || B { this->m_t }.is_valid(); }
};

/**
 * @brief Negates a given constraint.
 * @tparam A
*/
template<typename A> requires is_constrained<A>
struct _not : public constraint_proxy<typename A::value_type> {
    using constraint_proxy<typename A::value_type>::constraint_proxy;

    constexpr bool is_valid() const { return !A{ this->m_t }.is_valid(); }
};

}