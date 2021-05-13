#ifndef LIBME_ARRAY_PROXY_HPP
  #define LIBME_ARRAY_PROXY_HPP

#include "type.hpp"

#include <initializer_list>

namespace me {

  template<typename T> class vector;
  template<typename T, size_t Size> class array;

  template<typename T>
  struct array_proxy {

  protected:

    typedef size_t _Size;
    typedef const size_t _Const_Size;
    typedef T _Type;
    typedef const T _Const_Type;
    typedef const T* _Pointer;

    _Pointer _pointer;
    _Const_Size _length;

  public:

    array_proxy(_Pointer _pointer, _Size _length);

    array_proxy(const vector<T> &_vec);

    template<size_t Size>
    array_proxy(const array<T, Size> &_arr);

    template<typename CountType>
    array_proxy(const array_proxy &_arr);

    template<typename CountType, class A>
    array_proxy(const A &_arr);

    array_proxy(std::initializer_list<T> &_arr);

    [[nodiscard]] _Pointer data() const;
    [[nodiscard]] _Pointer begin() const;
    [[nodiscard]] _Pointer end() const;

    [[nodiscard]] _Size size() const;

    [[nodiscard]] _Const_Type& at(_Size _pos) const;

    [[nodiscard]] _Const_Type& operator[](_Size _pos) const;

  };

}

#include "vector.hpp"
#include "array.hpp"

template<typename T>
me::array_proxy<T>::array_proxy(_Pointer _pointer, _Size _length)
  : _pointer(_pointer), _length(_length)
{
}

template<typename T>
me::array_proxy<T>::array_proxy(const vector<T> &_vec)
  : _pointer(_vec.data()), _length(_vec.size())
{
}

template<typename T>
template<me::size_t Size>
me::array_proxy<T>::array_proxy(const array<T, Size> &_arr)
  : _pointer(_arr._elements), _length(Size)
{
}

template<typename T>
template<typename CountType>
me::array_proxy<T>::array_proxy(const array_proxy<T> &_arr)
  : _pointer(_arr._pointer), _length(_arr._length)
{
}

template<typename T>
template<typename CountType, class A>
me::array_proxy<T>::array_proxy(const A &_arr)
  : _pointer(_arr.data()), _length(_arr.size())
{
}

template<typename T>
me::array_proxy<T>::array_proxy(std::initializer_list<T> &_arr)
  : _pointer(_arr.begin()), _length(_arr.size())
{
}

template<typename T>
const T* me::array_proxy<T>::data() const
{
  return _pointer;
}

template<typename T>
const T* me::array_proxy<T>::begin() const
{
  return _pointer;
}

template<typename T>
const T* me::array_proxy<T>::end() const
{
  return _pointer + _length;
}

template<typename T>
me::size_t me::array_proxy<T>::size() const
{
  return _length;
}

template<typename T>
const T& me::array_proxy<T>::at(_Size _pos) const
{
  return _pointer[_pos];
}

template<typename T>
const T& me::array_proxy<T>::operator[](_Size _pos) const
{
  return _pointer[_pos];
}

#endif
