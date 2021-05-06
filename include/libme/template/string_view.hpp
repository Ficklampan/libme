#ifndef LIBME_STRING_VIEW_TYPE_HPP
  #define LIBME_STRING_VIEW_TYPE_HPP

#define LIBME_STRING_VIEW_VALIDATE 1

#include "../type.hpp"

#include <stdarg.h>

namespace me {

  template<typename T>
  class string_t;

  template<typename T>
  class string_view_t {

  protected:

    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Const_Iterator begin_; /* first character */
    Const_Iterator end_; /* last character + 1 */

  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;

    constexpr string_view_t(Const_Iterator str, Size len);
    constexpr string_view_t(Const_Iterator begin, Const_Iterator end);
    constexpr string_view_t(Const_Iterator str);
    constexpr string_view_t(const string_t<T> &str);
    constexpr string_view_t(const string_view_t<T> &copy);
    constexpr string_view_t(string_view_t<T> &&move);
    constexpr string_view_t();

    [[nodiscard]] constexpr Const_Iterator data() const;
    [[nodiscard]] constexpr Const_Iterator begin() const;
    [[nodiscard]] constexpr Const_Iterator end() const;

    [[nodiscard]] constexpr Type at(Size pos) const;

    template<typename I> [[nodiscard]] constexpr I as_int(Size off = 0, int base = 10) const;
    template<bool Seek> constexpr void split(Type delimiter, Size &len, string_view_t<T>* strs) const;
    [[nodiscard]] constexpr bool starts_with(const string_view_t &str) const;
    [[nodiscard]] constexpr bool ends_with(const string_view_t &str) const;
    [[nodiscard]] constexpr Size find(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size find(const string_view_t &str, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(const string_view_t &str, Size off = 0) const;
    [[nodiscard]] constexpr string_view_t substr(Size begin, Size end) const;
    [[nodiscard]] constexpr string_view_t substr(Size begin) const;
    constexpr void copy(Size off, Size len, Iterator dst) const;

    constexpr Iterator c_str(Iterator str, Size off) const;
    constexpr Iterator c_str(Iterator str) const;
    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;
    [[nodiscard]] constexpr bool is_valid() const;

    [[nodiscard]] constexpr Type operator[](Size pos) const;

    [[nodiscard]] constexpr bool operator==(const string_view_t &_str) const;
    [[nodiscard]] constexpr bool operator==(Const_Iterator _str) const;

    constexpr string_view_t& operator=(const string_view_t &copy);
    constexpr string_view_t& operator=(string_view_t &&move);
    constexpr string_view_t& operator=(Const_Iterator str);

  };

}
inline me::string_view_t<char> operator "" _sv(const char* str, me::size_t len);

#include "string.hpp"
#include "string_util.hpp"
#include "../exception.hpp"

#include <malloc.h>

template<typename T>
constexpr me::string_view_t<T>::string_view_t(Const_Iterator str, Size len)
  : begin_(str), end_(str + len)
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t(Const_Iterator begin, Const_Iterator end)
  : begin_(begin), end_(end)
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t(Const_Iterator str)
  : begin_(str), end_(str + strlen(str))
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t(const string_t<T> &str)
  : begin_(str.begin()), end_(str.end())
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t(const string_view_t<T> &copy)
  : begin_(copy.begin_), end_(copy.end_)
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t(string_view_t<T> &&move)
  : begin_(static_cast<string_view_t<T>&&>(move).begin_), end_(static_cast<string_view_t<T>&&>(move).end_)
{
}

template<typename T>
constexpr me::string_view_t<T>::string_view_t()
  : begin_(nullptr), end_(nullptr)
{
}

template<typename T>
constexpr const T* me::string_view_t<T>::data() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::string_view_t<T>::begin() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::string_view_t<T>::end() const
{
  return end_;
}

template<typename T>
constexpr T me::string_view_t<T>::at(Size pos) const
{
  return begin_[pos];
}

template<typename T>
template<typename I>
constexpr I me::string_view_t<T>::as_int(Size off, int base) const
{
  return strint<I>(begin_ + off, this->size(), base);
}

template<typename T>
template<bool Seek>
constexpr void me::string_view_t<T>::split(Type delimiter, Size &len, string_view_t<T>* strs) const
{
  if constexpr (Seek)
  {
    len = 1;
    for (Const_Iterator i = begin_; *i != nullchar; i++)
    {
      if (*i == delimiter)
	len++;
    }
  }else
  {
    Const_Iterator first = begin_;
    for (Const_Iterator i = begin_; *i != nullchar; i++)
    {
      if (*i == delimiter)
      {
	*strs++ = string_view_t<T>(first, i);
	first = i + 1;
      }
    }

    *strs++ = string_view_t<T>(first, end_);
  }
}

template<typename T>
constexpr bool me::string_view_t<T>::starts_with(const string_view_t &str) const
{
  return this->size() >= str.size() && ::memcmp(begin_, str.begin(), str.size()) == 0;
}

template<typename T>
constexpr bool me::string_view_t<T>::ends_with(const string_view_t &str) const
{
  return this->size() >= str.size() && ::memcmp(end_ - str.size(), str.begin(), str.size()) == 0;
}

template<typename T>
constexpr me::size_t me::string_view_t<T>::find(Type chr, Size off) const
{
  Const_Iterator iter = begin_ + off;
  while (iter != end_)
  {
    if (*iter == chr)
      return iter - begin_;
    iter++;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::string_view_t<T>::rfind(Type chr, Size off) const
{
  Const_Iterator iter = end_ - 1 - off;
  while (iter != begin_ + 1)
  {
    if (*iter == chr)
      return iter - begin_;
    iter--;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::string_view_t<T>::find(const string_view_t<T> &str, Size off) const
{
  if (this->size() - off < str.size())
    return npos;

  Const_Iterator iter = begin_ + off;
  while (iter != end_ - str.size() + 1)
  {
    if (::memcmp(iter, str.begin(), str.size()) == 0)
      return iter - begin_;
    iter++;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::string_view_t<T>::rfind(const string_view_t<T> &str, Size off) const
{
  if (this->size() - off < str.size())
    return npos;

  Const_Iterator iter = end_ - str.size() - off;
  while (iter != begin_ - 1)
  {
    if (::memcmp(iter, str.begin(), str.size()) == 0)
      return iter - begin_;
    iter--;
  }
  return npos;
}

template<typename T>
constexpr me::string_view_t<T> me::string_view_t<T>::substr(Size off, Size end) const
{
  return string_view_t<T>(begin_ + off, begin_ + off + end);
}

template<typename T>
constexpr me::string_view_t<T> me::string_view_t<T>::substr(Size off) const
{
  return string_view_t<T>(begin_ + off, end_);
}

template<typename T>
constexpr void me::string_view_t<T>::copy(Size off, Size len, Iterator dst) const
{
  for (Size i = 0; i < len; i++)
    dst[i] = begin_[i + off];
}

template<typename T>
constexpr T* me::string_view_t<T>::c_str(Iterator str, Size off) const
{
  Size len = this->size() - off;

  for (Size i = 0; i < len; i++)
    str[i] = begin_[i + off];

  str[len] = nullchar;
  return str;
}

template<typename T>
constexpr T* me::string_view_t<T>::c_str(Iterator str) const
{
  Size len = this->size();

  for (Size i = 0; i < len; i++)
    str[i] = begin_[i];

  str[len] = nullchar;
  return str;
}

template<typename T>
constexpr me::size_t me::string_view_t<T>::size() const
{
  return end_ - begin_;
}

template<typename T>
constexpr bool me::string_view_t<T>::is_empty() const
{
  return begin_ != end_;
}

template<typename T>
constexpr bool me::string_view_t<T>::is_valid() const
{
  return begin_ != nullptr;
}

template<typename T>
constexpr T me::string_view_t<T>::operator[](Size pos) const
{
  return this->at(pos);
}

template<typename T>
constexpr bool me::string_view_t<T>::operator==(const string_view_t &str) const
{
  if (str.size() != this->size())
    return false;

  for (Size i = 0; i < this->size(); i++)
  {
    if (begin_[i] != str[i])
      return false;
  }
  return true;
}

template<typename T>
constexpr bool me::string_view_t<T>::operator==(Const_Iterator str) const
{
  if (strlen(str) != this->size())
    return false;

  for (Size i = 0; i < this->size(); i++)
  {
    if (begin_[i] != str[i])
      return false;
  }
  return true;
}

template<typename T>
constexpr me::string_view_t<T>& me::string_view_t<T>::operator=(const string_view_t<T> &str)
{
  begin_ = str.begin_;
  end_ = str.end_;
  return *this;
}

template<typename T>
constexpr me::string_view_t<T>& me::string_view_t<T>::operator=(string_view_t<T> &&str)
{
  begin_ = static_cast<string_view_t<T>&&>(str).begin_;
  end_ = static_cast<string_view_t<T>&&>(str).end_;
  return *this;
}

template<typename T>
constexpr me::string_view_t<T>& me::string_view_t<T>::operator=(Const_Iterator str)
{
  Size len = strlen(str);

  begin_ = str;
  end_ = str + len;
  return *this;
}

inline me::string_view_t<char> operator "" _sv(const char* str, me::size_t len)
{
  return me::string_view_t<char>(str, len);
}

#endif
