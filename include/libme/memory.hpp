#ifndef LIBME_MEMORY_HPP
  #define LIBME_MEMORY_HPP

#include "type.hpp"

#include <string.h>
#include <utility>
#include <type_traits>

namespace me {

  namespace memory {

    template<typename T> void uninitialized_move(T* _dest, T* _src, me::size_t _count);
    template<typename T> void move(T* _dest, T* _src, me::size_t _count);
    template<typename T> void safe_move(T* _dest, T* _src, me::size_t _count) requires std::is_move_assignable_v<T>;
    template<typename T> void safe_move(T* _dest, T* _src, me::size_t _count);

    template<typename T> void uninitialized_copy(T* _dest, T* _begin, T* _end);
    template<typename T> void copy(T* _dest, T* _begin, T* _end);
    template<typename T> void safe_copy(T* _dest, T* _begin, T* _end) requires std::is_copy_assignable_v<T>;
    template<typename T> void safe_copy(T* _dest, T* _begin, T* _end);

  }

}

template<typename T>
void me::memory::uninitialized_move(T* _dest, T* _src, me::size_t _count)
{
  if (_dest < _src)
  {
    for (me::size_t i = 0; _count--; i++)
      ::memcpy(_dest + i, _src + i, sizeof(T));
  }else if (_dest > _src)
  {
    for (me::size_t i = _count - 1; _count--; i--)
      ::memcpy(_dest + i, _src + i, sizeof(T));
  }
}

template<typename T>
void me::memory::move(T* _dest, T* _src, me::size_t _count)
{
  if (_dest < _src)
  {
    for (me::size_t i = 0; _count--; i++)
      _dest[i] = std::move(_src[i]);
  }else if (_dest > _src)
  {
    for (me::size_t i = _count - 1; _count--; i--)
      _dest[i] = std::move(_src[i]);
  }
}

template<typename T>
void me::memory::safe_move(T* _dest, T* _src, me::size_t _count) requires std::is_move_assignable_v<T>
{
  //move(_dest, _src, _count);
  uninitialized_move(_dest, _src, _count);
}

template<typename T>
void me::memory::safe_move(T* _dest, T* _src, me::size_t _count)
{
  uninitialized_move(_dest, _src, _count);
}

template<typename T>
void me::memory::uninitialized_copy(T* _dest, T* _begin, T* _end)
{
  char* __dest = reinterpret_cast<char*>(_dest);
  char* __begin = reinterpret_cast<char*>(_begin);
  char* __end = reinterpret_cast<char*>(_end);

  while (__begin != __end)
    *__dest++ = *__begin++;
}

template<typename T>
void me::memory::copy(T* _dest, T* _begin, T* _end)
{
  while (_begin != _end)
    *_dest++ = static_cast<const T&>(*_begin++);
}

template<typename T>
void me::memory::safe_copy(T* _dest, T* _begin, T* _end) requires std::is_copy_assignable_v<T>
{
  //copy(_dest, _begin, _end);
  uninitialized_copy(_dest, _begin, _end);
}

template<typename T>
void me::memory::safe_copy(T* _dest, T* _begin, T* _end)
{
  uninitialized_copy(_dest, _begin, _end);
}

#endif
