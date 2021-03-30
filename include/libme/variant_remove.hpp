#ifndef LIBME_VARIANT_HPP
  #define LIBME_VARIANT_HPP

#include "type.hpp"
#include "shared_ptr.hpp"

namespace me {

  template<typename T>
  struct _type_wrapper {
    typedef T _Type;
    typedef T _Ref_Type;
  };

  template<typename T>
  struct _type_wrapper<T&> {
    typedef T _Type;
    typedef T _Ref_Type;
  };

  template<typename T>
  struct _type_wrapper<const T> {
    typedef T _Type;
    typedef T _Ref_Type;
  };

  template<typename T>
  struct _type_wrapper<const T&> {
    typedef T _Type;
    typedef T _Ref_Type;
  };

  class variant {

  protected:

    struct _Abstract_Value {
      virtual ~_Abstract_Value() { }
    };

    template<typename T>
    struct _Value : public _Abstract_Value {
      T _value;
      _Value(T _value)
	: _value(_value)
      {
      }
      ~_Value() override
      {
	_value.~T();
      }
    };

    _Abstract_Value* _value;

  public:

    template<typename T>
    variant(T &&_value);
    
    variant(const variant &_copy);
    variant(variant &&_move);
    variant();

    template<typename T>
    T& get();

    variant& operator=(const variant &_copy);
    variant& operator=(variant &&_move);

  };

}

#include "exception.hpp"

#include <type_traits>

template<typename T>
me::variant::variant(T &&_value)
  : _value((_Abstract_Value*) new _Value<typename _type_wrapper<T>::_Type>{_value})
{
}

template<typename T>
T& me::variant::get()
{
  if (this->_value == nullptr)
    throw exception("me::variant::get() -> '_value' not valid");
  return *((_Value<T>*) this->_value);
}
#endif
