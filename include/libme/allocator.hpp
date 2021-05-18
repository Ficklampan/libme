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

    typedef size_t Size;
    typedef void* Pointer;

  public:

    template<typename T>
    static constexpr T* malloc(Size len);

    template<typename T>
    static constexpr T* realloc(T* ptr, Size len);

    template<typename T>
    static constexpr void free(T* ptr);

    /* Construct Allocate */
    template<typename T, typename... A>
    static constexpr T* calloc(A&&... args);

    /* Allocate */
    template<typename T>
    static constexpr T* alloc(T &&value);

    template<typename T>
    static constexpr void dealloc(T* ptr) requires std::is_trivially_destructible_v<T>;

    template<typename T>
    static constexpr void dealloc(T* ptr);

    template<typename T, typename... A>
    static constexpr T* construct(T* ptr, A&&... args);

    template<typename T>
    static constexpr void destruct(T* ptr) requires std::is_trivially_destructible_v<T>;

    template<typename T>
    static constexpr void destruct(T* ptr);

  };

}

template<typename T>
constexpr T* me::allocator::malloc(Size len)
{
  return reinterpret_cast<T*>(::malloc(len * sizeof(T)));
}

template<typename T>
constexpr T* me::allocator::realloc(T* ptr, Size len)
{
  return reinterpret_cast<T*>(::realloc(ptr, len * sizeof(T)));
}

template<typename T>
constexpr void me::allocator::free(T* ptr)
{
  ::free(ptr);
}

template<typename T, typename... A>
constexpr T* me::allocator::calloc(A&&... args)
{
  T* ptr = reinterpret_cast<T*>(malloc<T>(sizeof(T)));
  return (T*) ::new ((void*) ptr) T(static_cast<A&&>(args)...);
}

template<typename T>
constexpr T* me::allocator::alloc(T &&value)
{
  T* ptr = reinterpret_cast<T*>(malloc<T>(sizeof(T)));
  return (T*) ::new ((void*) ptr) T(static_cast<T&&>(value));
}

template<typename T>
constexpr void me::allocator::dealloc(T* ptr) requires std::is_trivially_destructible_v<T>
{
  free(ptr);
}

template<typename T>
constexpr void me::allocator::dealloc(T* ptr)
{
  destruct(ptr);
  free(ptr);
}

template<typename T, typename... A>
constexpr T* me::allocator::construct(T* ptr, A&&... args)
{
  return (T*) ::new ((void*) ptr) T(static_cast<A&&>(args)...);
}

template<typename T>
constexpr void me::allocator::destruct(T* ) requires std::is_trivially_destructible_v<T>
{
}

template<typename T>
constexpr void me::allocator::destruct(T* ptr)
{
  ptr->~T();
}

#endif
