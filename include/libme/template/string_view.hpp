#ifndef LIBME_STRING_VIEW_TYPE_HPP
  #define LIBME_STRING_VIEW_TYPE_HPP

#include "../type.hpp"

#include <stdarg.h>

namespace me {

  template<typename T>
  class _string_t;

  template<typename T>
  class _string_view_t {

  protected:

    typedef size_t _Size;
    typedef const T* _Iterator;
    typedef T _Char;
    typedef T* _String;
    typedef const T* _Const_String;

    _Iterator _begin; /* first character */
    _Iterator _end; /* last character + 1 */

  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;

    _string_view_t(_Const_String _string, _Size _length);
    _string_view_t(_Iterator _begin, _Iterator _end);
    _string_view_t(_Const_String _string);
    _string_view_t(const _string_t<T> &_string);
    _string_view_t(const _string_view_t<T> &_copy);
    _string_view_t(_string_view_t<T> &&_move);
    _string_view_t();

    [[nodiscard]] _Iterator data() const;
    [[nodiscard]] _Iterator begin() const;
    [[nodiscard]] _Iterator end() const;

    template<typename I> [[nodiscard]] I as_int(_Size _off = 0, int _base = 10) const;
    void split(_Char _delimiter, _Size &_len, _string_view_t<T>* _strs) const;

    [[nodiscard]] _Char at(_Size _pos) const;

    [[nodiscard]] _Size find(_Char _chr, _Size _off = 0) const;
    [[nodiscard]] _Size rfind(_Char _chr, _Size _off = 0) const;
    [[nodiscard]] _Size find(const _string_view_t &_str, _Size _off = 0) const;
    [[nodiscard]] _Size rfind(const _string_view_t &_str, _Size _off = 0) const;
    [[nodiscard]] _string_view_t substr(_Size _begin, _Size _end) const;
    [[nodiscard]] _string_view_t substr(_Size _begin) const;
    void copy(_Size _off, _Size _len, _String _dst) const;

    _String c_str(_String _str, _Size _off) const;
    _String c_str(_String _str) const;
    [[nodiscard]] _Size size() const;
    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] _Char operator[](_Size _pos) const;

    [[nodiscard]] bool operator==(const _string_view_t &_str) const;
    [[nodiscard]] bool operator==(_Const_String _str) const;

    _string_view_t& operator=(const _string_view_t &_copy);
    _string_view_t& operator=(_string_view_t &&_move);

  };

}

#include "string.hpp"
#include "string_util.hpp"

template<typename T>
me::_string_view_t<T>::_string_view_t(_Const_String _string, _Size _length)
  : _begin(_string), _end(_string + _length)
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t(_Iterator _begin, _Iterator _end)
  : _begin(_begin), _end(_end)
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t(_Const_String _string)
  : _string_view_t(_string, strlen(_string))
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t(const _string_t<T> &_string)
  : _string_view_t(_string.data(), _string.size())
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t(const _string_view_t<T> &_copy)
  : _begin(_copy._begin), _end(_copy._end)
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t(_string_view_t<T> &&_move)
  : _begin(_move._begin), _end(_move._end)
{
}

template<typename T>
me::_string_view_t<T>::_string_view_t()
  : _begin(nullptr), _end(nullptr)
{
}

template<typename T>
const T* me::_string_view_t<T>::data() const
{
  return this->_begin;
}

template<typename T>
const T* me::_string_view_t<T>::begin() const
{
  return this->_begin;
}

template<typename T>
const T* me::_string_view_t<T>::end() const
{
  return this->_end;
}

template<typename T>
template<typename I>
I me::_string_view_t<T>::as_int(_Size _off, int _base) const
{
  return strint<I>(this->_begin + _off, this->size(), _base);
}

template<typename T>
T me::_string_view_t<T>::at(_Size _pos) const
{
  return this->_begin[_pos];
}

template<typename T>
void me::_string_view_t<T>::split(_Char _delimiter, _Size &_len, _string_view_t<T>* _strs) const
{
  // TODO: need testing
  if (_strs == nullptr)
  {
    _len = 1;
    for (_Iterator i = this->_begin; *i != nullchar; i++)
    {
      if (*i == _delimiter)
	_len++;
    }
  }else
  {
    _Iterator _first = this->_begin;
    for (_Iterator i = this->_begin; *i != nullchar; i++)
    {
      if (*i == _delimiter)
      {
	*_strs++ = _string_view_t<T>(_first, i);
	_first = i + 1;
      }
    }

    *_strs++ = _string_view_t<T>(_first, this->_end);
  }
}

template<typename T>
me::size_t me::_string_view_t<T>::find(_Char _chr, _Size _off) const
{
  _Iterator _iter = this->_begin + _off;
  while (_iter != this->_end)
  {
    if (_chr == *_iter++)
      return _iter;
  }
  return npos;
}

template<typename T>
me::size_t me::_string_view_t<T>::rfind(_Char _chr, _Size _off) const
{
  _Iterator _iter = this->_end - _off;
  while (_iter != this->_begin)
  {
    if (_chr == *_iter--)
      return _iter;
  }
  return npos;
}

template<typename T>
me::size_t me::_string_view_t<T>::find(const _string_view_t<T> &_str, _Size _off) const
{
  for (_Iterator i = this->_begin + _off; i != this->_end; i++)
  {
    bool match = true;

    _Iterator k = i;
    for (_Iterator j = _str.begin(); j != _str.end(); j++)
    {
      if (*k == nullchar || *j != *k)
      {
	match = false;
	break;
      }
      k++;
    }

    if (match)
      return i - this->_begin;
  }
  return npos;
}

template<typename T>
me::size_t me::_string_view_t<T>::rfind(const _string_view_t<T> &_str, _Size _off) const
{
  for (_Iterator i = this->_end - 1 - _off; i != this->_begin - 1; i++)
  {
    bool match = true;

    _Iterator k = i;
    for (_Iterator j = _str.begin(); j != _str.end(); j++)
    {
      if (*k == nullchar || *j != *k)
      {
	match = false;
	break;
      }
      k--;
    }

    if (match)
      return i - this->_begin;
  }
  return npos;
  return npos;
}

template<typename T>
me::_string_view_t<T> me::_string_view_t<T>::substr(_Size _off, _Size _end) const
{
  return _string_view_t<T>(this->_begin + _off, this->_begin + _off + _end);
}

template<typename T>
me::_string_view_t<T> me::_string_view_t<T>::substr(_Size _off) const
{
  return _string_view_t<T>(this->_begin + _off, this->_end);
}

template<typename T>
void me::_string_view_t<T>::copy(_Size _off, _Size _len, _String _dest) const
{
  for (_Size i = 0; i < _len; i++)
    _dest[i] = this->_begin[i + _off];
}

template<typename T>
T* me::_string_view_t<T>::c_str(_String _str, _Size _off) const
{
  _Size _len = this->size() - _off;

  for (_Size i = 0; i < _len; i++)
    _str[i] = this->_begin[i + _off];

  _str[_len] = nullchar;
  return _str;
}

template<typename T>
T* me::_string_view_t<T>::c_str(_String _str) const
{
  for (_Size i = 0; i < this->size(); i++)
    _str[i] = this->_begin[i];

  _str[this->size()] = nullchar;
  return _str;
}

template<typename T>
me::size_t me::_string_view_t<T>::size() const
{
  return this->_end - this->_begin;
}

template<typename T>
bool me::_string_view_t<T>::is_empty() const
{
  return this->_begin != this->_end;
}

template<typename T>
bool me::_string_view_t<T>::is_valid() const
{
  return this->_begin != nullptr;
}

template<typename T>
T me::_string_view_t<T>::operator[](_Size _pos) const
{
  return this->at(_pos);
}

template<typename T>
bool me::_string_view_t<T>::operator==(const _string_view_t &_str) const
{
  if (this->size() != _str.size())
    return false;

  const _Char* _iter1 = this->_begin;
  const _Char* _iter2 = _str._begin;
  while (_iter1 != this->_end)
  {
    if (*_iter1++ != *_iter2++)
      return false;
  }
  return true;
}

template<typename T>
bool me::_string_view_t<T>::operator==(_Const_String _str) const
{
  size_t _str_len = strlen(_str);
  if (this->size() != _str_len)
    return false;

  const _Char* _iter = this->_begin;
  while (_iter != this->_end)
  {
    if (*_iter++ != *_str++)
      return false;
  }
  return true;
}

template<typename T>
me::_string_view_t<T>& me::_string_view_t<T>::operator=(const _string_view_t<T> &_str)
{
  this->_begin = _str._begin;
  this->_end = _str._end;
  return *this;
}

template<typename T>
me::_string_view_t<T>& me::_string_view_t<T>::operator=(_string_view_t<T> &&_str)
{
  this->_begin = static_cast<_string_view_t<T>&&>(_str)._begin;
  this->_end = static_cast<_string_view_t<T>&&>(_str)._end;
  return *this;
}
#endif
