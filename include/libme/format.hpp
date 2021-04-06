#ifndef LIBME_FORMAT_HPP
  #define LIBME_FORMAT_HPP

#include "type.hpp"
#include "exception.hpp"

#include "string.hpp"
#include "vector.hpp"

#include <type_traits>
#include <typeinfo>

namespace me {

  template<typename C, typename... A>
  constexpr int format(C* _buffer, const C* _format, const A&... _args);

}

namespace __me {

  using namespace me;

  enum __format_flags {
    FORMAT_FLAG_SPECIAL = 0x01
  };

  enum __arg_type : uint16_t {
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

    ARG_TYPE_ME_STRING,
    ARG_TYPE_ME_STRING_VIEW,

    ARG_TYPE_CLASS,
  };

  struct __arg {
    const void* _ptr;
    __arg_type _type;
  };

  template<typename C, typename... A>
  constexpr int __format(C* _buffer, const C* _format, const A&... _args);

  template<typename T, typename... A>
  constexpr void __store_args(__arg* _iter, const T &_first, const A&... _args);

  template<typename C>
  __arg_type __get_arg_type(const C* &_format);

  template<typename T, typename C>
  void __integer_to_string(T _i, C* &_str, int _flags, int _base) requires std::is_integral_v<T>;

  template<typename T, typename C>
  void __float_to_string(T _f, C* &_str, int _flags, int _base) requires std::is_floating_point_v<T>;

  template<typename T, typename C>
  void __number_to_string(T _n, C* &_str, int _flags, int _base);

  template<typename C>
  int __string_to_integer(C* &_str);

}

/* "This is a test {0%HH:%MM:%SS}" */
template<typename C, typename... A>
constexpr int __me::__format(C* _buffer, const C* _format, const A&... _args)
{
  constexpr size_t _len = sizeof...(_args);
  __arg _iter[_len];
  __store_args(_iter, _args...);

  size_t _index = 0;

  const char* _val_str;
#ifdef LIBME_STRING_HPP
  const string* _val_me_str;
  const string_view* _val_me_str_view;
#endif

  int _flags;
  int _base;

  /* getting options */
  while (*_format != '\0')
  {
    if (*_format != '{')
    {
      *_buffer++ = *_format++;
      continue;
    }
      _format++;

    /* default */
    _flags = 0;
    _base = 10;

    /* getting index if found */
    if (isdigit(*_format))
      _index = __string_to_integer(_format);

    /* getting current argument */
    const __arg &_arg = _iter[_index];
    __arg_type _arg_type = _arg._type;

    /* getting options */
    do {

      /* print number in 16 bit */
      if (*_format == 'x')
      {
	_format++;
	_base = 16;

      /* print number in 2 bit */
      }else if (*_format == 'b')
      {
	_format++;
	_base = 2;

      /* add special charcters */
      }else if (*_format == '#')
      {
	_format++;
	_flags |= FORMAT_FLAG_SPECIAL;

      /* specify the type manullay */
      }else if (*_format == '%')
      {
	_format++;
	_arg_type = __get_arg_type(_format);
      }

    }while (*_format && *_format != '}');

    switch (_arg._type)
    {
      /* C string type */
      case ARG_TYPE_CSTRING:
        _val_str = reinterpret_cast<const char*>(_arg._ptr);
        while (*_val_str != '\0')
          *_buffer++ = *_val_str++;
        break;

      /* pointer */
      case ARG_TYPE_POINTER:
	__number_to_string(*reinterpret_cast<const size_t*>(_arg._ptr), _buffer, _flags, 16);
	break;

      /* signed integer types */
      case ARG_TYPE_CHAR:
	__number_to_string(*reinterpret_cast<const char*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_SHORT:
	__number_to_string(*reinterpret_cast<const short*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_INT:
	__number_to_string(*reinterpret_cast<const int*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_LONG:
	__number_to_string(*reinterpret_cast<const long*>(_arg._ptr), _buffer, _flags, _base);
	break;

      /* unsigned integer types */
      case ARG_TYPE_UCHAR:
	__number_to_string(*reinterpret_cast<const unsigned char*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_USHORT:
	__number_to_string(*reinterpret_cast<const unsigned short*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_UINT:
	__number_to_string(*reinterpret_cast<const unsigned int*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_ULONG:
	__number_to_string(*reinterpret_cast<const unsigned long*>(_arg._ptr), _buffer, _flags, _base);
	break;

      /* float types */
      case ARG_TYPE_FLOAT:
	__number_to_string(*reinterpret_cast<const float*>(_arg._ptr), _buffer, _flags, _base);
	break;
      case ARG_TYPE_DOUBLE:
	__number_to_string(*reinterpret_cast<const double*>(_arg._ptr), _buffer, _flags, _base);
	break;

      /* libme classes */
#ifdef LIBME_STRING_HPP
      case ARG_TYPE_ME_STRING:
        _val_me_str = reinterpret_cast<const string*>(_arg._ptr);
	for (const char* i = _val_me_str->cbegin(); i != _val_me_str->cend(); i++)
	  *_buffer++ = *i;
	break;
      case ARG_TYPE_ME_STRING_VIEW:
        _val_me_str_view = reinterpret_cast<const string_view*>(_arg._ptr);
	for (const char* i = _val_me_str_view->begin(); i != _val_me_str_view->end(); i++)
	  *_buffer++ = *i;
	break;
#endif

      case ARG_TYPE_CLASS:
	break;

      default:
        break;
    }

    _format++;
    _index++;
  }

  *_buffer++ = '\0';
  return 0;
}

template<typename C>
__me::__arg_type __me::__get_arg_type(const C* &_format)
{
  switch (*_format++)
  {
    case 's':
      return ARG_TYPE_CSTRING;
    case 'c':
      return ARG_TYPE_CHAR;
    case 'p':
      return ARG_TYPE_POINTER;

    /* long types */
    case 'l':
      switch (*_format++)
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
      switch (*_format++)
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
constexpr void __me::__store_args(__arg* _iter, const T &_first, const A&... _args)
{
  typedef std::remove_cvref_t<T> Tr;

  _iter->_ptr = &_first;

  /* array types */
  if (std::is_array<Tr>::value)
  {
    typedef std::remove_extent_t<Tr> Tre;

    if (std::is_same<Tre, char>::value)
      _iter->_type = ARG_TYPE_CSTRING;

  /* pointer types */
  }else if (std::is_pointer<Tr>::value)
  {
    if (std::is_same<Tr, char*>::value)
      _iter->_type = ARG_TYPE_CSTRING;
    else
      _iter->_type = ARG_TYPE_POINTER;

  /* class types */
  }else if (std::is_class<Tr>::value)
  {
#ifdef LIBME_STRING_HPP
    if (std::is_same<Tr, string>::value)
      _iter->_type = ARG_TYPE_ME_STRING;
    else if (std::is_same<Tr, string_view>::value)
      _iter->_type = ARG_TYPE_ME_STRING_VIEW;
#endif

  }else
  {
    /* signed integer */
    if (std::is_same<Tr, char>::value)
      _iter->_type = ARG_TYPE_CHAR;
    else if (std::is_same<Tr, short>::value)
      _iter->_type = ARG_TYPE_SHORT;
    else if (std::is_same<Tr, int>::value)
      _iter->_type = ARG_TYPE_INT;
    else if (std::is_same<Tr, long>::value)
      _iter->_type = ARG_TYPE_LONG;

    /* unsigned integer */
    else if (std::is_same<Tr, unsigned char>::value)
      _iter->_type = ARG_TYPE_UCHAR;
    else if (std::is_same<Tr, unsigned short>::value)
      _iter->_type = ARG_TYPE_USHORT;
    else if (std::is_same<Tr, unsigned int>::value)
      _iter->_type = ARG_TYPE_UINT;
    else if (std::is_same<Tr, unsigned long>::value)
      _iter->_type = ARG_TYPE_ULONG;

    /* floating-point */
    else if (std::is_same<Tr, float>::value)
      _iter->_type = ARG_TYPE_FLOAT;
    else if (std::is_same<Tr, double>::value)
      _iter->_type = ARG_TYPE_DOUBLE;

    else
      throw exception("unknown data type '%s'", typeid(Tr).name());
  }

  if constexpr (sizeof...(_args) > 0)
  {
    _iter++;
    __store_args(_iter, _args...);
  }
}

template<typename T, typename C>
void __me::__integer_to_string(T _i, C* &_str, int _flags, int _base) requires std::is_integral_v<T>
{
  static const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  C _temp[66];
  int _index = 0;
  bool _unsigned;

  if constexpr (std::is_signed_v<T>)
  {
    if (_i < 0)
    {
      _unsigned = true;
      _i = -_i;
    }else
      _unsigned = false;
  }else
    _unsigned = false;

  while (_i)
  {
    T _r = _i % _base;
    _i = _i / _base;

    _temp[_index++] = _CHARS[_r];
  }

  if (_unsigned)
    *_str++ = '-';
  while (_index)
    *_str++ = _temp[(_index--) - 1];
}

template<typename T, typename C>
void __me::__float_to_string(T _f, C* &_str, int _flags, int _base) requires std::is_floating_point_v<T>
{
  static const char* _CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  long _i1 = static_cast<long>(_f);
  long _i2 = static_cast<long>(_f - static_cast<T>(_i1));

  C _dec_point = '.';
  C _temp[128];
  int _index = 0;
  bool _unsigned;

  if (_i1 < 0)
  {
    _unsigned = true;
    _i1 = -_i1;
  }else
    _unsigned = false;

  while (_i1)
  {
    int _r = _i1 % _base;
    _i1 = _i1 / _base;
    _temp[_index++] = _CHARS[_r];
  }

  _temp[_index++] = _dec_point;

  while (_i2)
  {
    int _r = _i2 % _base;
    _i2 = _i2 / _base;
    _temp[_index++] = _CHARS[_r];
  }

  if (_unsigned)
    *_str++ = '-';
  while (_index)
    *_str++ = _temp[(_index--) - 1];
}

template<typename T, typename C>
void __me::__number_to_string(T _n, C* &_str, int _flags, int _base)
{
  if (_flags & FORMAT_FLAG_SPECIAL)
  {
    *_str++ = '0';
    *_str++ = 'x';
  }

  if constexpr (std::is_integral_v<T>)
    __integer_to_string(_n, _str, _flags, _base);
  else if constexpr (std::is_floating_point_v<T>)
    __float_to_string(_n, _str, _flags, _base);
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
  return __me::__format(_buffer, _format, _args...);
}

#endif
