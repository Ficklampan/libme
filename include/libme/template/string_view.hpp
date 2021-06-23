#ifndef LIBME_STRING_VIEW_TYPE_HPP
  #define LIBME_STRING_VIEW_TYPE_HPP

#define LIBME_STRING_VIEW_VALIDATE 1

#include "string_base.hpp"

namespace me {

  template<typename T>
  class StringView_T : public StringBase<T> {

  protected:

    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Const_Iterator begin_; /* first character */
    Const_Iterator end_; /* last character + 1 */

    [[nodiscard]] constexpr Const_Iterator __begin() const override;
    [[nodiscard]] constexpr Const_Iterator __end() const override;
    [[nodiscard]] constexpr Size __size() const override;

  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;

    constexpr StringView_T(Const_Iterator str, Size len);
    constexpr StringView_T(Const_Iterator begin, Const_Iterator end);
    constexpr StringView_T(Const_Iterator str);
    template<class A> constexpr StringView_T(const String_T<T, A> &str);
    constexpr StringView_T(const StringView_T<T> &copy);
    constexpr StringView_T(StringView_T<T> &&move);
    constexpr StringView_T();

    [[nodiscard]] constexpr Const_Iterator data() const;
    [[nodiscard]] constexpr Const_Iterator begin() const;
    [[nodiscard]] constexpr Const_Iterator end() const;

    constexpr Iterator c_str(Iterator str, Size off) const;
    constexpr Iterator c_str(Iterator str) const;
    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;
    [[nodiscard]] constexpr bool is_valid() const;

    [[nodiscard]] constexpr Type at(Size pos) const;
    [[nodiscard]] constexpr Type operator[](Size pos) const;

    [[nodiscard]] constexpr bool operator==(const StringView_T<T> &_str) const;
    [[nodiscard]] constexpr bool operator==(Const_Iterator _str) const;

    constexpr StringView_T& operator=(const StringView_T<T> &copy);
    constexpr StringView_T& operator=(StringView_T<T> &&move);
    constexpr StringView_T& operator=(Const_Iterator str);

  };

}
inline me::StringView_T<char> operator "" _sv(const char* str, me::size_t len);

#include "string.hpp"
#include "string_util.hpp"
#include "../format.hpp"

#include <malloc.h>

template<typename T>
constexpr me::StringView_T<T>::StringView_T(Const_Iterator str, Size len)
  : begin_(str), end_(str + len)
{
}

template<typename T>
constexpr me::StringView_T<T>::StringView_T(Const_Iterator begin, Const_Iterator end)
  : begin_(begin), end_(end)
{
}

template<typename T>
constexpr me::StringView_T<T>::StringView_T(Const_Iterator str)
  : begin_(str), end_(str + strlen(str))
{
}

template<typename T>
template<class A>
constexpr me::StringView_T<T>::StringView_T(const String_T<T, A> &str)
  : begin_(str.begin()), end_(str.end())
{
}

template<typename T>
constexpr me::StringView_T<T>::StringView_T(const StringView_T<T> &copy)
  : begin_(copy.begin_), end_(copy.end_)
{
}

template<typename T>
constexpr me::StringView_T<T>::StringView_T(StringView_T<T> &&move)
  : begin_(static_cast<StringView_T<T>&&>(move).begin_), end_(static_cast<StringView_T<T>&&>(move).end_)
{
}

template<typename T>
constexpr me::StringView_T<T>::StringView_T()
  : begin_(nullptr), end_(nullptr)
{
}

template<typename T>
constexpr const T* me::StringView_T<T>::__begin() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::StringView_T<T>::__end() const
{
  return end_;
}

template<typename T>
constexpr size_t me::StringView_T<T>::__size() const
{
  return end_ - begin_;
}

template<typename T>
constexpr const T* me::StringView_T<T>::data() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::StringView_T<T>::begin() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::StringView_T<T>::end() const
{
  return end_;
}

template<typename T>
constexpr T* me::StringView_T<T>::c_str(Iterator str, Size off) const
{
  Size len = this->size() - off;

  for (Size i = 0; i < len; i++)
    str[i] = begin_[i + off];

  str[len] = nullchar;
  return str;
}

template<typename T>
constexpr T* me::StringView_T<T>::c_str(Iterator str) const
{
  Size len = this->size();

  for (Size i = 0; i < len; i++)
    str[i] = begin_[i];

  str[len] = nullchar;
  return str;
}

template<typename T>
constexpr me::size_t me::StringView_T<T>::size() const
{
  return end_ - begin_;
}

template<typename T>
constexpr bool me::StringView_T<T>::is_empty() const
{
  return begin_ == end_;
}

template<typename T>
constexpr bool me::StringView_T<T>::is_valid() const
{
  return begin_ != nullptr;
}

template<typename T>
constexpr T me::StringView_T<T>::at(Size pos) const
{
  return begin_[pos];
}

template<typename T>
constexpr T me::StringView_T<T>::operator[](Size pos) const
{
  return this->at(pos);
}

template<typename T>
constexpr bool me::StringView_T<T>::operator==(const StringView_T &str) const
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
constexpr bool me::StringView_T<T>::operator==(Const_Iterator str) const
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
constexpr me::StringView_T<T>& me::StringView_T<T>::operator=(const StringView_T<T> &str)
{
  begin_ = str.begin_;
  end_ = str.end_;
  return *this;
}

template<typename T>
constexpr me::StringView_T<T>& me::StringView_T<T>::operator=(StringView_T<T> &&str)
{
  begin_ = static_cast<StringView_T<T>&&>(str).begin_;
  end_ = static_cast<StringView_T<T>&&>(str).end_;
  return *this;
}

template<typename T>
constexpr me::StringView_T<T>& me::StringView_T<T>::operator=(Const_Iterator str)
{
  Size len = strlen(str);

  begin_ = str;
  end_ = str + len;
  return *this;
}

inline me::StringView_T<char> operator "" _sv(const char* str, me::size_t len)
{
  return me::StringView_T<char>(str, len);
}

LIBME_NEW_FORMATTER(StringView_T<char>,
{
  const StringView_T<char> &str = *reinterpret_cast<const StringView_T<char>*>(value);
  append_string(buffer, str.data(), str.size());
})

#endif
