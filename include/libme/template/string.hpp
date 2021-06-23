#ifndef LIBME_STRING_TYPE_HPP
  #define LIBME_STRING_TYPE_HPP

#define LIBME_STRING_VALIDATE 1

#include "string_base.hpp"
#include "../allocator.hpp"
#include "../memory.hpp"

#include <string.h>
#include <stdarg.h>

namespace me {

  template<typename T, class A = Allocator>
  class String_T : public StringBase<T> {
    
  protected:

    typedef A Alloc;
    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_; // pointer to the first character
    Iterator end_; // pointer to the nullchar
    Iterator capacity_;

    [[nodiscard]] constexpr Const_Iterator __begin() const override;
    [[nodiscard]] constexpr Const_Iterator __end() const override;
    [[nodiscard]] constexpr Size __size() const override;
  
  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;
  
    constexpr String_T(Const_Iterator str, Size len);
    constexpr String_T(Type chr, Size len);
    constexpr String_T(Const_Iterator string);
    constexpr String_T(const StringView_T<T> &str);
    constexpr String_T(const String_T<T, A> &copy);
    constexpr String_T(String_T<T, A> &&move);
    constexpr String_T();
    constexpr ~String_T();
  
    [[nodiscard]] constexpr Iterator data() const;
    [[nodiscard]] constexpr Iterator begin() const;
    [[nodiscard]] constexpr Iterator end() const;
    [[nodiscard]] constexpr Const_Iterator cbegin() const;
    [[nodiscard]] constexpr Const_Iterator cend() const;
    constexpr void reserve(Size cap);
 
    constexpr String_T& insert(Size pos, const StringView_T<T> &str);
    constexpr String_T& append(const StringView_T<T> &str);
    constexpr String_T& append(Type chr);
    constexpr String_T& push_back(Type chr);
    constexpr Type pop_back();
    constexpr void erase(Iterator first, Iterator last);
    constexpr void erase(Iterator pos);
    constexpr void clear();
    constexpr void replace(Iterator first, Iterator last, const StringView_T<T> &str);
    constexpr void to_lowercase(Iterator begin, Iterator end);
    constexpr void to_uppercase(Iterator begin, Iterator end);

    [[nodiscard]] constexpr Const_Iterator c_str() const;
    [[nodiscard]] constexpr Size capacity() const;
    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;

    [[nodiscard]] constexpr Type& at(Size pos) const;
    [[nodiscard]] constexpr const Type& operator[](Size pos) const;

    template<class A2> [[nodiscard]] constexpr bool operator==(const String_T<T, A2> &str) const;
    [[nodiscard]] constexpr bool operator==(const StringView_T<T> &str) const;
    [[nodiscard]] constexpr bool operator==(Const_Iterator str) const;
  
    constexpr String_T& operator+=(Type chr);
    constexpr String_T& operator+=(const StringView_T<T> &str);
  
    constexpr String_T& operator=(const String_T<T, A> &copy);
    constexpr String_T& operator=(String_T<T, A> &&move);
    constexpr String_T& operator=(Const_Iterator str);
    constexpr String_T& operator=(const StringView_T<T> &str);
 
  };
 
}

inline me::String_T<char, me::Allocator> operator "" _s(const char* str, me::size_t len);

#include "string_view.hpp"
#include "string_util.hpp"
#include "../format.hpp"

#include <stdio.h>

template<typename T, class A>
constexpr me::String_T<T, A>::String_T(Const_Iterator str, Size len)
{
  begin_ = Alloc::template malloc<Type>(len + 1);
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *str++;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::String_T<T, A>::String_T(Type chr, Size len)
{
  begin_ = Alloc::template malloc<Type>(len + 1);
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = chr;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::String_T<T, A>::String_T(Const_Iterator str)
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
constexpr me::String_T<T, A>::String_T(const StringView_T<T> &str)
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
constexpr me::String_T<T, A>::String_T(const String_T<T, A> &copy)
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
constexpr me::String_T<T, A>::String_T(String_T<T, A> &&move)
{
  begin_ = static_cast<String_T<T, A>&&>(move).begin_;
  end_ = static_cast<String_T<T, A>&&>(move).end_;
  capacity_ = static_cast<String_T<T, A>&&>(move).capacity_;
}

template<typename T, class A>
constexpr me::String_T<T, A>::String_T()
{
  begin_ = (Iterator) Alloc::template malloc<Type>(1);
  end_ = begin_;
  capacity_ = end_;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr me::String_T<T, A>::~String_T()
{
  if (this->capacity() != 0)
  {
#if LIBME_STRING_VALIDATE == 1
    if (begin_ == nullptr)
      puts("me::String_T::destroy(): freeing nullptr");
#endif
    Alloc::free(begin_);
  }
}

template<typename T, class A>
constexpr const T* me::String_T<T, A>::__begin() const
{
  return begin_;
}

template<typename T, class A>
constexpr const T* me::String_T<T, A>::__end() const
{
  return end_;
}

template<typename T, class A>
constexpr size_t me::String_T<T, A>::__size() const
{
  return end_ - begin_;
}

template<typename T, class A>
constexpr T* me::String_T<T, A>::data() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::String_T<T, A>::begin() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::String_T<T, A>::end() const
{
  return end_;
}

template<typename T, class A>
constexpr const T* me::String_T<T, A>::cbegin() const
{
  return begin_;
}

template<typename T, class A>
constexpr const T* me::String_T<T, A>::cend() const
{
  return end_;
}

template<typename T, class A>
constexpr void me::String_T<T, A>::reserve(Size cap)
{
  if (cap > this->capacity())
  {
    Size count = this->size();

    Iterator begin = (Iterator) Alloc::template malloc<Type>(cap);
#if LIBME_STRING_VALIDATE == 1
    if (begin == nullptr)
      printf("me::String_T::reserve(%lu): me::Allocator::template malloc<Type> returned nullptr\n", cap);
#endif
    memory::uninitialized_copy<Type>(begin, begin_, end_);
    Alloc::free(begin_);
    begin_ = begin;
    end_ = begin + count;
    capacity_ = begin + cap;
  }
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::insert(Size pos, const StringView_T<T> &str)
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
constexpr me::String_T<T, A>& me::String_T<T, A>::append(const StringView_T<T> &str)
{
  this->reserve(this->size() + str.size() + 1);

  Const_Iterator iter = str.begin();

  while (iter != str.end())
    *end_++ = *iter++;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::append(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::push_back(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T, class A>
constexpr T me::String_T<T, A>::pop_back()
{
  end_--;
  Type chr = *end_;
  *end_ = nullchar;
  return chr;
}

template<typename T, class A>
constexpr void me::String_T<T, A>::erase(Iterator begin, Iterator end)
{
#if LIBME_STRING_VALIDATE == 1
  if (begin < begin_ || end > end_)
    printf("me::String_T::erase(%p, %p): out of range", begin, end);
#endif

  Size len = (end - begin);
  Size tail_len = (end_ - end);

  while (tail_len--)
    *begin++ = *end++;
  end_ -= len;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr void me::String_T<T, A>::erase(Iterator pos)
{
  this->erase(pos, pos + 1);
}

template<typename T, class A>
constexpr void me::String_T<T, A>::clear()
{
  end_ = begin_;
  *end_ = nullchar;
}

template<typename T, class A>
constexpr void me::String_T<T, A>::replace(Iterator begin, Iterator end, const StringView_T<T> &str)
{
  // TODO: faster?

  this->erase(begin, end);
  this->insert(begin - begin_, str);
}

template<typename T, class A>
constexpr void me::String_T<T, A>::to_lowercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = lowercase(*begin);
}

template<typename T, class A>
constexpr void me::String_T<T, A>::to_uppercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = uppercase(*begin);
}

template<typename T, class A>
constexpr const T* me::String_T<T, A>::c_str() const
{
  return begin_;
}

template<typename T, class A>
constexpr me::size_t me::String_T<T, A>::capacity() const
{
  return capacity_ - begin_;
}

template<typename T, class A>
constexpr me::size_t me::String_T<T, A>::size() const
{
  return end_ - begin_;
}

template<typename T, class A>
constexpr bool me::String_T<T, A>::is_empty() const
{
  return end_ == begin_;
}

template<typename T, class A>
constexpr T& me::String_T<T, A>::at(Size pos) const
{
  return begin_[pos];
}

template<typename T, class A>
constexpr const T& me::String_T<T, A>::operator[](Size pos) const
{
  return begin_[pos];
}

template<typename T, class A>
template<class A2>
constexpr bool me::String_T<T, A>::operator==(const String_T<T, A2> &str) const
{
  return strcmp(begin_, str.begin_) == 0;
}

template<typename T, class A>
constexpr bool me::String_T<T, A>::operator==(const StringView_T<T> &str) const
{
  return str == begin_;
}

template<typename T, class A>
constexpr bool me::String_T<T, A>::operator==(Const_Iterator str) const
{
  return StringView_T<T>(*this).operator==(str);
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator+=(Type chr)
{
  this->append(chr);
  return *this;
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator+=(const StringView_T<T> &str)
{
  this->append(str);
  return *this;
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator=(const String_T<T, A> &copy)
{
  this->clear();
  return this->append(copy);
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator=(String_T<T, A> &&move)
{
  begin_ = static_cast<String_T<T, A>&&>(move).begin_;
  end_ = static_cast<String_T<T, A>&&>(move).end_;
  capacity_ = static_cast<String_T<T, A>&&>(move).capacity_;
  return *this;
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator=(Const_Iterator str)
{
  this->clear();
  return this->append(str);
}

template<typename T, class A>
constexpr me::String_T<T, A>& me::String_T<T, A>::operator=(const StringView_T<T> &str)
{
  this->clear();
  return this->append(str);
}

inline me::String_T<char, me::Allocator> operator "" _s(const char* str, me::size_t len)
{
  return me::String_T<char, me::Allocator>(str, len);
}

LIBME_NEW_FORMATTER(String_T<char>,
{
  const String_T<char> &str = *reinterpret_cast<const String_T<char>*>(value);
  append_string(buffer, str.c_str());
})

#endif
