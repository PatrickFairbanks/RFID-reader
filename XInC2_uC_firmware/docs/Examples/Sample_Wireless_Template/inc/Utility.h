/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_UTILITY_H
#define SKAA_COMMON_UTILITY_H

//! Interface macro for declaration of a static interface
#define INTERFACE(Name) template <typename Concrete> class Name

//! Concrete macro for declaration of a static implementation of an interface
#define CONCRETE(Name, Interface) class Name : public Interface < Name >

//! Static dispatch macro for calling functions in the conrete implementations
#define STATIC_DISPATCH(a) static_cast<Concrete*>(this)-> a
#define STATIC_CONST_DISPATCH(a) static_cast<const Concrete*>(this)-> a

#define STATIC_DISPATCH_CONST(a) static_cast<Concrete const *>(this)-> a

//! Useful macro for supressing compiler warnings
#define USED(x) ((void)(x))

#ifdef __XINC2__
//! Only enable these asserts if we are compiling for XInC2
#define xinc2_assert(a) assert(a)
#else
//! Empty assert in case of hardware-less testing
#define xinc2_assert(a)
#endif

#ifdef assert
#define assert_unreachable() assert(false && "Unreachable code detected!")
#else
#define assert_unreachable() while (true) {}
#endif

template <typename RetTy>
inline RetTy constexpr_unreachable() {
  assert_unreachable();
  return {}; 
}

#endif
