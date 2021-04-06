#ifndef LIBME_STRING_TYPE_HPP
  #define LIBME_STRING_TYPE_HPP

#define LIBME_STRING_VALIDATE 1

#include "../type.hpp"
#include "../allocator.hpp"
#include "../memory.hpp"

#include <string.h>
#include <stdarg.h>

namespace me {

  template<typename T>
  class _string_view_t;

  template<typename T>
  class _string_t {
    
  protected:

    typedef allocator _Alloc_Type;
    typedef size_t _Size;
    typedef T* _Iterator;
    typedef const T* _Const_Iterator;
    typedef T _Char;
    typedef T* _String;
    typedef const T* _Const_String;

    _Iterator _begin; // pointer to the first character
    _Iterator _end; // pointer to the nullchar
    _Iterator _capacity;
    _Alloc_Type* _alloc;
  
  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;
  
    _string_t(_Const_String _string, _Size _length, _Alloc_Type* _alloc = allocator::_default());
    _string_t(_Char _char, _Size _length, _Alloc_Type* _alloc = allocator::_default());
    _string_t(_Const_String _string, _Alloc_Type* _alloc = allocator::_default());
    _string_t(const _string_view_t<T> &_string, _Alloc_Type* _alloc = allocator::_default());
    _string_t(const _string_t &_copy);
    _string_t(_string_t &&_move);
    _string_t(_Alloc_Type* _alloc = allocator::_default());
    ~_string_t();
  
    [[nodiscard]] _Iterator data() const;
    [[nodiscard]] _Iterator begin() const;
    [[nodiscard]] _Iterator end() const;
    [[nodiscard]] _Const_Iterator cbegin() const;
    [[nodiscard]] _Const_Iterator cend() const;
    void reserve(_Size _capacity);
 
    //void assign(const _string_view_t<T> &_str);
    _string_t& insert(_Size _pos, const _string_view_t<T> &_str);
    _string_t& append(const _string_view_t<T> &_str);
    _string_t& append(_Char _chr);
    _string_t& push_back(_Char _chr);
    _Char pop_back();
    void erase(_Iterator _first, _Iterator _last);
    void erase(_Iterator _pos);
    void clear();
    void replace(_Iterator _first, _Iterator _last, const _string_view_t<T> &_str);
    void to_lowercase(_Iterator _begin, _Iterator _end);
    void to_uppercase(_Iterator _begin, _Iterator _end);

    template<typename I> [[nodiscard]] I as_int(_Size _off = 0, uint8_t _base = 10) const;
    void split(_Char _delimiter, _Size &_len, _string_view_t<T>* _strs) const;

    [[nodiscard]] _Char& at(_Size _pos) const;

    [[nodiscard]] bool starts_with(const _string_view_t<T> &_str) const;
    [[nodiscard]] bool ends_with(const _string_view_t<T> &_str) const;
    [[nodiscard]] _Size find(_Char _chr, _Size _off = 0) const;
    [[nodiscard]] _Size rfind(_Char _chr, _Size _off = 0) const;
    [[nodiscard]] _Size find(const _string_view_t<T> &_str, _Size _off = 0) const;
    [[nodiscard]] _Size rfind(const _string_view_t<T> &_str, _Size _off = 0) const;
    [[nodiscard]] _string_view_t<T> substr(_Size _begin, _Size _end) const;
    [[nodiscard]] _string_view_t<T> substr(_Size _begin) const;
    void copy(_Size _off, _Size _len, _String _dst) const;
  
    [[nodiscard]] _Const_String c_str() const;
    [[nodiscard]] _Size capacity() const;
    [[nodiscard]] _Size size() const;
    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] const _Char& operator[](_Size _pos) const;

    [[nodiscard]] bool operator==(const _string_t &_str) const;
    [[nodiscard]] bool operator==(const _string_view_t<T> &_str) const;
    [[nodiscard]] bool operator==(const _String _str) const;
  
    _string_t& operator+=(_Char _chr);
    _string_t& operator+=(const _string_view_t<T> &_str);
  
    _string_t& operator=(const _string_t &_copy);
    _string_t& operator=(_string_t &&_move);

    static _string_t format(_Const_String _format, ...);
 
  };
 
}

#include "string_view.hpp"

#define IS_UPPERCASE(c) (c >= 0x41 && c <= 0x5A)
#define IS_LOWERCASE(c) (c >= 0x61 && c <= 0x7A)

#define TO_UPPERCASE(c) IS_LOWERCASE(c) ? c - 0x20 : c;
#define TO_LOWERCASE(c) IS_UPPERCASE(c) ? c + 0x20 : c;

template<typename T>
me::_string_t<T>::_string_t(_Const_String _string, _Size _length, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Const_Iterator _iter = _string;

  this->_begin = (_Iterator) _alloc->malloc((_length * sizeof(_Char)) + sizeof(_Char));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
     *i = *_iter++;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::_string_t(_Char _char, _Size _length, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  this->_begin = (_Iterator) _alloc->malloc((_length * sizeof(_Char)) + sizeof(_Char));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = _char;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::_string_t(_Const_String _string, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Size _length = strlen(_string);

  _Const_Iterator _iter = _string;

  this->_begin = (_Iterator) _alloc->malloc((_length * sizeof(_Char)) + sizeof(_Char));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
     *i = *_iter++;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::_string_t(const _string_view_t<T> &_string, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Const_Iterator _iter = _string.begin();

  this->_begin = (_Iterator) _alloc->malloc((_string.size() * sizeof(_Char)) + sizeof(_Char));
  this->_end = this->_begin + _string.size();
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
     *i = *_iter++;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::_string_t(const _string_t<T> &_copy)
  : _alloc(_copy._alloc)
{
  _Const_Iterator _iter = _copy.begin();

  this->_begin = (_Iterator) _alloc->malloc((_copy.size() * sizeof(_Char)) + sizeof(_Char));
  this->_end = this->_begin + _copy.size();
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
     *i = *_iter++;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::_string_t(_string_t<T> &&_move)
{
  this->_begin = static_cast<_string_t<T>&&>(_move)._begin;
  this->_end = static_cast<_string_t<T>&&>(_move)._end;
  this->_capacity = static_cast<_string_t<T>&&>(_move)._capacity;
  this->_alloc = static_cast<_string_t<T>&&>(_move)._alloc;
}

template<typename T>
me::_string_t<T>::_string_t(_Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  this->_begin = (_Iterator) _alloc->malloc(sizeof(_Char));
  this->_end = this->_begin;
  this->_capacity = this->_end;
  *this->_end = nullchar;
}

template<typename T>
me::_string_t<T>::~_string_t()
{
  if (this->capacity() != 0)
  {
#if LIBME_STRING_VALIDATE == 1
    if (this->_begin == nullptr)
      logf(ERROR, "me::_string_t::destroy(): freeing nullptr");
#endif
    _alloc->free(this->_begin);
  }
}

template<typename T>
T* me::_string_t<T>::data() const
{
  return this->_begin;
}

template<typename T>
T* me::_string_t<T>::begin() const
{
  return this->_begin;
}

template<typename T>
T* me::_string_t<T>::end() const
{
  return this->_end;
}

template<typename T>
const T* me::_string_t<T>::cbegin() const
{
  return this->_begin;
}

template<typename T>
const T* me::_string_t<T>::cend() const
{
  return this->_end;
}

template<typename T>
void me::_string_t<T>::reserve(_Size _capacity)
{
  if (_capacity > this->capacity())
  {
    _Size _count = this->size();

    _Iterator _begin = (_Iterator) _alloc->malloc(_capacity * sizeof(_Char));
#if LIBME_STRING_VALIDATE == 1
    if (_begin == nullptr)
      logf(WARNING, "me::_string_t::reserve(%lu): me::allocator::malloc returned nullptr", _capacity);
#endif
    memory::uninitialized_copy<_Char>(_begin, this->_begin, this->_end);
    _alloc->free(this->_begin);
    this->_begin = _begin;
    this->_end = _begin + _count;
    this->_capacity = _begin + _capacity;
  }
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::insert(_Size _pos, const _string_view_t<T> &_str)
{
  this->reserve(this->size() + _str.size() + 1);

  _Iterator _src = this->_begin + _pos;
  _Iterator _dest = this->_end + _str.size();
  _Const_Iterator _iter = this->_end;
  while (_iter != _src - 1)
    *_dest-- = *_iter--;
  for (_Size i = 0; i != _str.size(); i++)
    _src[i] = _str.at(i);
  this->_end += _str.size();
  *this->_end = nullchar;
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::append(const _string_view_t<T> &_str)
{
  this->reserve(this->size() + _str.size() + 1);

  _Const_Iterator _iter = _str.begin();

  while (_iter != _str.end())
    *this->_end++ = *_iter++;

  *this->_end = nullchar;
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::append(_Char _chr)
{
  this->reserve(this->size() + 2);

  *this->_end++ = _chr;
  *this->_end = nullchar;
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::push_back(_Char _chr)
{
  this->reserve(this->size() + 2);

  *this->_end++ = _chr;
  *this->_end = nullchar;
  return *this;
}

template<typename T>
T me::_string_t<T>::pop_back()
{
  this->_end--;
  _Char _char = *this->_end;
  *this->_end = nullchar;
  return _char;
}

template<typename T>
void me::_string_t<T>::erase(_Iterator _begin, _Iterator _end)
{
#if LIBME_STRING_VALIDATE == 1
  if (_begin < this->_begin || _end > this->_end)
    logf(ERROR, "me::_string_t::erase(%p, %p): out of range", _begin, _end);
#endif

  _Size _len = (_end - _begin);
  _Size _tail_len = (this->_end - _end);

  while (_tail_len--)
    *_begin++ = *_end++;
  this->_end -= _len;
  *this->_end = nullchar;
}

template<typename T>
void me::_string_t<T>::erase(_Iterator _pos)
{
  erase(_pos, _pos + 1);
}

template<typename T>
void me::_string_t<T>::clear()
{
  this->_end = this->_begin;
  *this->_end = nullchar;
}

template<typename T>
void me::_string_t<T>::replace(_Iterator _begin, _Iterator _end, const _string_view_t<T> &_str)
{
  // TODO: faster?

  this->erase(_begin, _end);
  this->insert(_begin - this->_begin, _str);
}

template<typename T>
void me::_string_t<T>::to_lowercase(_Iterator _begin, _Iterator _end)
{
  while (_begin != _end)
    *_begin++ = TO_LOWERCASE(*_begin);
}

template<typename T>
void me::_string_t<T>::to_uppercase(_Iterator _begin, _Iterator _end)
{
  while (_begin != _end)
    *_begin++ = TO_UPPERCASE(*_begin);
}

template<typename T>
template<typename I>
[[nodiscard]] I me::_string_t<T>::as_int(_Size _off, uint8_t _base) const
{
  return strint<I>(this->_begin + _off, _base);
}

template<typename T>
void me::_string_t<T>::split(_Char _delimiter, _Size &_len, _string_view_t<T>* _strs) const
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
T& me::_string_t<T>::at(_Size _pos) const
{
  return this->_begin[_pos];
}

template<typename T>
bool me::_string_t<T>::starts_with(const _string_view_t<T> &_str) const
{
  if (this->size() < _str.size())
    return false;

  _Const_Iterator _iter1 = _str.begin();
  _Const_Iterator _iter2 = this->_begin;

  while (_iter1 != _str.end())
  {
    if (*_iter1++ != *_iter2++)
      return false;
  }

  return true;
}

template<typename T>
bool me::_string_t<T>::ends_with(const _string_view_t<T> &_str) const
{
  if (this->size() < _str.size())
    return false;

  _Const_Iterator _iter1 = _str.end() - 1;
  _Const_Iterator _iter2 = this->_end - 1;

  while (_iter1 != _str.begin() - 1)
  {
    if (*_iter1-- != *_iter2--)
      return false;
  }

  return true;
}

template<typename T>
me::size_t me::_string_t<T>::find(_Char _chr, _Size _off) const
{
  for (_Const_Iterator i = this->_begin + _off; i != this->_end; i++)
  {
    if (_chr == *i)
      return i - this->_begin;
  }
  return npos;
}

template<typename T>
me::size_t me::_string_t<T>::rfind(_Char _chr, _Size _off) const
{
  for (_Const_Iterator i = this->_end - 1 - _off; i != this->_begin - 1; i--)
  {
    if (_chr == *i)
      return i - this->_begin;
  }
  return npos;
}

template<typename T>
me::size_t me::_string_t<T>::find(const _string_view_t<T> &_str, _Size _off) const
{
  for (_Const_Iterator i = this->_begin + _off; i != this->_end; i++)
  {
    bool match = true;

    _Const_Iterator k = i;
    for (_Const_Iterator j = _str.begin(); j != _str.end(); j++)
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
me::size_t me::_string_t<T>::rfind(const _string_view_t<T> &_str, _Size _off) const
{
  for (_Const_Iterator i = this->_end - 1 - _off; i != this->_begin - 1; i++)
  {
    bool match = true;

    _Const_Iterator k = i;
    for (_Const_Iterator j = _str.begin(); j != _str.end(); j++)
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
}

template<typename T>
me::_string_view_t<T> me::_string_t<T>::substr(_Size _off, _Size _end) const
{
  return _string_view_t<T>(this->_begin + _off, this->_begin + _off + _end);
}

template<typename T>
me::_string_view_t<T> me::_string_t<T>::substr(_Size _off) const
{
  return _string_view_t<T>(this->_begin + _off, this->_end);
}

template<typename T>
void me::_string_t<T>::copy(_Size _off, _Size _len, _String _dst) const
{
  for (_Iterator i = this->_begin + _off; i != this->_begin + _off + _len; i++)
    *_dst++ = *i;
}

template<typename T>
const T* me::_string_t<T>::c_str() const
{
  return this->_begin;
}

template<typename T>
me::size_t me::_string_t<T>::capacity() const
{
  return this->_capacity - this->_begin;
}

template<typename T>
me::size_t me::_string_t<T>::size() const
{
  return this->_end - this->_begin;
}

template<typename T>
bool me::_string_t<T>::is_empty() const
{
  return this->_end == this->_begin;
}

template<typename T>
const T& me::_string_t<T>::operator[](_Size _pos) const
{
  return this->_begin[_pos];
}

template<typename T>
bool me::_string_t<T>::operator==(const _string_t &_str) const
{
  if (this->size() != _str.size())
    return false;

  _Const_Iterator _iter = _str.begin();
  for (_Iterator i = this->_begin; i != this->_end; i++)
  {
    if (*i != *_iter++)
      return false;
  }
  return true;
}

template<typename T>
bool me::_string_t<T>::operator==(const _string_view_t<T> &_str) const
{
  if (this->size() != _str.size())
    return false;

  _Const_Iterator _iter = _str.begin();
  for (_Iterator i = this->_begin; i != this->_end; i++)
  {
    if (*i != *_iter++)
      return false;
  }
  return true;
}

template<typename T>
bool me::_string_t<T>::operator==(const _String _str) const
{
  _Iterator _iter = this->_begin;
  while (true)
  {
    if (*_iter == nullchar || *_str == nullchar)
      return true;
    else if (*_iter != *_str)
      return false;
    _iter++;
    _str++;
  }
  return true;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::operator+=(_Char _chr)
{
  this->append(_chr);
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::operator+=(const _string_view_t<T> &_str)
{
  this->append(_str);
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::operator=(const _string_t<T> &_str)
{
  _Size _len = _str.size();
  _Const_Iterator _iter = _str.begin();

  this->_alloc = _str._alloc;
  this->_begin = (_Iterator) _alloc->malloc(_len * sizeof(_Char));
  this->_end = this->_begin + _len;
  this->_capacity = this->_begin + _len;

  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = *_iter++;
  this->_end[_len] = nullchar;
  return *this;
}

template<typename T>
me::_string_t<T>& me::_string_t<T>::operator=(_string_t<T> &&_str)
{
  this->_begin = static_cast<_string_t<T>&&>(_str)._begin;
  this->_end = static_cast<_string_t<T>&&>(_str)._end;
  this->_capacity = static_cast<_string_t<T>&&>(_str)._capacity;
  this->_alloc = static_cast<_string_t<T>&&>(_str)._alloc;
  return *this;
}

template<typename T>
me::_string_t<T> me::_string_t<T>::format(_Const_String _format, ...)
{
  T _temp[1024];

  va_list _args;
  va_start(_args, _format);
  vsprintf(_temp, _format, _args);
  va_end(_args);

  return _temp;
}

#endif
