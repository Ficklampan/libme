#ifndef LIBME_STRING_UTIL_HPP
  #define LIBME_STRING_UTIL_HPP

#include "../bit.hpp"
#include "../allocator.hpp"
#include <string.h>

#include <type_traits>

namespace me {

  template<typename T, class A>
  class string_t;

  template<typename T>
  class string_view_t;

  template<typename C> [[nodiscard]] constexpr size_t strlen(const C* str);
  template<typename C> [[nodiscard]] constexpr int strcmp(const C* str1, const C* str2);


  // ---------- INTEGER ---------- //

  /* converts a string to a integer */
  template<typename T, typename C> [[nodiscard]] constexpr T strint(const C* str, int base = 10) requires std::is_integral_v<T>;
  template<typename T, typename C> [[nodiscard]] constexpr T strint(const C* str, size_t len, int base = 10) requires std::is_integral_v<T>;

  /* creates a integer string from a integer */
  template<typename T, typename C> constexpr void intstr(T i, C* str, int base = 10) requires std::is_integral_v<T>;


  // ---------- FLOATING POINT ---------- //

  /* converts a string to a float */
  template<typename T, typename C> [[nodiscard]] constexpr T strfloat(const C* str, int base = 10) requires std::is_floating_point_v<T>;
  template<typename T, typename C> [[nodiscard]] constexpr T strfloat(const C* str, size_t len, int base = 10) requires std::is_floating_point_v<T>;

  /* creates a float string from a float */
  template<typename T, typename C> constexpr void floatstr(T f, C* str, int base = 10) requires std::is_floating_point_v<T>;


  /* creates a number string from a number */
  template<typename T, typename C> constexpr void numstr(T n, C* str, int base = 10);

  // --------- STUFF --------- //
  template<typename C> [[nodiscard]] constexpr C lowercase(C chr);
  template<typename C> [[nodiscard]] constexpr C uppercase(C chr);

}

template<typename C, class A = me::allocator> [[nodiscard]] constexpr me::string_t<C, A> operator+(const me::string_t<C, A> &_str1, const me::string_t<C, A> &_str2);
template<typename C, class A = me::allocator> [[nodiscard]] constexpr me::string_t<C, A> operator+(const C* _str1, const me::string_t<C, A> &_str2);

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

template<typename T, typename C>
constexpr T me::strint(const C* _str, int _base) requires std::is_integral_v<T>
{
  T _i = 0;

  while (isdigit(*_str))
    _i = _base * _i + *_str++ - '0';
  return _i;
}

template<typename T, typename C>
constexpr T me::strint(const C* _str, size_t _len, int _base) requires std::is_integral_v<T>
{
  T _i = 0;

  while (_len-- && isdigit(*_str))
    _i = _base * _i + *_str++ - '0';
  return _i;
}

template<typename T, typename C>
constexpr void me::intstr(T _i, C* _str, int _base) requires std::is_integral_v<T>
{
  constexpr const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  C _temp[66];
  int _index = 0;
  bool _unsigned;

  if constexpr (std::is_signed_v<T>)
  {
    if (_i < 0)
    {
      _unsigned = true;
      _i = -_i;
    }else
      _unsigned = false;
  }else
    _unsigned = false;

  while (_i)
  {
    T _r = _i % _base;
    _i = _i / _base;

    _temp[_index++] = _CHARS[_r];
  }

  if (_unsigned)
    *_str++ = '-';
  while (_index)
    *_str++ = _temp[(_index--) - 1];
}

template<typename TF, typename TI, typename C>
constexpr void __me__floatstr__(TF _f, C* _str, int _base)
{
  constexpr const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  TI _i1 = static_cast<TI>(_f);
  TI _i2 = static_cast<TI>(_f - static_cast<TF>(_i1));

  C _dec_point = '.';
  C _temp[128];
  int _index = 0;
  bool _unsigned;

  if (_i1 < 0)
  {
    _unsigned = true;
    _i1 = -_i1;
  }else
    _unsigned = false;

  while (_i1)
  {
    int _r = _i1 % _base;
    _i1 = _i1 / _base;
    _temp[_index++] = _CHARS[_r];
  }

  _temp[_index++] = _dec_point;

  while (_i2)
  {
    int _r = _i2 % _base;
    _i2 = _i2 / _base;
    _temp[_index++] = _CHARS[_r];
  }

  if (_unsigned)
    *_str++ = '-';
  while (_index)
    *_str++ = _temp[(_index--) - 1];
}

template<typename T, typename C>
constexpr void me::floatstr(T _f, C* _str, int _base) requires std::is_floating_point_v<T>
{
  if constexpr (std::is_same<T, float>::value)
    __me__floatstr__<T, int, C>(_f, _str, _base);
  else if constexpr (std::is_same<T, double>::value)
    __me__floatstr__<T, long, C>(_f, _str, _base);
  else
    __me__floatstr__<T, long long, C>(_f, _str, _base);
}

template<typename T, typename C>
constexpr void me::numstr(T _n, C* _str, int _base)
{
  if constexpr (std::is_integral<T>::value)
    intstr(_n, _str, _base);
  else if (std::is_floating_point<T>::value)
    floatstr(_n, _str, _base);
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
constexpr me::string_t<C, A> operator+(const me::string_t<C, A> &str1, const me::string_t<C, A> &str2)
{
  me::string_t<C> str = str1;
  str += str2;
  return str;
}

template<typename C, class A>
constexpr me::string_t<C, A> operator+(const C* str1, const me::string_t<C, A> &str2)
{
  me::string_t<C> str = str1;
  str += str2;
  return str;
}

#endif
