#ifndef LIBME_OPTIONAL_HPP
  #define LIBME_OPTIONAL_HPP

#include "type.hpp"

namespace me {

  template<typename T>
  class optional {

  protected:

    typedef bool _Valid;
    typedef T _Type;
    typedef const T _Const_Type;

    mutable _Valid _valid;
    _Type _data;

  public:

    optional();

    _Valid has() const;
    _Type& value();

    void assign(_Type &&_value);
    void assign(_Const_Type &_value);

    void operator=(_Type &&_value);
    void operator=(_Const_Type &_value);

  };

}

#include "exception.hpp"

template<typename T>
me::optional<T>::optional()
  : _valid(false)
{
}

template<typename T>
bool me::optional<T>::has() const
{
  return _valid;
}

template<typename T>
T& me::optional<T>::value()
{
  if (!_valid)
    throw exception("me::optional<T>::data not valid");
  return _data;
}

template<typename T>
void me::optional<T>::assign(_Type &&_value)
{
  _data = static_cast<_Type&&>(_value);
  _valid = true;
}

template<typename T>
void me::optional<T>::assign(_Const_Type &_value)
{
  _data = _value;
  _valid = true;
}

template<typename T>
void me::optional<T>::operator=(_Type &&_value)
{
  assign(_value);
}

template<typename T>
void me::optional<T>::operator=(_Const_Type &_value)
{
  assign(_value);
}
#endif
