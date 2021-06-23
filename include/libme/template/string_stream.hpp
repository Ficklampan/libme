#ifndef LIBME_STRING_STREAM_TYPE_HPP
  #define LIBME_STRING_STREAM_TYPE_HPP

#include "../type.hpp"

namespace me {

  template<typename T, class A>
  class string_t;

  template<typename T>
  class string_view_t;

  template<typename T>
  class string_stream_t {

  protected:

    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_; // pointer to the first character
    Iterator end_; // pointer to the nullchar
    Iterator iter_;

  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;

    constexpr string_stream_t(Const_Iterator str, Size len);
    constexpr string_stream_t(Const_Iterator begin, Const_Iterator end);
    constexpr string_stream_t(Const_Iterator str);
    constexpr string_stream_t(const string_view_t<T> &str);
    template<class A> constexpr string_stream_t(const string_t<T, A> &str);
    constexpr string_stream_t(const string_stream_t<T> &copy);
    constexpr string_stream_t(string_view_t<T> &&move);
    constexpr string_stream_t();

    constexpr Type next();
    constexpr const string_view_t<T> next_string(Size len);
    template<typename I> constexpr I next_integer();

    Size remaining() const;

  };

}

#include "string.hpp"
#include "string_view.hpp"
#include "string_util.hpp"

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(Const_Iterator str, Size len)
  : begin_(str), end_(str + len)
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(Const_Iterator begin, Const_Iterator end)
  : begin_(begin), end_(end)
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(Const_Iterator str)
  : begin_(str), end_(str + strlen(str))
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(const string_view_t<T> &str)
  : begin_(str.begin()), end_(str.end())
{
  iter_ = begin_;
}

template<typename T>
template<class A>
constexpr me::string_stream_t<T>::string_stream_t(const string_t<T, A> &str)
  : begin_(str.cbegin()), end_(str.cend())
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(const string_stream_t<T> &copy)
  : begin_(copy.begin_), end_(copy.end_)
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t(string_view_t<T> &&move)
  : begin_(static_cast<T&&>(move).begin_), end_(static_cast<T&&>(move).end_)
{
  iter_ = begin_;
}

template<typename T>
constexpr me::string_stream_t<T>::string_stream_t()
  : begin_(nullptr), end_(nullptr)
{
  iter_ = begin_;
}

template<typename T>
constexpr T me::string_stream_t<T>::next()
{
  return *iter_++;
}

template<typename T>
constexpr const me::string_view_t<T> me::string_stream_t<T>::next_string(Size len)
{
  return string_view_t<T>(iter_, iter_ + len);
}

template<typename T>
template<typename I>
constexpr I me::string_stream_t<T>::next_integer()
{
  iter_ = strint<I>(iter_, iter_ - end_);
}

template<typename T>
me::size_t me::string_stream_t<T>::remaining() const
{
  return end_ - iter_;
}

#endif
