#ifndef MELIB_TYPE_HPP
  #define MELIB_TYPE_HPP

#include <ctype.h>

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

#ifndef SIZE_MAX
  #define SIZE_MAX __SIZE_MAX__
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

}

#endif
