#ifndef LIBME_VALUE_HPP
  #define LIBME_VALUE_HPP

#include "__util.hpp"

#include <type_traits>

namespace me {


  template<typename... Ts>
  class variant {

  protected:

    static constexpr size_t _Index_Size = __me::__int_bytes_needed<sizeof...(Ts)>();
    static constexpr size_t _Size = __me::__calculate_size<Ts...>();

    typedef typename __me::suitable_integral<_Index_Size>::Type _Index;
    typedef uint8_t _Byte;

    _Index _index;
    _Byte _data[_Size];

  public:

    template<typename T> variant(T &&_value);
    ~variant();

    template<typename T> T& get() const;

  };

}

#include "exception.hpp"

#include <string.h>

template<typename... Ts>
template<typename T>
me::variant<Ts...>::variant(T &&_value)
{
  if (!__me::__contains_type<T, Ts...>())
    throw exception("me::variant<Ts...>::variant(_value): invalid type");

  this->_index = __me::__index_of<T, Ts...>();
  ::new ((void*) &this->_data[0]) T(static_cast<T&&>(_value));
}

template<typename... Ts>
me::variant<Ts...>::~variant()
{
  //auto* _value = __me::__get_value_at<Ts...>(&_data[0], _index);
  //__me::__destruct(*_value);
}

template<typename... Ts>
template<typename T>
T& me::variant<Ts...>::get() const
{
  return *reinterpret_cast<T*>(&_data[0]);
}

#endif
