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

template<typename Base, typename T>
inline bool isInstanceOf(const T* ptr) {
  return dynamic_cast<const Base*>(ptr) != nullptr;
}

template<typename T, T _first, T _last>
class EnumRange
{
public:
  class Iterator {
  public:
    explicit Iterator(int value)
      : m_value{value}
    { }

    T operator*() const {
      return (T)m_value;
    }

    void operator++() {
      ++m_value;
    }

    bool operator!=(Iterator rhs) {
      return m_value != rhs.m_value;
    }

  private:
    int m_value;
  };

};

template <typename T, T _first, T _last>
typename EnumRange<T, _first, _last>::Iterator begin(EnumRange<T, _first, _last>)
{
  return typename EnumRange<T, _first, _last>::Iterator((int)_first);
}

template <typename T, T _first, T _last>
typename EnumRange<T, _first, _last>::Iterator end(EnumRange<T, _first, _last>)
{
  return typename EnumRange<T, _first, _last>::Iterator(((int)_last) + 1);
}


#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_UTILS_HPP */
