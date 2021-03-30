#ifndef LIBME_ALGORITHM_HPP
  #define LIBME_ALGORITHM_HPP

#include "type.hpp"
#include "array_proxy.hpp"

namespace me::algorithm {

  template<typename T>
  size_t array_find(const array_proxy<T> &_array, const T &_value);

  template<typename T>
  bool array_has(const array_proxy<T> &_array, const array_proxy<T> &_elements);

  template<typename T>
  T* unique_array(const array_proxy<T> &_array, T* _buffer, uint32_t &_count);

}

template<typename T>
me::size_t me::algorithm::array_find(const array_proxy<T> &_array, const T &_value)
{
  for (size_t i = 0; i < _array.size(); i++)
  {
    if (_array[i] == _value)
      return i;
  }
  return SIZE_MAX;
}

template<typename T>
bool me::algorithm::array_has(const array_proxy<T> &_array, const array_proxy<T> &_elements)
{
  for (const T &_element : _elements)
  {
    if (array_find(_array, _element) == SIZE_MAX)
      return false;
  }
  return true;
}

template<typename T>
T* me::algorithm::unique_array(const array_proxy<T> &_array, T* _buffer, uint32_t &_count)
{
  _count = 0;
  for (uint32_t i = 0; i < _array.size(); i++)
  {
    if (array_find({_count, _buffer}, _array[i]) == SIZE_MAX)
      _buffer[_count++] = _array[i];
  }
  return _buffer;
}
#endif
