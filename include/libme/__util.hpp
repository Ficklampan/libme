#ifndef __LIBME_UTIL
  #define __LIBME_UTIL

#include "type.hpp"

#include <type_traits>

namespace __me {

  using namespace me;

  template<typename First, typename... Types>
  static constexpr size_t __calculate_size();

  template<typename Type, typename First, typename... Types>
  static constexpr size_t __index_of(size_t _index = 0);

  template<typename Type, typename First, typename... Types>
  static constexpr bool __contains_type();

  template<size_t Number>
  static constexpr int __int_bytes_needed();

  template<typename First, typename... Types>
  static constexpr auto* __get_value_at(void* _value, auto _index, size_t _iter = 0);

  template<typename T>
  static void __destruct(T &_value);

  template<int Bytes> struct suitable_integral { typedef int Type; };
  template<> struct suitable_integral<1> { typedef uint8_t Type; };
  template<> struct suitable_integral<2> { typedef uint16_t Type; };
  template<> struct suitable_integral<4> { typedef uint32_t Type; };
  template<> struct suitable_integral<8> { typedef uint64_t Type; };

}

template<typename First, typename... Types>
static constexpr me::size_t __me::__calculate_size()
{
  me::size_t _size = sizeof(First);
  if (sizeof...(Types) > 0)
    _size += __calculate_size<Types...>();
  return _size;
}

template<typename Type, typename First, typename... Types>
static constexpr me::size_t __me::__index_of(size_t _index)
{
  typedef std::remove_cvref<Type> _Type1;
  typedef std::remove_cvref<First> _Type2;

  if (std::is_same<_Type1, _Type2>::value)
    return _index;

  if (sizeof...(Types) > 0)
    return __index_of<Type, Types...>(_index + 1);
  return -1;
}

template<typename Type, typename First, typename... Types>
static constexpr bool __me::__contains_type()
{
  typedef std::remove_cvref<Type> _Type1;
  typedef std::remove_cvref<First> _Type2;

  if (std::is_same<_Type1, _Type2>::value)
    return true;

  if (sizeof...(Types) > 0)
    return __contains_type<Type, Types...>();
  return false;
}

template<__me::size_t Number>
static constexpr int __me::__int_bytes_needed()
{
  if (Number > 0xFFFFFFFF)
    return 8;
  else if (Number > 0xFFFF)
    return 4;
  else if (Number > 0xFF)
    return 2;
  return 1;
}

template<typename First, typename... Types>
static constexpr auto* __me::__get_value_at(void* _value, auto _index, size_t _iter)
{
  if (_iter++ == _index)
    return reinterpret_cast<First*>(_value);

  if (sizeof...(Types) > 0)
    return __get_value_at<Types...>(_value, _index, _iter);
  return nullptr;
}

template<typename T>
static void __destruct(T &_value)
{
  if (std::is_destructible_v<T>)
    _value.~T();
}

#endif
