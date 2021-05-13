#ifndef LIBME_STRING_HPP
  #define LIBME_STRING_HPP

#include "type.hpp"
#include "template/string.hpp"
#include "template/string_view.hpp"
#include "template/string_proxy.hpp"
#include "template/string_util.hpp"

#include <wchar.h>

namespace me {

  typedef string_t<char> string;
  typedef string_t<wchar_t> wstring;

  typedef string_view_t<char> string_view;
  typedef string_view_t<wchar_t> wstring_view;

  template<class S>
  using string_proxy = _string_proxy_t<char, S>;

  template<class S>
  using wstring_proxy = _string_proxy_t<wchar_t, S>;

}

#endif
