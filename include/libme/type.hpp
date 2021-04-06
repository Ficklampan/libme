#ifndef MELIB_TYPE_HPP
  #define MELIB_TYPE_HPP

namespace me {

  enum LogType {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
  };


#ifndef MIN
  #define MIN(a, b) (((b) < (a)) ? (b) : (a))
#endif

#ifndef MAX
  #define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef isdigit
  #define isdigit(c) ((c) >= '0' && (c) <= '9')
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

  static inline void logf(LogType _type, const char* _format, ...);

}

#include <stdio.h>
#include <stdarg.h>

inline void me::logf(LogType _type, const char* _format, ...)
{
  switch (_type)
  {
    case FATAL:
      ::printf("\e[31mfatal:\e[0m ");
      break;

    case ERROR:
      ::printf("\e[31merror:\e[0m ");
      break;

    case WARNING:
      ::printf("\e[33mwarning:\e[0m ");
      break;

    case INFO:
      ::printf("\e[32minfo:\e[0m ");
      break;

    case DEBUG:
      ::printf("\e[35mdebug:\e[0m ");
      break;
  }

  va_list _args;
  va_start(_args, _format);
  vprintf(_format, _args);
  va_end(_args);

  putchar('\n');
}

#endif
