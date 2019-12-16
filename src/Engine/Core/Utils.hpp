#ifndef MOTEUR_DE_JEUX_SRC_CORE_UTILS_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_UTILS_HPP

#include <type_traits>
#include <functional>
#include <utility>

namespace _detail {

  template<template<typename...> class base, typename derived>
  struct is_base_of_template_impl {
    template<typename... Ts>
    static constexpr std::true_type test(const base<Ts...> *);

    static constexpr std::false_type test(...);

    using type = decltype(test(std::declval<derived *>()));
  };

}

template < template <typename...> class base,typename derived>
using is_base_of_template = typename _detail::is_base_of_template_impl<base,derived>::type;

template < template <typename...> class base,typename derived>
using is_base_of_template_v = typename _detail::is_base_of_template_impl<base,derived>::type::value;


#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_UTILS_HPP */
