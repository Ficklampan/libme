#ifndef MELIB_TYPE_HPP
  #define MELIB_TYPE_HPP

#include <ctype.h>
#include <memory>
#include <type_traits>

namespace me {

#ifndef MIN
  #define MIN(a, b) (((b) < (a)) ? (b) : (a))
#endif

#ifndef MAX
  #define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef isletter
  #define isletter(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#endif

#ifndef distance
  #define distance(p1, p2) ((p2) - (p1))
#endif

#ifndef SIZE_MAX
  #define SIZE_MAX __SIZE_MAX__
#endif

#ifndef ME_ALWAYS_INLINE
  #define ME_ALWAYS_INLINE [[gnu::always_inline]]
#endif

  typedef __UINT8_TYPE__ uint8_t;
  typedef __UINT16_TYPE__ uint16_t;
  typedef __UINT32_TYPE__ uint32_t;
  typedef __UINT64_TYPE__ uint64_t;

  typedef __INT8_TYPE__ int8_t;
  typedef __INT16_TYPE__ int16_t;
  typedef __INT32_TYPE__ int32_t;
  typedef __INT64_TYPE__ int64_t;
  typedef __INTMAX_TYPE__ intmax_t;
  typedef __UINTMAX_TYPE__ uintmax_t;

  typedef int32_t int_t;
  typedef uint32_t uint_t;

  typedef __SIZE_TYPE__ size_t;

  static constexpr size_t dynamic_extent = SIZE_MAX;

#define LIBME_ASSERT(t, s)

  template<typename T>
  constexpr T* to_address(T* ptr)
  {
    static_assert(!std::is_function<T>::value);
    return ptr;
  }


  template<typename T>
  constexpr T* to_address(const T &val)
  {
    if constexpr (requires{std::pointer_traits<T>::to_address(val);})
      return std::pointer_traits<T>::pointer_to(val);
    else
      return val.operator->();
  }

}

#endif
