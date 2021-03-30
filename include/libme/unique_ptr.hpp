#ifndef LIBME_UNIQUE_PTR_HPP
  #define LIBME_UNIQUE_PTR_HPP

#include "type.hpp"

namespace me {

  template<typename T>
  class unique_ptr {

  private:

    typedef T _Type;
    typedef T* _Pointer;

    _Pointer _pointer;

  public:

    unique_ptr(_Pointer _pointer);
    unique_ptr(const unique_ptr &_copy);

    _Pointer operator->() const;

    template<typename... A>
    [[nodiscard]] static unique_ptr make(A&&... _args);

  };

}

#include "exception.hpp"

template<typename T>
me::unique_ptr<T>::unique_ptr(_Pointer _pointer)
  : _pointer(_pointer)
{
}

template<typename T>
me::unique_ptr<T>::unique_ptr(const unique_ptr<T>&)
{
  throw exception("unique_ptr cannot be copied");
}

template<typename T>
T* me::unique_ptr<T>::operator->() const
{
  return _pointer;
}

template<typename T>
template<typename... A>
me::unique_ptr<T> me::unique_ptr<T>::make(A&&... _args)
{
  return unique_ptr(new T(static_cast<A&&>(_args)...));
}

#endif
