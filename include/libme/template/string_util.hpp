#ifndef LIBME_STRING_UTIL_HPP
  #define LIBME_STRING_UTIL_HPP

#include "../bit.hpp"
#include "../allocator.hpp"
#include <string.h>

#include <type_traits>

namespace me {

  struct strnum_opts {
    const char* chars = "0123456789abcdefghijklmnopqrstuvwxyz";

    uint16_t base : 6 {10};
    uint16_t allign : 2 {1};
    uint16_t sign : 2 {1};
    uint16_t special : 1 {0};
  };

  template<typename T, class A>
  class String_T;

  template<typename T>
  class StringView_T;

  template<typename C> [[nodiscard]] constexpr size_t strlen(const C* str);
  template<typename C> [[nodiscard]] constexpr int strcmp(const C* str1, const C* str2);


  // ---------- INTEGER ---------- //

  /* converts a string to a integer */
  template<typename T, typename Char> [[nodiscard]] constexpr T strint(const Char* &str, int base = 10) requires std::is_integral_v<T>;
  template<typename T, typename Char> [[nodiscard]] constexpr T strint(const Char* &str, size_t len, int base = 10) requires std::is_integral_v<T>;

  /* creates a integer string from a integer */
  template<typename T, typename Char> constexpr void intstr(T i, Char* &str, const strnum_opts &opts) requires std::is_integral_v<T>;


  // ---------- FLOATING POINT ---------- //

  /* converts a string to a float */
  template<typename T, typename Char> [[nodiscard]] constexpr T strfloat(const Char* &str, int base = 10) requires std::is_floating_point_v<T>;
  template<typename T, typename Char> [[nodiscard]] constexpr T strfloat(const Char* &str, size_t len, int base = 10) requires std::is_floating_point_v<T>;

  /* creates a float string from a float */
  template<typename T, typename Char> constexpr void floatstr(T f, Char* &str, const strnum_opts &opts) requires std::is_floating_point_v<T>;


  /* creates a number string from a number */
  template<typename T, typename Char> constexpr void numstr(T n, Char* &str, const strnum_opts &opts);

  // --------- STUFF --------- //
  template<typename Char> [[nodiscard]] constexpr Char lowercase(Char chr);
  template<typename Char> [[nodiscard]] constexpr Char uppercase(Char chr);

}

template<typename C, class A = me::Allocator> [[nodiscard]] constexpr me::String_T<C, A> operator+(const me::String_T<C, A> &_str1, const me::String_T<C, A> &_str2);
template<typename C, class A = me::Allocator> [[nodiscard]] constexpr me::String_T<C, A> operator+(const C* _str1, const me::String_T<C, A> &_str2);

#include "string.hpp"
#include "string_view.hpp"

template<typename C>
constexpr me::size_t me::strlen(const C* _str)
{
  size_t _len = 0;
  while (*_str++ != '\0')
    _len++;
  return _len;
}

template<typename C>
constexpr int me::strcmp(const C* _str1, const C* _str2)
{
  while (true)
  {
    if (*_str1 == '\0' && *_str2 == '\0')
      return 0;
    else if (*_str1 != *_str2)
      return 1;
    _str1++;
    _str2++;
  }
  return 0;
}

template<typename T, typename Char>
constexpr T me::strint(const Char* &str, int base) requires std::is_integral_v<T>
{
  T i = 0;

  while (isdigit(*str))
    i = base * i + *str++ - '0';
  return i;
}

template<typename T, typename Char>
constexpr T me::strint(const Char* &str, size_t len, int base) requires std::is_integral_v<T>
{
  T i = 0;

  while (len-- && isdigit(*str))
    i = base * i + *str++ - '0';
  return i;
}

template<typename T, typename Char>
constexpr void me::intstr(T i, Char* &str, const strnum_opts &opts) requires std::is_integral_v<T>
{
  Char temp[66];
  int index = 0;
  bool unsign;

  if (i == 0)
  {
    *str++ = '0';
    return str;
  }

  if constexpr (std::is_signed_v<T>)
  {
    if (i < 0)
    {
      unsign = true;
      i = -i;
    }else
      unsign = false;
  }else
    unsign = false;

  while (i)
  {
    T r = i % opts.base;
    i = i / opts.base;

    temp[index++] = opts.chars[r];
  }

  if (unsign && opts.sign)
    *str++ = '-';
  while (index)
    *str++ = temp[(index--) - 1];
}

template<typename T, typename Char>
constexpr void me::floatstr(T f, Char* &str, const strnum_opts &opts) requires std::is_floating_point_v<T>
{
  typedef long TI;

  Char dec_point = '.';

  T i1 = static_cast<TI>(f);
  T i2 = static_cast<TI>(f - static_cast<T>(i1));

  str = intstr(i1, str, opts);
  *str++ = dec_point;
  intstr(i2, str, opts);
}

template<typename T, typename Char>
constexpr void me::numstr(T n, Char* &str, const strnum_opts &opts)
{
  if constexpr (std::is_integral<T>::value)
    intstr(n, str, opts);
  else if constexpr (std::is_floating_point<T>::value)
    floatstr(n, str, opts);
}

template<typename C>
constexpr C me::lowercase(C chr)
{
  if (chr >= 0x41 && chr <= 0x5A)
    return chr + 0x20;
  return chr;
}

template<typename C>
constexpr C me::uppercase(C chr)
{
  if (chr >= 0x61 && chr <= 0x7A)
    return chr - 0x20;
  return chr;
}

template<typename C, class A>
constexpr me::String_T<C, A> operator+(const me::String_T<C, A> &str1, const me::String_T<C, A> &str2)
{
  me::String_T<C> str = str1;
  str += str2;
  return str;
}

template<typename C, class A>
constexpr me::String_T<C, A> operator+(const C* str1, const me::String_T<C, A> &str2)
{
  me::String_T<C> str = str1;
  str += str2;
  return str;
}

#endif
