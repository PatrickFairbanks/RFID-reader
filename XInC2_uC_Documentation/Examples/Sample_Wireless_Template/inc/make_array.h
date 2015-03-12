/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_MAKE_ARRAY_H_
#define SKAA_COMMON_MAKE_ARRAY_H_

#include <type_traits>
#include <utility>
#include <array>
// Just imagine that std::forward were required to be constexpr...

template <class T>
constexpr T &&cforward(typename std::remove_reference<T>::type &t) {
  return static_cast<T &&>(t);
}
template <class T>
constexpr T &&cforward(typename std::remove_reference<T>::type &&t) {
  static_assert(!std::is_lvalue_reference<T>::value,
                "T must not be an lvalue-reference");
  return static_cast<T &&>(t);
}
template <class T, class... U> struct all_convertible_to;
template <class T> struct all_convertible_to<T> : std::true_type {};
template <class T, class U>
struct all_convertible_to<T, U> : std::is_convertible<U, T>::type {};
template <class T, class U, class... R>
struct all_convertible_to<T, U, R...>
    : std::conditional<std::is_convertible<U, T>::value,
                       all_convertible_to<T, R...>,
                       std::false_type>::type::type {};
// This tag type is used to detect whether we perform automatic
// type deduction, else the type is user-provided:
struct auto_element_type_t;

template <class T, class...> struct deduce_element_type { typedef T type; };

template <class... T> struct deduce_element_type<auto_element_type_t, T...> {
  typedef typename std::decay<typename std::common_type<T...>::type>::type type;
};
// Sfinae protection for the empty sequence case: User code
// is required to provide the destination type in this case:
template <> struct deduce_element_type<auto_element_type_t> {};

template <class T, class... U> struct array_creator {
  static constexpr std::array<T, sizeof...(U)> create(U &&... u) {
    return std::array<T, sizeof...(U)>{{static_cast<T>(cforward<U>(u))...}};
  }
};
template <class T> struct array_creator<T> {
  static constexpr std::array<T, 0> create() { return std::array<T, 0>(); }
};

template <class D = auto_element_type_t, class... T>
constexpr auto make_array(T &&... t)
    -> std::array<typename deduce_element_type<D, T...>::type, sizeof...(T)> {
  typedef typename deduce_element_type<D, T...>::type U;
  static_assert(all_convertible_to<U, T...>::value,
                "All argument types must be implicitly convertible to the "
                "element type of std::array");
  return array_creator<U, T...>::create(cforward<T>(t)...);
}

#endif
