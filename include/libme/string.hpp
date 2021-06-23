#ifndef LIBME_STRING_HPP
  #define LIBME_STRING_HPP

#include "type.hpp"
#include "template/string.hpp"
#include "template/string_view.hpp"
#include "template/string_stream.hpp"
#include "template/string_util.hpp"

#include <wchar.h>

namespace me {

  typedef String_T<char, Allocator> String;
  typedef String_T<wchar_t, Allocator> WString;

  typedef StringView_T<char> StringView;
  typedef StringView_T<wchar_t> WStringView;

  typedef string_stream_t<char> StringStream;
  typedef string_stream_t<wchar_t> WStringStream;

}

#endif
