#ifndef LIBME_STRING_UTIL_HPP
  #define LIBME_STRING_UTIL_HPP

#include "../bit.hpp"

#include <type_traits>

namespace me {

  template<typename T>
  class _string_t;

  template<typename T>
  class _string_view_t;

  template<typename C> [[nodiscard]] size_t strlen(const C* _str);
  template<typename C> [[nodiscard]] int strcmp(const C* _str1, const C* _str2);


  // ---------- INTEGER ---------- //

  /* converts a string to a integer */
  template<typename T, typename C> [[nodiscard]] T strint(const C* _str, int _base = 10) requires std::is_integral_v<T>;
  template<typename T, typename C> [[nodiscard]] T strint(const C* _str, size_t _len, int _base = 10) requires std::is_integral_v<T>;

  /* creates a integer string from a integer */
  template<typename T, typename C> void intstr(T _i, C* _str, int _base = 10) requires std::is_integral_v<T>;


  // ---------- FLOATING POINT ---------- //

  /* converts a string to a float */
  template<typename T, typename C> [[nodiscard]] T strfloat(const C* _str, int _base = 10) requires std::is_floating_point_v<T>;
  template<typename T, typename C> [[nodiscard]] T strfloat(const C* _str, size_t _len, int _base = 10) requires std::is_floating_point_v<T>;

  /* creates a float string from a float */
  template<typename T, typename C> void floatstr(T _f, C* _str, int _base = 10) requires std::is_floating_point_v<T>;


  /* creates a number string from a number */
  template<typename T, typename C> void numstr(T _n, C* _str, int _base = 10);

}

template<typename C> [[nodiscard]] inline me::_string_t<C> operator+(const me::_string_t<C> &_str1, const me::_string_t<C> &_str2);
template<typename C> [[nodiscard]] inline me::_string_t<C> operator+(const C* _str1, const me::_string_t<C> &_str2);

[[nodiscard]] inline me::_string_t<char> operator "" _s(const char* _str, me::size_t _len);
[[nodiscard]] inline me::_string_view_t<char> operator "" _sv(const char* _str, me::size_t len);

#include "string.hpp"
#include "string_view.hpp"

template<typename C>
me::size_t me::strlen(const C* _str)
{
  size_t _len = 0;
  while (*_str++ != '\0')
    _len++;
  return _len;
}

template<typename C>
int me::strcmp(const C* _str1, const C* _str2)
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
T me::strint(const C* _str, int _base) requires std::is_integral_v<T>
{
  T _i = 0;

  while (isdigit(*_str))
    _i = _base * _i + *_str++ - '0';
  return _i;
}

template<typename T, typename C>
T me::strint(const C* _str, size_t _len, int _base) requires std::is_integral_v<T>
{
  T _i = 0;

  while (_len-- && isdigit(*_str))
    _i = _base * _i + *_str++ - '0';
  return _i;
}

template<typename T, typename C>
void me::intstr(T _i, C* _str, int _base) requires std::is_integral_v<T>
{
  static const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
void __me__floatstr__(TF _f, C* _str, int _base)
{
  static const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
void me::floatstr(T _f, C* _str, int _base) requires std::is_floating_point_v<T>
{
  if constexpr (std::is_same<T, float>::value)
    __me__floatstr__<T, int, C>(_f, _str, _base);
  else if constexpr (std::is_same<T, double>::value)
    __me__floatstr__<T, long, C>(_f, _str, _base);
  else
    __me__floatstr__<T, long long, C>(_f, _str, _base);
}

template<typename T, typename C>
void me::numstr(T _n, C* _str, int _base)
{
  if constexpr (std::is_integral<T>::value)
    intstr(_n, _str, _base);
  else if (std::is_floating_point<T>::value)
    floatstr(_n, _str, _base);
}

template<typename C>
inline me::_string_t<C> operator+(const me::_string_t<C> &_str1, const me::_string_t<C> &_str2)
{
  me::_string_t<C> _str = _str1;
  _str.reserve(_str1.size() + _str2.size() + 1);
  _str += _str2;
  return _str;
}

template<typename C>
inline me::_string_t<C> operator+(const C* _str1, const me::_string_t<C> &_str2)
{
  me::size_t _len = me::strlen(_str1);

  me::_string_t<C> _str = _str1;
  _str.reserve(_len + _str2.size() + 1);
  _str += _str2;
  return _str;
}

inline me::_string_t<char> operator "" _s(const char* _str, me::size_t _len)
{
  return me::_string_t<char>(_str, _len);
}

inline me::_string_view_t<char> operator "" _sv(const char* _str, me::size_t _len)
{
  return me::_string_view_t<char>(_str, _len);
}

#endif
