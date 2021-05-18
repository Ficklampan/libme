#ifndef LIBME_STRING_TYPE_HPP
  #define LIBME_STRING_TYPE_HPP

#define LIBME_STRING_VALIDATE 1

#include "../type.hpp"
#include "../allocator.hpp"
#include "../memory.hpp"

#include <string.h>
#include <stdarg.h>

namespace me {

  template<typename T>
  class string_view_t;

  template<typename T, class A = allocator>
  class string_t {
    
  protected:

    typedef A Alloc;
    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_; // pointer to the first character
    Iterator end_; // pointer to the nullchar
    Iterator capacity_;
  
  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;
  
    constexpr string_t(Const_Iterator str, Size len);
    constexpr string_t(Type chr, Size len);
    constexpr string_t(Const_Iterator string);
    constexpr string_t(const string_view_t<T> &str);
    constexpr string_t(const string_t<T, A> &copy);
    constexpr string_t(string_t<T, A> &&move);
    constexpr string_t();
    constexpr ~string_t();
  
    [[nodiscard]] constexpr Iterator data() const;
    [[nodiscard]] constexpr Iterator begin() const;
    [[nodiscard]] constexpr Iterator end() const;
    [[nodiscard]] constexpr Const_Iterator cbegin() const;
    [[nodiscard]] constexpr Const_Iterator cend() const;
    constexpr void reserve(Size cap);
 
    constexpr string_t& insert(Size pos, const string_view_t<T> &str);
    constexpr string_t& append(const string_view_t<T> &str);
    constexpr string_t& append(Type chr);
    constexpr string_t& push_back(Type chr);
    constexpr Type pop_back();
    constexpr void erase(Iterator first, Iterator last);
    constexpr void erase(Iterator pos);
    constexpr void clear();
    constexpr void replace(Iterator first, Iterator last, const string_view_t<T> &str);
    constexpr void to_lowercase(Iterator begin, Iterator end);
    constexpr void to_uppercase(Iterator begin, Iterator end);

    [[nodiscard]] constexpr Type& at(Size pos) const;

    template<typename I> [[nodiscard]] constexpr I as_int(Size off = 0, uint8_t base = 10) const;
    template<bool Seek> constexpr void split(Type delimiter, Size &len, string_view_t<T>* strs) const;
    [[nodiscard]] constexpr bool equals(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool equals_ignore_case(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool starts_with(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool ends_with(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool starts_with_ignore_case(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool ends_with_ignore_case(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr Size find(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(Type chr, Size off = 0) const;
    [[nodiscard]] constexpr Size find(const string_view_t<T> &str, Size off = 0) const;
    [[nodiscard]] constexpr Size rfind(const string_view_t<T> &str, Size off = 0) const;
    [[nodiscard]] constexpr string_view_t<T> substr(Size begin, Size end) const;
    [[nodiscard]] constexpr string_view_t<T> substr(Size begin) const;
    constexpr void copy(Size off, Size len, Iterator dst) const;
  
    [[nodiscard]] constexpr Const_Iterator c_str() const;
    [[nodiscard]] constexpr Size capacity() const;
    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;

    [[nodiscard]] constexpr const Type& operator[](Size pos) const;

    template<class A2> [[nodiscard]] constexpr bool operator==(const string_t<T, A2> &str) const;
    [[nodiscard]] constexpr bool operator==(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool operator==(Const_Iterator str) const;
  
    constexpr string_t& operator+=(Type chr);
    constexpr string_t& operator+=(const string_view_t<T> &str);
  
    constexpr string_t& operator=(const string_t<T, A> &copy);
    constexpr string_t& operator=(string_t<T, A> &&move);
    constexpr string_t& operator=(Const_Iterator str);
    constexpr string_t& operator=(const string_view_t<T> &str);
 
  };
 
}

inline me::string_t<char, me::allocator> operator "" _s(const char* str, me::size_t len);

#include "string_view.hpp"
#include "string_util.hpp"

#include <stdio.h>

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(Const_Iterator str, Size len)
{
  begin_ = Alloc::template malloc<Type>(len + 1);
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *str++;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(Type chr, Size len)
{
  begin_ = Alloc::template malloc<Type>(len + 1);
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = chr;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(Const_Iterator str)
{
  Size len = strlen(str);

  begin_ = Alloc::template malloc<Type>(len + 1);
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *str++;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(const string_view_t<T> &str)
{
  Const_Iterator iter = str.begin();

  begin_ = Alloc::template malloc<Type>(str.size() + 1);
  end_ = begin_ + str.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *iter++;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(const string_t<T, A> &copy)
{
  Const_Iterator iter = copy.begin();

  begin_ = Alloc::template malloc<Type>(copy.size() + 1);
  end_ = begin_ + copy.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *iter++;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t(string_t<T, A> &&move)
{
  begin_ = static_cast<string_t<T, A>&&>(move).begin_;
  end_ = static_cast<string_t<T, A>&&>(move).end_;
  capacity_ = static_cast<string_t<T, A>&&>(move).capacity_;
}

template<typename T, class A>
constexpr me::string_t<T, A>::string_t()
{
  begin_ = (Iterator) Alloc::template malloc<Type>(1);
  end_ = begin_;
  capacity_ = end_;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::string_t<T, A>::~string_t()
{
  if (this->capacity() != 0)
  {
#if LIBME_STRING_VALIDATE == 1
    if (begin_ == nullptr)
      puts("me::string_t::destroy(): freeing nullptr");
#endif
    Alloc::free(begin_);
  }
}

template<typename T, class A>
constexpr T* me::string_t<T, A>::data() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::string_t<T, A>::begin() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::string_t<T, A>::end() const
{
  return end_;
}

template<typename T, class A>
constexpr const T* me::string_t<T, A>::cbegin() const
{
  return begin_;
}

template<typename T, class A>
constexpr const T* me::string_t<T, A>::cend() const
{
  return end_;
}

template<typename T, class A>
constexpr void me::string_t<T, A>::reserve(Size cap)
{
  if (cap > this->capacity())
  {
    Size count = this->size();

    Iterator begin = (Iterator) Alloc::template malloc<Type>(cap);
#if LIBME_STRING_VALIDATE == 1
    if (begin == nullptr)
      printf("me::string_t::reserve(%lu): me::allocator::template malloc<Type> returned nullptr\n", cap);
#endif
    memory::uninitialized_copy<Type>(begin, begin_, end_);
    Alloc::free(begin_);
    begin_ = begin;
    end_ = begin + count;
    capacity_ = begin + cap;
  }
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::insert(Size pos, const string_view_t<T> &str)
{
  this->reserve(this->size() + str.size() + 1);

  Iterator src = begin_ + pos;
  Iterator dest = end_ + str.size();
  Const_Iterator iter = end_;
  while (iter != src - 1)
    *dest-- = *iter--;
  for (Size i = 0; i != str.size(); i++)
    src[i] = str.at(i);
  end_ += str.size();
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::append(const string_view_t<T> &str)
{
  this->reserve(this->size() + str.size() + 1);

  Const_Iterator iter = str.begin();

  while (iter != str.end())
    *end_++ = *iter++;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::append(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::push_back(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr T me::string_t<T, A>::pop_back()
{
  end_--;
  Type chr = *end_;
  *end_ = nullchar;
  return chr;
}

template<typename T, class A>
constexpr void me::string_t<T, A>::erase(Iterator begin, Iterator end)
{
#if LIBME_STRING_VALIDATE == 1
  if (begin < begin_ || end > end_)
    printf("me::string_t::erase(%p, %p): out of range", begin, end);
#endif

  Size len = (end - begin);
  Size tail_len = (end_ - end);

  while (tail_len--)
    *begin++ = *end++;
  end_ -= len;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr void me::string_t<T, A>::erase(Iterator pos)
{
  this->erase(pos, pos + 1);
}

template<typename T, class A>
constexpr void me::string_t<T, A>::clear()
{
  end_ = begin_;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr void me::string_t<T, A>::replace(Iterator begin, Iterator end, const string_view_t<T> &str)
{
  // TODO: faster?

  this->erase(begin, end);
  this->insert(begin - begin_, str);
}

template<typename T, class A>
constexpr void me::string_t<T, A>::to_lowercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = lowercase(*begin);
}

template<typename T, class A>
constexpr void me::string_t<T, A>::to_uppercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = uppercase(*begin);
}

template<typename T, class A>
constexpr T& me::string_t<T, A>::at(Size pos) const
{
  return begin_[pos];
}

template<typename T, class A>
template<typename I>
constexpr I me::string_t<T, A>::as_int(Size off, uint8_t base) const
{
  return strint<I>(begin_ + off, base);
}

template<typename T, class A>
template<bool Seek>
constexpr void me::string_t<T, A>::split(Type delimiter, Size &len, string_view_t<T>* strs) const
{
  return string_view_t<T>(*this).split(delimiter, len, strs);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::equals(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).equals(str);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::equals_ignore_case(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).equals_ignore_case(str);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::starts_with(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).starts_with(str);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::ends_with(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).ends_with(str);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::starts_with_ignore_case(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).starts_with_ignore_case(str);
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::ends_with_ignore_case(const string_view_t<T> &str) const
{
  return string_view_t<T>(*this).ends_with_ignore_case(str);
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::find(Type chr, Size off) const
{
  return string_view_t<T>(*this).find(chr, off);
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::rfind(Type chr, Size off) const
{
  return string_view_t<T>(*this).rfind(chr, off);
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::find(const string_view_t<T> &str, Size off) const
{
  return string_view_t<T>(*this).find(str, off);
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::rfind(const string_view_t<T> &str, Size off) const
{
  return string_view_t<T>(*this).rfind(str, off);
}

template<typename T, class A>
constexpr me::string_view_t<T> me::string_t<T, A>::substr(Size off, Size end) const
{
  return string_view_t<T>(begin_ + off, begin_ + off + end);
}

template<typename T, class A>
constexpr me::string_view_t<T> me::string_t<T, A>::substr(Size off) const
{
  return string_view_t<T>(begin_ + off, end_);
}

template<typename T, class A>
constexpr void me::string_t<T, A>::copy(Size off, Size len, Iterator dst) const
{
  for (Iterator i = begin_ + off; i != begin_ + off + len; i++)
    *dst++ = *i;
}

template<typename T, class A>
constexpr const T* me::string_t<T, A>::c_str() const
{
  return begin_;
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::capacity() const
{
  return capacity_ - begin_;
}

template<typename T, class A>
constexpr me::size_t me::string_t<T, A>::size() const
{
  return end_ - begin_;
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::is_empty() const
{
  return end_ == begin_;
}

template<typename T, class A>
constexpr const T& me::string_t<T, A>::operator[](Size pos) const
{
  return begin_[pos];
}

template<typename T, class A>
template<class A2>
constexpr bool me::string_t<T, A>::operator==(const string_t<T, A2> &str) const
{
  return strcmp(begin_, str.begin_) == 0;
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::operator==(const string_view_t<T> &str) const
{
  return str == begin_;
}

template<typename T, class A>
constexpr bool me::string_t<T, A>::operator==(Const_Iterator str) const
{
  return string_view_t<T>(*this).operator==(str);
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator+=(Type chr)
{
  this->append(chr);
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator+=(const string_view_t<T> &str)
{
  this->append(str);
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator=(const string_t<T, A> &copy)
{
  this->clear();
  return this->append(copy);
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator=(string_t<T, A> &&move)
{
  begin_ = static_cast<string_t<T, A>&&>(move).begin_;
  end_ = static_cast<string_t<T, A>&&>(move).end_;
  capacity_ = static_cast<string_t<T, A>&&>(move).capacity_;
  return *this;
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator=(Const_Iterator str)
{
  this->clear();
  return this->append(str);
}

template<typename T, class A>
constexpr me::string_t<T, A>& me::string_t<T, A>::operator=(const string_view_t<T> &str)
{
  this->clear();
  return this->append(str);
}

inline me::string_t<char, me::allocator> operator "" _s(const char* str, me::size_t len)
{
  return me::string_t<char, me::allocator>(str, len);
}

#endif
