#ifndef LIBME_STRING_BASE_HPP
  #define LIBME_STRING_BASE_HPP

#include "../type.hpp"

namespace me {

  template<typename T>
  class StringBase;

  template<typename T, class A>
  class String_T;

  template<typename T>
  class StringView_T;

  template<typename T>
  class StringBase {

  protected:

    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    [[nodiscard]] virtual constexpr Const_Iterator __begin() const = 0;
    [[nodiscard]] virtual constexpr Const_Iterator __end() const = 0;
    [[nodiscard]] virtual constexpr Size __size() const = 0;

  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;

    constexpr void split(Type delimiter, Size &len, StringView_T<T>* strs) const;
    [[nodiscard]] constexpr int as_int(Size off = 0, int base = 10) const;
    [[nodiscard]] constexpr bool equals(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool equals_ignore_case(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool starts_with(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool ends_with(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool starts_with_ignore_case(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool ends_with_ignore_case(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr Size find(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size find(const StringView_T<T> &str, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(const StringView_T<T> &str, Size off = 0) const;
    [[nodiscard]] constexpr StringView_T<T> substr(Size begin, Size end) const;
    [[nodiscard]] constexpr StringView_T<T> substr(Size begin) const;
    constexpr void copy(Size off, Size len, Iterator dst) const;

  };

}

#include <string.h>

template<typename T>
constexpr void me::StringBase<T>::split(Type delimiter, Size &len, StringView_T<T>* strs) const
{
  if (strs == nullptr)
  {
    len = 1;
    for (Const_Iterator i = __begin(); *i != nullchar; i++)
    {
      if (*i == delimiter)
	len++;
    }
  }else
  {
    Const_Iterator first = __begin();
    for (Const_Iterator i = __begin(); *i != nullchar; i++)
    {
      if (*i == delimiter)
      {
	*strs++ = StringView_T<T>(first, i);
	first = i + 1;
      }
    }

    *strs++ = StringView_T<T>(first, __end());
  }
}

template<typename T>
constexpr int me::StringBase<T>::as_int(Size off, int base) const
{
  return strint<int>(__begin() + off, __size(), base);
}

template<typename T>
constexpr bool me::StringBase<T>::equals(const StringView_T<T> &str) const
{
  if (__size() != str.__size())
    return false;

  Const_Iterator iter1 = __begin();
  Const_Iterator iter2 = str.__begin();

  while (iter1 != __end())
  {
    if (*iter1++ != *iter2++)
      return false;
  }
  return true;
}

template<typename T>
constexpr bool me::StringBase<T>::equals_ignore_case(const StringView_T<T> &str) const
{
  if (__size() != str.__size())
    return false;

  Const_Iterator iter1 = __begin();
  Const_Iterator iter2 = str.__begin();

  while (iter1 != __end())
  {
    if (uppercase(*iter1++) != uppercase(*iter2++))
      return false;
  }
  return true;
}

template<typename T>
constexpr bool me::StringBase<T>::starts_with(const StringView_T<T> &str) const
{
  return __size() >= str.__size() && ::memcmp(__begin(), str.__begin(), str.__size()) == 0;
}

template<typename T>
constexpr bool me::StringBase<T>::ends_with(const StringView_T<T> &str) const
{
  return __size() >= str.__size() && ::memcmp(__end() - str.__size(), str.__begin(), str.__size()) == 0;
}

template<typename T>
constexpr bool me::StringBase<T>::starts_with_ignore_case(const StringView_T<T> &str) const
{
  if (__size() < str.__size())
    return false;

  Const_Iterator iter1 = __begin();
  Const_Iterator iter2 = str.__begin();
  while (iter2 != str.end())
  {
    if (uppercase(*iter1++) != uppercase(*iter2++))
      return false;
  }
  return true;
}

template<typename T>
constexpr bool me::StringBase<T>::ends_with_ignore_case(const StringView_T<T> &str) const
{
  if (__size() < str.__size())
    return false;

  Const_Iterator iter1 = __end() - str.__size();
  Const_Iterator iter2 = str.__begin();
  while (iter2 != str.end())
  {
    if (uppercase(*iter1++) != uppercase(*iter2++))
      return false;
  }
  return true;
}

template<typename T>
constexpr me::size_t me::StringBase<T>::find(Type chr, Size off) const
{
  Const_Iterator iter = __begin() + off;
  while (iter != this->__end())
  {
    if (*iter == chr)
      return iter - __begin();
    iter++;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::StringBase<T>::rfind(Type chr, Size off) const
{
  Const_Iterator iter = __end() - 1 - off;
  while (iter != __begin() + 1)
  {
    if (*iter == chr)
      return iter - __begin();
    iter--;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::StringBase<T>::find(const StringView_T<T> &str, Size off) const
{
  if (__size() - off < str.__size())
    return npos;

  Const_Iterator iter = __begin() + off;
  while (iter != __end() - str.__size() + 1)
  {
    if (::memcmp(iter, str.__begin(), str.__size()) == 0)
      return iter - __begin();
    iter++;
  }
  return npos;
}

template<typename T>
constexpr me::size_t me::StringBase<T>::rfind(const StringView_T<T> &str, Size off) const
{
  if (__size() - off < str.__size())
    return npos;

  Const_Iterator iter = __end() - str.__size() - off;
  while (iter != __begin() - 1)
  {
    if (::memcmp(iter, str.__begin(), str.__size()) == 0)
      return iter - __begin();
    iter--;
  }
  return npos;
}

template<typename T>
constexpr me::StringView_T<T> me::StringBase<T>::substr(Size off, Size end) const
{
  return StringView_T<T>(__begin() + off, __begin() + off + end);
}

template<typename T>
constexpr me::StringView_T<T> me::StringBase<T>::substr(Size off) const
{
  return StringView_T<T>(__begin() + off, __end());
}

template<typename T>
constexpr void me::StringBase<T>::copy(Size off, Size len, Iterator dst) const
{
  for (Size i = 0; i < len; i++)
    dst[i] = __begin()[i + off];
}

#endif
