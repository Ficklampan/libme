#ifndef LIBME_FORMAT_HPP
  #define LIBME_FORMAT_HPP

#include "type.hpp"

namespace me::format {

  template<typename Char>
  class Param final {

  public:

    typedef const void* Value;
    typedef void (WriteFunction) (Char* &buffer, Value value);

    Value value;
    WriteFunction* func;

  };

  template<typename Char, size_t Count>
  class ParamArray final {

  private:

    Param<Char> params_[Count];

  public:

    constexpr ParamArray(const auto&... args);

    template<typename First>
    constexpr void store_param(Param<Char>* param, const First &first, const auto& ...args) const;

    constexpr const Param<Char>& at(size_t index) const;

  };

  template<typename Char, typename Type>
  class Formatter {

  public:

    static constexpr void write(Char* &buffer, const void* value);

  };

  template<typename Char>
  constexpr void print(Char* buffer, const Char* format, const auto&... args);

}

template<typename Char, size_t Count>
constexpr me::format::ParamArray<Char, Count>::ParamArray(const auto&... args)
{
  if constexpr (sizeof...(args) != 0)
    store_param(params_, args...);
}

template<typename Char, size_t Count>
template<typename First>
constexpr void me::format::ParamArray<Char, Count>::store_param(Param<Char>* param, const First &first, const auto& ...args) const
{
  param->value = &first;
  param->func = Formatter<Char, First>::write;

  if constexpr (sizeof...(args) != 0)
    store_param(param + 1, args...);
}

template<typename Char, size_t Count>
constexpr const me::format::Param<Char>& me::format::ParamArray<Char, Count>::at(size_t index) const
{
  return params_[index];
}

template<typename Char, typename Type>
constexpr void me::format::Formatter<Char, Type>::write(Char* &buffer, const void* value)
{
  (void)value;
  *buffer++ = '?';
}

template<typename Char>
constexpr void me::format::print(Char* buffer, const Char* format, const auto&... args)
{
  ParamArray<Char, sizeof...(args)> params(args...);

  size_t index = 0;
  while (*format)
  {
    if (*format == '{')
    {
      format++;
      while (*format++ != '}');

      const Param<Char> &param = params.at(index);
      param.func(buffer, param.value);
    }else
      *buffer++ = *format++;
  }
  *buffer++ = '\0';
}

template<typename Char, size_t N>
class me::format::Formatter<Char, char[N]> {

public:

  static constexpr void write(Char* &buffer, const void* value)
  {
    const char* str = reinterpret_cast<const char*>(value);
    while (*str)
      *buffer++ = *str++;
  }

};

#endif



//#ifndef LIBME_FORMAT_HPP
//  #define LIBME_FORMAT_HPP
//
//#include "exception.hpp"
//#include "format_helper.hpp"
//
//namespace me::format {
//
//  class 
//
//  typedef char Char;
//
//  class Param {
//
//  public:
//
//    enum class Type : uint8_t {
//      NONE,
//      CUSTOM,
//      INT8,
//      INT16,
//      INT32,
//      INT64,
//      UINT8,
//      UINT16,
//      UINT32,
//      UINT64,
//      FLOAT,
//      DOUBLE,
//      CHAR_ARRAY,
//      CHAR_POINTER,
//      POINTER
//    };
//
//    typedef const void* Value;
//    typedef void (*Function) (FormatOptions&, ParameterArray&, Char*&, Value);
//
//  private:
//
//    Value value_;
//    Function function_;
//
//  public:
//
//    ME_ALWAYS_INLINE constexpr Parameter() = default;
//
//    ME_ALWAYS_INLINE constexpr Value get_value() const { return value_; }
//    ME_ALWAYS_INLINE constexpr Function get_function() const { return function_; }
//
//    ME_ALWAYS_INLINE constexpr int as_int() const
//    {
//      //switch (type_)
//      //{
//      //  case Type::INT8:
//      //    return static_cast<int>(*reinterpret_cast<const int8_t*>(value_));
//      //  case Type::INT16:
//      //    return static_cast<int>(*reinterpret_cast<const int16_t*>(value_));
//      //  case Type::INT32:
//      //    return static_cast<int>(*reinterpret_cast<const int32_t*>(value_));
//      //  case Type::INT64:
//      //    return static_cast<int>(*reinterpret_cast<const int64_t*>(value_));
//      //  case Type::UINT8:
//      //    return static_cast<int>(*reinterpret_cast<const uint8_t*>(value_));
//      //  case Type::UINT16:
//      //    return static_cast<int>(*reinterpret_cast<const uint16_t*>(value_));
//      //  case Type::UINT32:
//      //    return static_cast<int>(*reinterpret_cast<const uint32_t*>(value_));
//      //  case Type::UINT64:
//      //    return static_cast<int>(*reinterpret_cast<const uint64_t*>(value_));
//      //  default:
//      //    return 0;
//      //}
//      return 0;
//    }
//
//    ME_ALWAYS_INLINE constexpr void set_value(Value value) { value_ = value; }
//    ME_ALWAYS_INLINE constexpr void set_function(Function function) { function_ = function; }
//
//  };
//
//  class ParamArray {
//
//  private:
//
//    Parameter* parameters_;
//    size_t index_;
//
//    template<typename First> constexpr void store(Parameter* iter, const First &first, const auto&... rest);
//
//  public:
//
//    template<typename... Args> constexpr ParamArray(const Args&... args);
//
//    ME_ALWAYS_INLINE constexpr void set_index(size_t index) { index_ = index; }
//    ME_ALWAYS_INLINE constexpr size_t next_index() { return index_++; }
//
//    ME_ALWAYS_INLINE constexpr const Parameter* get_parameter(size_t index) const { return parameters_ + index; }
//
//  };
//
//  class Options {
//
//  private:
//
//    static constexpr const char* digits_uppercase = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//    static constexpr const char* digits_lowercase = "0123456789abcdefghijklmnopqrstuvwxyz";
//
//    bool changed = true;
//
//    bool digit_uppercase_;
//    bool special_;
//    int base_;
//    char sign_; /* '-', '+', ' ' */
//    char allign_; /* '<', '^', '>' */
//    Char padding_char_;
//    size_t width_;
//
//    const Parameter* parameter_;
//    /* function to execute when Parameter::print is called */
//    Parameter::Function function_;
//
//  public:
//
//    constexpr FormatOptions() = default;
//
//    ME_ALWAYS_INLINE constexpr void set_defaults()
//    {
//      if (!changed)
//	return;
//      digit_uppercase_ = false;
//      special_ = false;
//      base_ = 10;
//      sign_ = '-';
//      allign_ = '<';
//      padding_char_ = ' ';
//      width_ = 0;
//
//      changed = false;
//    }
//
//    ME_ALWAYS_INLINE constexpr bool is_digit_uppercase() const { return digit_uppercase_; }
//    ME_ALWAYS_INLINE constexpr bool is_special() const { return special_; }
//    ME_ALWAYS_INLINE constexpr const char* get_digits() const { return digit_uppercase_ ? digits_uppercase : digits_lowercase; }
//    ME_ALWAYS_INLINE constexpr int get_base() const { return base_; }
//    ME_ALWAYS_INLINE constexpr char get_sign() const { return sign_; }
//    ME_ALWAYS_INLINE constexpr char get_allign() const { return allign_; }
//    ME_ALWAYS_INLINE constexpr char get_padding_char() const { return padding_char_; }
//    ME_ALWAYS_INLINE constexpr size_t get_width() const { return width_; }
//
//    ME_ALWAYS_INLINE constexpr const Parameter* get_parameter() const { return parameter_; }
//    ME_ALWAYS_INLINE constexpr Parameter::Function get_function() const { return function_; }
//
//    ME_ALWAYS_INLINE constexpr void set_digit_uppercase(bool digit_uppercase) { changed = true; digit_uppercase_ = digit_uppercase; }
//    ME_ALWAYS_INLINE constexpr void set_special(bool special) { changed = true; special_ = special; }
//    ME_ALWAYS_INLINE constexpr void set_base(int base) { changed = true; base_ = base; }
//    ME_ALWAYS_INLINE constexpr void set_sign(char sign) { changed = true; sign_ = sign; }
//    ME_ALWAYS_INLINE constexpr void set_allign(char allign) { changed = true; allign_ = allign; }
//    ME_ALWAYS_INLINE constexpr void set_padding_char(char padding_char) { changed = true; padding_char_ = padding_char; }
//    ME_ALWAYS_INLINE constexpr void set_width(size_t width) { changed = true; width_ = width; }
//
//    ME_ALWAYS_INLINE constexpr void set_parameter(const Parameter* parameter) { parameter_ = parameter; function_ = parameter->get_function(); }
//    ME_ALWAYS_INLINE constexpr void set_function(Parameter::Function function) { function_ = function; }
//
//  };
//
//  ME_ALWAYS_INLINE static constexpr void append_string(Char* &buffer, const Char* str)
//  {
//    while (*str)
//      *buffer++ = *str++;
//  }
//
//  ME_ALWAYS_INLINE static constexpr void append_string(Char* &buffer, const Char* str, size_t len)
//  {
//    for (size_t i = 0; i != len && *str; i++)
//      *buffer++ = str[i];
//  }
//
//  ME_ALWAYS_INLINE static constexpr bool next_if(const Char* &format, Char c)
//  {
//    if (*format == c)
//    {
//      format++;
//      return true;
//    }
//    return false;
//  }
//
//  ME_ALWAYS_INLINE static constexpr int read_int(const Char* &format, ParameterArray &parameters)
//  {
//    (void)parameters;
//    if (*format == '*')
//    {
//      //return parameters.get_parameter(parameters.next_index()).as_int();
//      return 0;
//    }
//
//    int i = 0;
//
//    while (isdigit(*format))
//      i = 10 * i + *format++ - '0';
//    return i;
//  }
//
//  template<typename Type>
//  struct Formatter {
//    ME_ALWAYS_INLINE static void __print(FormatOptions &options, ParameterArray &params, Char* &builder, const void* value)
//    {
//      (void)options;
//      (void)params;
//      (void)value;
//      (void)builder;
//      throw RuntimeError("me::format::Parameter::print(...) unknown parameter type");
//    }
//  };
//
//  template<typename... Args>
//  constexpr void print(Char* buffer, const Char* format_begin, const Char* format_end, const Args&... args);
//
//  template<typename... Args>
//  constexpr void print(FILE* file, const Char* format, const Args&... args);
//
//  template<typename... Args>
//  constexpr void print(const Char* format, const Args&... args);
//
//  constexpr void parse_options(const Char* &format, FormatOptions &options, ParameterArray &parameters);
//  constexpr void parse_standard_options(const Char* &format, FormatOptions &options, ParameterArray &parameters);
//
//}
//
//LIBME_NEW_INTEGER_FORMATTER(int8_t)
//LIBME_NEW_INTEGER_FORMATTER(int16_t)
//LIBME_NEW_INTEGER_FORMATTER(int32_t)
//LIBME_NEW_INTEGER_FORMATTER(int64_t)
//LIBME_NEW_INTEGER_FORMATTER(uint8_t)
//LIBME_NEW_INTEGER_FORMATTER(uint16_t)
//LIBME_NEW_INTEGER_FORMATTER(uint32_t)
//LIBME_NEW_INTEGER_FORMATTER(uint64_t)
//LIBME_NEW_FLOAT_FORMATTER(float)
//LIBME_NEW_FLOAT_FORMATTER(double)
//
//namespace me::format {
//  template<size_t N>
//  struct Formatter<char[N]> {
//    ME_ALWAYS_INLINE static void __print(FormatOptions &options, ParameterArray &params, Char* &buffer, const void* value)
//    {
//      (void)params;
//      if (options.get_width() == 0)
//	append_string(buffer, *reinterpret_cast<const char* const*>(value));
//      else
//	append_string(buffer, *reinterpret_cast<const char* const*>(value), options.get_width());
//    }
//  };
//  template<>
//  struct Formatter<const char*> {
//    ME_ALWAYS_INLINE static void __print(FormatOptions &options, ParameterArray &params, Char* &buffer, const void* value)
//    {
//      (void)params;
//      if (options.get_width() == 0)
//	append_string(buffer, *reinterpret_cast<const char* const*>(value));
//      else
//	append_string(buffer, *reinterpret_cast<const char* const*>(value), options.get_width());
//    }
//  };
//}
//
//template<typename... Args>
//constexpr me::format::ParameterArray::ParameterArray(const Args&... args)
//  : parameters_(new Parameter[sizeof...(args)]), index_(0)
//{
//  if constexpr (sizeof...(args) != 0)
//    store(parameters_, args...);
//}
//
//template<typename First>
//constexpr void me::format::ParameterArray::store(Parameter* iter, const First &first, const auto&... rest)
//{
//  typedef std::remove_cvref_t<First> Tr;
//
//  iter->set_function(Formatter<Tr>::__print);
//  iter->set_value(&first);
//
//  if constexpr (sizeof...(rest) != 0)
//    store(iter + 1, rest...);
//}
//
//template<typename... Args>
//constexpr void me::format::print_buffer(Char* buffer, const Char* format, const Args&... args)
//{
//  ParameterArray parameters(args...);
//  FormatOptions options;
//  Char param_string[1024];
//  Char* param_buffer;
//
//  while (*format)
//  {
//    if (next_if(format, '{'))
//    {
//      options.set_defaults();
//      options.set_parameter(parameters.get_parameter(parameters.next_index()));
//      parse_options(format, options, parameters);
//
//      param_buffer = param_string;
//      options.get_function()(options, parameters, param_buffer, options.get_parameter()->get_value());
//
//      size_t len = param_buffer - param_string;
//      if (options.get_width() > len)
//      {
//	size_t padding = options.get_width() - len;
//
//	/* left padding */
//	if (options.get_allign() == '<')
//	{
//	  for (size_t i = 0; i != padding; i++)
//	    *buffer++ = options.get_padding_char();
//	  append_string(buffer, param_string, len);
//
//	/* right padding */
//	}else if (options.get_allign() == '>')
//	{
//	  append_string(buffer, param_string, len);
//	  for (size_t i = 0; i != padding; i++)
//	    *buffer++ = options.get_padding_char();
//
//	/* center padding */
//	/* FIXME: stuff not center if length is odd or even */
//	}else if (options.get_allign() == '^')
//	{
//	  padding /= 2;
//	  for (size_t i = 0; i != padding; i++)
//	    *buffer++ = options.get_padding_char();
//	  append_string(buffer, param_string, len);
//	  for (size_t i = 0; i != padding; i++)
//	    *buffer++ = options.get_padding_char();
//	}
//      }else
//	append_string(buffer, param_string, len);
//
//    }else
//      *buffer++ = *format++;
//  }
//  *buffer++ = '\0';
//}
//
//template<typename... Args>
//constexpr void me::format::print(FILE* file, const Char* format, const Args&... args)
//{
//  char buffer[1024];
//  print_buffer(buffer, format, args...);
//  fputs(buffer, file);
//}
//
//template<typename... Args>
//constexpr void me::format::print(const Char* format, const Args&... args)
//{
//  print(stdout, format, args...);
//}
//
//constexpr void me::format::parse_options(const Char* &format, FormatOptions &options, ParameterArray &parameters)
//{
//  while (*format)
//  {
//    if (next_if(format, '}'))
//      break;
//
//    if (isdigit(*format))
//    {
//      parameters.set_index(read_int(format, parameters));
//    }else if (next_if(format, ':'))
//    {
//      parse_standard_options(format, options, parameters);
//      break;
//    }else
//      format++;
//  }
//}
//
//constexpr void me::format::parse_standard_options(const Char* &format, FormatOptions &options, ParameterArray &parameters)
//{
//  while (*format)
//  {
//    if (next_if(format, '}'))
//      break;
//
//    if (next_if(format, 'p'))
//    {
//      options.set_base(16);
//      options.set_width(12);
//      options.set_padding_char(0);
//      options.set_function(Formatter<ptrdiff_t>::__print);
//
//    }else if (next_if(format, 'P'))
//    {
//      options.set_base(16);
//      options.set_width(12);
//      options.set_padding_char(0);
//      options.set_digit_uppercase(true);
//      options.set_function(Formatter<ptrdiff_t>::__print);
//    }else if (next_if(format, 'x'))
//    {
//      options.set_base(16);
//      options.set_digit_uppercase(false);
//    }else if (next_if(format, 'X'))
//    {
//      options.set_base(16);
//      options.set_digit_uppercase(true);
//    }else if (next_if(format, 'U'))
//      options.set_digit_uppercase(true);
//    else if (next_if(format, 'L'))
//      options.set_digit_uppercase(false);
//    else if (next_if(format, '#'))
//      options.set_special(true);
//    else if (next_if(format, '+'))
//      options.set_sign('+');
//    else if (next_if(format, ' '))
//      options.set_sign(' ');
//    else if (next_if(format, '-'))
//      options.set_allign('<');
//    else if (next_if(format, '<'))
//      options.set_allign('<');
//    else if (next_if(format, '^'))
//      options.set_allign('^');
//    else if (next_if(format, '>'))
//      options.set_allign('>');
//    else if (next_if(format, '_'))
//      options.set_padding_char(*format++);
//    else if (next_if(format, '0'))
//      options.set_padding_char('0');
//    else if (isdigit(*format))
//      options.set_width(read_int(format, parameters));
//    else
//      format++;
//  }
//}
//
//#endif
