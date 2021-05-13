#ifndef LIBME_ITERATOR_HPP
  #define LIBME_ITERATOR_HPP

#include "type.hpp"

namespace me {

  template<typename T> class array_proxy;

  template<typename T>
  class iterator {

  protected:

    typedef size_t _Length;
    typedef size_t _Position;
    typedef T _Element;
    typedef const T _Const_Element;
    typedef const T* _Elements;

    _Length _length;
    _Elements _elements;
    mutable _Position _position;

  public:

    iterator(const array_proxy<T> &_arr, _Position _position = 0);

    bool has_next() const;

    _Const_Element& next() const;
    _Const_Element& peek() const;
    _Const_Element& prev() const;

  };

}

#include "array_proxy.hpp"

template<typename T>
me::iterator<T>::iterator(const array_proxy<T> &_arr, _Position _position)
  : _length(_arr.size()), _elements(_arr.data()), _position(_position)
{
}

template<typename T>
bool me::iterator<T>::has_next() const
{
  return this->_position < this->_length;
}
 
template<typename T>
const T& me::iterator<T>::next() const
{
  return this->_elements[this->_position++];
}

template<typename T>
const T& me::iterator<T>::peek() const
{
  return this->_elements[this->_position];
}

template<typename T>
const T& me::iterator<T>::prev() const
{
  return this->_elements[this->_position - 1];
}

#endif
