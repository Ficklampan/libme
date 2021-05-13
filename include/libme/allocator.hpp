#ifndef LIBME_ALLOCATOR_HPP
  #define LIBME_ALLOCATOR_HPP

#include "type.hpp"

#include <stdlib.h>
#include <string.h>
#include <new>
#include <type_traits>

namespace me {

  class allocator {

  protected:

    typedef size_t _Size;
    typedef void* _Pointer;

  public:

    [[nodiscard]] inline virtual _Pointer malloc(_Size _len);
    [[nodiscard]] inline virtual _Pointer realloc(_Pointer _ptr, _Size _len);
    inline virtual void free(_Pointer _ptr);

    /* Construct Allocate */
    template<typename T, typename... A>
    T* calloc(A&&... _args);

    /* Multiple Allocate */
    template<typename T>
    T* malloc(_Size _count, T &&_value);

    /* Allocate */
    template<typename T>
    T* alloc(T &&_value);

    template<typename T>
    void mdealloc(T* _begin, T* _end) requires std::is_trivially_destructible_v<T>;

    template<typename T>
    void mdealloc(T* _begin, T* _end);

    template<typename T>
    void dealloc(T* _ptr) requires std::is_trivially_destructible_v<T>;

    template<typename T>
    void dealloc(T* _ptr);

    template<typename T, typename... A>
    T* construct(T* _ptr, A&&... _args);

    template<typename T>
    void destruct(T* _ptr);

    static allocator* _default();

  };

}

inline void* me::allocator::malloc(_Size _len)
{
  return ::malloc(_len);
}

inline void* me::allocator::realloc(_Pointer _ptr, _Size _len)
{
  return ::realloc(_ptr, _len);
}

inline void me::allocator::free(_Pointer _ptr)
{
  ::free(_ptr);
}

template<typename T, typename... A>
T* me::allocator::calloc(A&&... _args)
{
  T* _ptr = reinterpret_cast<T*>(malloc(sizeof(T)));
  return (T*) ::new ((void*) _ptr) T(static_cast<A&&>(_args)...);
}

template<typename T>
T* me::allocator::malloc(_Size , T &&)
{
  // TODO
  return nullptr;
}

template<typename T>
T* me::allocator::alloc(T &&_value)
{
  T* _ptr = reinterpret_cast<T*>(malloc(sizeof(T)));
  return (T*) ::new ((void*) _ptr) T(static_cast<T&&>(_value));
}

template<typename T>
void me::allocator::mdealloc(T* , T* ) requires std::is_trivially_destructible_v<T>
{
}

template<typename T>
void me::allocator::mdealloc(T* _begin, T* _end)
{
  while (_begin != _end)
    destruct(_begin++);
}

template<typename T>
void me::allocator::dealloc(T* _ptr) requires std::is_trivially_destructible_v<T>
{
  free(_ptr);
}

template<typename T>
void me::allocator::dealloc(T* _ptr)
{
  destruct(_ptr);
  free(_ptr);
}

template<typename T, typename... A>
T* me::allocator::construct(T* _ptr, A&&... _args)
{
  return (T*) ::new ((void*) _ptr) T(static_cast<A&&>(_args)...);
}

template<typename T>
void me::allocator::destruct(T* _ptr)
{
  _ptr->~T();
}

#endif
