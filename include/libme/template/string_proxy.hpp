#ifndef LIBME_STRING_PROXY_TYPE_HPP
  #define LIBME_STRING_PROXY_TYPE_HPP

#include "../type.hpp"

namespace me {

  template<typename T, class S>
  class _string_proxy_t {

  protected:

    typedef size_t _Size;
    typedef T _Char;
    typedef S _String_Type;
    typedef const S _Const_String_Type;

    const _String_Type* _string;

  public:

    _string_proxy_t(_Const_String_Type* _string);

    [[nodiscard]] _Char at(_Size _pos) const;

    [[nodiscard]] _Size size() const;
    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] _Char operator[](_Size _pos) const;

    [[nodiscard]] bool operator==(const _string_proxy_t<T, S> &_str) const;

  };

}

template<typename T, class S>
me::_string_proxy_t<T, S>::_string_proxy_t(_Const_String_Type* _string)
  : _string(_string)
{
}

template<typename T, class S>
T me::_string_proxy_t<T, S>::at(_Size _pos) const
{
  return this->_string->at(_pos);
}

template<typename T, class S>
me::size_t me::_string_proxy_t<T, S>::size() const
{
  return this->_string->size();
}

template<typename T, class S>
bool me::_string_proxy_t<T, S>::is_empty() const
{
  return this->_string->is_empty();
}

template<typename T, class S>
T me::_string_proxy_t<T, S>::operator[](_Size _pos) const
{
  return this->_string->operator[](_pos);
}

template<typename T, class S>
bool me::_string_proxy_t<T, S>::operator==(const _string_proxy_t<T, S> &_str) const
{
  if (this->_string->size() != _str.size())
    return false;

  for (_Size i = 0; i < this->_string->size(); i++)
  {
    if (this->_string->at(i) != _str.at(i))
      return false;
  }
  return true;
}

#endif
