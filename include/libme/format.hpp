#ifndef LIBME_FORMAT_HPP
  #define LIBME_FORMAT_HPP

#include "type.hpp"
#include "exception.hpp"
#include "string.hpp"

#include <cmath>
#include <type_traits>
#include <typeinfo>

namespace me {

  template<typename T>
  struct arg {
#ifdef LIBME_STRING_HPP
    const string_view name;
#else
    const char* name;
#endif
    const T value;
  };

  template<typename C, typename... A>
  constexpr int format(C* _buffer, const C* _format, const A&... _args);

}

namespace __me {

  using namespace me;

  enum __format_flags {
    FORMAT_FLAG_SPECIAL = 0x01
  };

  enum __arg_type : uint16_t {
    ARG_TYPE_CHAR_ARRAY,
    ARG_TYPE_CSTRING,
    ARG_TYPE_POINTER,

    ARG_TYPE_CHAR,
    ARG_TYPE_SHORT,
    ARG_TYPE_INT,
    ARG_TYPE_LONG,

    ARG_TYPE_UCHAR,
    ARG_TYPE_USHORT,
    ARG_TYPE_UINT,
    ARG_TYPE_ULONG,

    ARG_TYPE_FLOAT,
    ARG_TYPE_DOUBLE,

    ARG_TYPE_ME_ARG,
    ARG_TYPE_ME_STRING,
    ARG_TYPE_ME_STRING_VIEW,

    ARG_TYPE_CLASS,
  };

  struct __arg {
    const void* ptr;
    __arg_type type;
  };

  template<typename C, typename... A>
  constexpr int __format(C* buffer, const C* format, const A&... args);

  template<typename T, typename... A>
  constexpr void __store_args(__arg* iter, const T &first, const A&... args);

  template<typename C>
  __arg_type __get_arg_type(const C* &format);

  template<typename T, typename C>
  void __integer_to_string(T i, C* &str, int flags, int base) requires std::is_integral_v<T>;

  template<typename T, typename C>
  void __float_to_string(T f, C* &str, int flags, int base) requires std::is_floating_point_v<T>;

  template<typename T, typename C>
  void __number_to_string(T n, C* &str, int flags, int base);

  template<typename C>
  int __string_to_integer(C* &str);

}

/* "This is a test {0%HH:%MM:%SS}" */
template<typename C, typename... A>
constexpr int __me::__format(C* buffer, const C* format, const A&... args)
{
  constexpr size_t len = sizeof... (args);
  __arg iter[len];
  __store_args(iter, args...);

  size_t index = 0;

  const char* val_str;
#ifdef LIBME_STRING_HPP
  const string* val_me_str;
  const string_view* val_me_str_view;
#endif

  int flags;
  int base;

  /* getting options */
  while (*format != '\0')
  {
    if (*format != '{')
    {
      *buffer++ = *format++;
      continue;
    }
    format++;

    /* default */
    flags = 0;
    base = 10;

    /* getting index if found */
    if (isdigit(*format))
      index = __string_to_integer(format);

    /* getting current argument */
    const __arg &arg = iter[index];
    const void* arg_ptr = arg.ptr;
    __arg_type arg_type = arg.type;

    /* getting options */
    do {

      /* print number in 16 bit */
      if (*format == 'x')
      {
	format++;
	base = 16;

      /* print number in 2 bit */
      }else if (*format == 'b')
      {
	format++;
	base = 2;

      /* add special charcters */
      }else if (*format == '#')
      {
	format++;
	flags |= FORMAT_FLAG_SPECIAL;

      /* specify the type manullay */
      }else if (*format == '%')
      {
	format++;
	arg_type = __get_arg_type(format);
      }

    }while (*format && *format != '}');

    switch (arg_type)
    {
      /* string */
      case ARG_TYPE_CSTRING:
        val_str = *reinterpret_cast<const char* const*>(arg_ptr);
        while (*val_str != '\0')
          *buffer++ = *val_str++;
        break;

      /* char array */
      case ARG_TYPE_CHAR_ARRAY:
        val_str = reinterpret_cast<const char*>(arg_ptr);
        while (*val_str != '\0')
          *buffer++ = *val_str++;
        break;

      /* pointer */
      case ARG_TYPE_POINTER:
	__number_to_string(*reinterpret_cast<const size_t*>(arg_ptr), buffer, flags, 16);
	break;

      /* signed integer types */
      case ARG_TYPE_CHAR:
	__number_to_string(*reinterpret_cast<const char*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_SHORT:
	__number_to_string(*reinterpret_cast<const short*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_INT:
	__number_to_string(*reinterpret_cast<const int*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_LONG:
	__number_to_string(*reinterpret_cast<const long*>(arg_ptr), buffer, flags, base);
	break;

      /* unsigned integer types */
      case ARG_TYPE_UCHAR:
	__number_to_string(*reinterpret_cast<const unsigned char*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_USHORT:
	__number_to_string(*reinterpret_cast<const unsigned short*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_UINT:
	__number_to_string(*reinterpret_cast<const unsigned int*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_ULONG:
	__number_to_string(*reinterpret_cast<const unsigned long*>(arg_ptr), buffer, flags, base);
	break;

      /* float types */
      case ARG_TYPE_FLOAT:
	__number_to_string(*reinterpret_cast<const float*>(arg_ptr), buffer, flags, base);
	break;
      case ARG_TYPE_DOUBLE:
	__number_to_string(*reinterpret_cast<const double*>(arg_ptr), buffer, flags, base);
	break;

      /* libme classes */
#ifdef LIBME_STRING_HPP
      case ARG_TYPE_ME_STRING:
        val_me_str = reinterpret_cast<const string*>(arg_ptr);
	for (const char* i = val_me_str->cbegin(); i != val_me_str->cend(); i++)
	  *buffer++ = *i;
	break;
      case ARG_TYPE_ME_STRING_VIEW:
        val_me_str_view = reinterpret_cast<const string_view*>(arg_ptr);
	for (const char* i = val_me_str_view->begin(); i != val_me_str_view->end(); i++)
	  *buffer++ = *i;
	break;
#endif

      case ARG_TYPE_CLASS:
	break;

      default:
        break;
    }

    format++;
    index++;
  }

  *buffer++ = '\0';
  return 0;
}

template<typename C>
__me::__arg_type __me::__get_arg_type(const C* &format)
{
  switch (*format++)
  {
    case 's':
      return ARG_TYPE_CSTRING;
    case 'c':
      return ARG_TYPE_CHAR;
    case 'p':
      return ARG_TYPE_POINTER;

    /* long types */
    case 'l':
      switch (*format++)
      {
	case 'i':
	  return ARG_TYPE_LONG;
	case 'u':
	  return ARG_TYPE_ULONG;
	case 'f':
	  return ARG_TYPE_DOUBLE;
      }
      break;

    /* short types */
    case 'h':
      switch (*format++)
      {
	case 'i':
	  return ARG_TYPE_SHORT;
	case 'u':
	  return ARG_TYPE_USHORT;
	case 'f':
	  return ARG_TYPE_FLOAT;
      }
      break;

    case 'i':
      return ARG_TYPE_INT;
    case 'u':
      return ARG_TYPE_UINT;
    case 'f':
      return ARG_TYPE_FLOAT;
  }
  return ARG_TYPE_INT;
}

template<typename T, typename... A>
constexpr void __me::__store_args(__arg* iter, const T &first, const A&... args)
{
  using Tr = std::remove_cvref_t<T>;

  iter->ptr = &first;

  /* array types */
  if (std::is_array<Tr>::value)
  {
    typedef std::remove_extent_t<Tr> Tre;

    if (std::is_same<Tre, char>::value)
      iter->type = ARG_TYPE_CHAR_ARRAY;

  /* pointer types */
  }else if (std::is_pointer<Tr>::value)
  {
    if (std::is_same<Tr, char const*>::value)
      iter->type = ARG_TYPE_CSTRING;
    else
      iter->type = ARG_TYPE_POINTER;

  /* class types */
  }else if (std::is_class<Tr>::value)
  {
#ifdef LIBME_STRING_HPP
    if (std::is_same<Tr, string>::value)
      iter->type = ARG_TYPE_ME_STRING;
    else if (std::is_same<Tr, string_view>::value)
      iter->type = ARG_TYPE_ME_STRING_VIEW;
#endif

  }else
  {
    /* signed integer */
    if (std::is_same<Tr, char>::value)
      iter->type = ARG_TYPE_CHAR;
    else if (std::is_same<Tr, short>::value)
      iter->type = ARG_TYPE_SHORT;
    else if (std::is_same<Tr, int>::value)
      iter->type = ARG_TYPE_INT;
    else if (std::is_same<Tr, long>::value)
      iter->type = ARG_TYPE_LONG;

    /* unsigned integer */
    else if (std::is_same<Tr, unsigned char>::value)
      iter->type = ARG_TYPE_UCHAR;
    else if (std::is_same<Tr, unsigned short>::value)
      iter->type = ARG_TYPE_USHORT;
    else if (std::is_same<Tr, unsigned int>::value)
      iter->type = ARG_TYPE_UINT;
    else if (std::is_same<Tr, unsigned long>::value)
      iter->type = ARG_TYPE_ULONG;

    /* floating-point */
    else if (std::is_same<Tr, float>::value)
      iter->type = ARG_TYPE_FLOAT;
    else if (std::is_same<Tr, double>::value)
      iter->type = ARG_TYPE_DOUBLE;

    else
      throw exception("unknown data type '%s'", typeid(Tr).name());
  }

  if constexpr (sizeof... (args) > 0)
  {
    iter++;
    __store_args(iter, args...);
  }
}

template<typename T, typename C>
void __me::__integer_to_string(T i, C* &str, int flags, int base) requires std::is_integral_v<T>
{
  static const char* CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  if (i == 0)
  {
    *str++ = '0';
    return;
  }

  C temp[66];
  int index = 0;
  bool unsig;

  if constexpr (std::is_signed_v<T>)
  {
    if (i < 0)
    {
      unsig = true;
      i = -i;
    }else
      unsig = false;
  }else
    unsig = false;

  while (i)
  {
    T r = i % base;
    i = i / base;

    temp[index++] = CHARS[r];
  }

  if (unsig)
    *str++ = '-';
  while (index)
    *str++ = temp[(index--) - 1];
}

template<typename T, typename C>
void __me::__float_to_string(T f, C* &str, int flags, int base) requires std::is_floating_point_v<T>
{
  static const char* CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const C dec_point = '.';

  long i1 = static_cast<long>(f);
  long i2 = static_cast<long>(f - static_cast<T>(i1));

  C temp[128];
  int index = 0;
  bool unsig;

  if (i1 < 0)
  {
    unsig = true;
    i1 = -i1;
  }else
    unsig = false;

  if (i1 == 0)
    temp[index++] = dec_point;

  while (i1)
  {
    int r = i1 % base;
    i1 = i1 / base;
    temp[index++] = CHARS[r];
  }

  temp[index++] = dec_point;

  if (i2 == 0)
    temp[index++] = dec_point;

  while (i2)
  {
    int r = i2 % base;
    i2 = i2 / base;
    temp[index++] = CHARS[r];
  }

  if (unsig)
    *str++ = '-';
  while (index)
    *str++ = temp[(index--) - 1];
}

template<typename T, typename C>
void __me::__number_to_string(T n, C* &str, int flags, int base)
{
  if (base == 16 && flags & FORMAT_FLAG_SPECIAL)
  {
    *str++ = '0';
    *str++ = 'x';
  }

  if constexpr (std::is_integral_v<T>)
    __integer_to_string(n, str, flags, base);
  else if constexpr (std::is_floating_point_v<T>)
    __float_to_string(n, str, flags, base);
}

template<typename C>
int __me::__string_to_integer(C* &_str)
{
  int i = 0;
  while (isdigit(*_str))
    i = 10 * i + *_str++ - '0';
  return i;
}

template<typename C, typename... A>
constexpr int me::format(C* _buffer, const C* _format, const A&... _args)
{
  if constexpr (sizeof...(_args) > 0)
    return __me::__format(_buffer, _format, _args...);
  while (*_format != '\0')
    *_buffer++ = *_format++;
  return 0;
}

#endif
