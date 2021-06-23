#ifndef LIBME_FORMAT_HELPER_HPP
  #define LIBME_FORMAT_HELPER_HPP

#include "type.hpp"

#include <type_traits>

namespace me::format {

  template<typename T>
  struct is_char_array : public std::false_type {
  };

  template<size_t S>
  struct is_char_array<char[S]> : public std::true_type {
  };

#define LIBME_NEW_FORMATTER(t, pf) \
  namespace me::format { \
  template<> \
  struct Formatter<t> { \
    ME_ALWAYS_INLINE static void __print( \
	[[maybe_unused]] FormatOptions &options, \
	[[maybe_unused]] ParameterArray &params, \
	[[maybe_unused]] Char* &buffer, \
	const void* value) \
      pf \
  }; \
  }

  template<typename T, typename Char>
  static constexpr void write_integer(T i, Char* &buffer, int base, char sign, const char* digits)
  {
    char temp[66];
    int index = 0;
    bool unsign;

    if (i == 0)
    {
      *buffer++ = '0';
      return;
    }

    if constexpr (std::is_signed_v<T>)
    {
      if (i < 0)
      {
        unsign = true;
        i = -i;
      }else
        unsign = false;
    }else
      unsign = false;

    while (i)
    {
      T r = i % base;
      i = i / base;

      temp[index++] = digits[r];
    }

    if (unsign)
      *buffer++ = '-';
    else if (sign == '+')
      *buffer++ = '+';
    else if (sign == ' ')
      *buffer++ = ' ';

    while (index)
      *buffer++ = temp[(index--) - 1];
  }

  template<typename T, typename Char>
  static constexpr void write_float(T f, Char* &buffer, int base, char sign, const char* digits)
  {
    (void)f;
    (void)buffer;
    (void)base;
    (void)sign;
    (void)digits;
  }

#define LIBME_NEW_INTEGER_FORMATTER(t) \
  LIBME_NEW_FORMATTER(t, \
  { \
    write_integer(*reinterpret_cast<const t*>(value), buffer, options.get_base(), options.get_sign(), options.get_digits()); \
  })

#define LIBME_NEW_FLOAT_FORMATTER(t) \
  LIBME_NEW_FORMATTER(t, \
  { \
    write_float(*reinterpret_cast<const t*>(value), buffer, options.get_base(), options.get_sign(), options.get_digits()); \
  })


}

#endif
