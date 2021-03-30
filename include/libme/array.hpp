#ifndef LIBME_ARRAY_HPP
  #define LIBME_ARRAY_HPP

#include "type.hpp"

namespace me {

  template<typename T, size_t Size>
  class array {

  public:

    typedef size_t _Length;
    typedef size_t _Position;
    typedef T _Type;
    typedef const T _Const_Type;
    typedef T* _Elems;

    _Type _elements[Size];

    [[nodiscard]] _Elems data() const;
    [[nodiscard]] _Elems begin() const;
    [[nodiscard]] _Elems end() const;

    void swap(_Position _src, _Position _dst);

    void copy(_Position _off, _Length _len, _Elems _dest) const;

    [[nodiscard]] _Type& at(_Position) const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] _Type& operator[](_Position);
    [[nodiscard]] _Type& operator[](_Position) const;

    [[nodiscard]] bool operator==(const array<T, Size> &_arr) const;
    [[nodiscard]] bool operator!=(const array<T, Size> &_arr) const;

  };
}

template<typename T, me::size_t Size>
T* me::array<T, Size>::data() const
{
  return _elements;
}

template<typename T, me::size_t Size>
T* me::array<T, Size>::begin() const
{
  return _elements;
}

template<typename T, me::size_t Size>
T* me::array<T, Size>::end() const
{
  return _elements + Size;
}

template<typename T, me::size_t Size>
void me::array<T, Size>::swap(_Position _src, _Position _dst)
{
  _Type &_temp = _elements[_src];
  _elements[_src] = _elements[_dst];
  _elements[_dst] = _temp;
}

template<typename T, me::size_t Size>
void me::array<T, Size>::copy(_Position _off, _Length _len, _Elems _dest) const
{
  for (_Position i = 0; i < _len; i++)
    _dest[i] = _elements[i + _off];
}

template<typename T, me::size_t Size>
T& me::array<T, Size>::at(_Position _pos) const
{
  return _elements[_pos];
}

template<typename T, me::size_t Size>
me::size_t me::array<T, Size>::size() const
{
  return Size;
}

template<typename T, me::size_t Size>
T& me::array<T, Size>::operator[](_Position _pos)
{
  return _elements[_pos];
}

template<typename T, me::size_t Size>
T& me::array<T, Size>::operator[](_Position _pos) const
{
  return _elements[_pos];
}

template<typename T, me::size_t Size>
bool me::array<T, Size>::operator==(const array<T, Size> &_arr) const
{
  for (_Position i = 0; i < Size; i++)
  {
    if (_arr._elements[i] != _elements[i])
      return false;
  }
  return true;
}

template<typename T, me::size_t Size>
bool me::array<T, Size>::operator!=(const array<T, Size> &_arr) const
{
  if (Size == 0)
    return false;

  return _elements[0] != _arr._elements[0];
}
#endif
