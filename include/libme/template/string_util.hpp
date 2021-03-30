#ifndef LIBME_STRING_UTIL_HPP
  #define LIBME_STRING_UTIL_HPP

#include "../type.hpp"

namespace me {

  template<typename T>
  class _string_t;

  template<typename T>
  class _string_view_t;

  template<typename C>
  [[nodiscard]] size_t strlen(const C* _str);

  template<typename C>
  [[nodiscard]] int strcmp(const C* _str1, const C* _str2);

  /* TODO: wide string */
  template<typename C>
  [[nodiscard]] int8_t strint8(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] int16_t strint16(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] int32_t strint32(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] int64_t strint64(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] uint8_t struint8(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] uint16_t struint16(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] uint32_t struint32(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] uint64_t struint64(const _string_view_t<C> &_str, const uint8_t _base = 10);

  template<typename C>
  [[nodiscard]] float strfloat(const _string_view_t<C> &_str);

  template<typename C>
  [[nodiscard]] double strdouble(const _string_view_t<C> &_str);

}

template<typename C>
[[nodiscard]] inline me::_string_t<C> operator+(const me::_string_t<C> &_str1, const me::_string_t<C> &_str2);

template<typename C>
[[nodiscard]] inline me::_string_t<C> operator+(const C* _str1, const me::_string_t<C> &_str2);

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

/* TODO: wide string */
template<typename C>
me::int8_t me::strint8(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<int8_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::int16_t me::strint16(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<int16_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::int32_t me::strint32(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<int32_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::int64_t me::strint64(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<int64_t>(strtoll(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::uint8_t me::struint8(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<uint8_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::uint16_t me::struint16(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<uint16_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::uint32_t me::struint32(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<uint32_t>(strtol(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
me::uint64_t me::struint64(const _string_view_t<C> &_str, const uint8_t _base)
{
  C _temp[_str.size() + 1];
  return static_cast<uint64_t>(strtoll(_str.c_str(_temp), nullptr, _base));
}

template<typename C>
float me::strfloat(const _string_view_t<C> &_str)
{
  C _temp[_str.size() + 1];
  return static_cast<uint64_t>(strtof(_str.c_str(_temp), nullptr));
}

template<typename C>
double me::strdouble(const _string_view_t<C> &_str)
{
  C _temp[_str.size() + 1];
  return static_cast<uint64_t>(strtod(_str.c_str(_temp), nullptr));
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
