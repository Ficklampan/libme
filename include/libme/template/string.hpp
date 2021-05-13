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

  template<typename T>
  class string_t {
    
  protected:

    typedef allocator Alloc_Type;
    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_; // pointer to the first character
    Iterator end_; // pointer to the nullchar
    Iterator capacity_;
    Alloc_Type* alloc_;
  
  public:

    static constexpr T nullchar = 0;
    static constexpr T npos = ~0;
  
    constexpr string_t(Const_Iterator str, Size len, Alloc_Type* alloc = allocator::_default());
    constexpr string_t(Type chr, Size len, Alloc_Type* alloc = allocator::_default());
    constexpr string_t(Const_Iterator string, Alloc_Type* alloc = allocator::_default());
    constexpr string_t(const string_view_t<T> &str, Alloc_Type* alloc = allocator::_default());
    constexpr string_t(const string_t &copy);
    constexpr string_t(string_t &&move);
    constexpr string_t(Alloc_Type* alloc = allocator::_default());
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
    constexpr void replace(Iterator first, Iterator last, const string_view_t<T> &_str);
    constexpr void to_lowercase(Iterator begin, Iterator end);
    constexpr void to_uppercase(Iterator begin, Iterator end);

    [[nodiscard]] constexpr Type& at(Size pos) const;

    template<typename I> [[nodiscard]] constexpr I as_int(Size off = 0, uint8_t base = 10) const;
    template<bool Seek> constexpr void split(Type delimiter, Size &len, string_view_t<T>* strs) const;
    [[nodiscard]] constexpr bool starts_with(const string_view_t<T> &_str) const;
    [[nodiscard]] constexpr bool ends_with(const string_view_t<T> &str) const;
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

    [[nodiscard]] constexpr bool operator==(const string_t &str) const;
    [[nodiscard]] constexpr bool operator==(const string_view_t<T> &str) const;
    [[nodiscard]] constexpr bool operator==(Const_Iterator str) const;
  
    constexpr string_t& operator+=(Type chr);
    constexpr string_t& operator+=(const string_view_t<T> &str);
  
    constexpr string_t& operator=(const string_t &copy);
    constexpr string_t& operator=(string_t &&move);
    constexpr string_t& operator=(Const_Iterator str);
 
  };
 
}
inline me::string_t<char> operator "" _s(const char* str, me::size_t len);

#include "string_view.hpp"
#include "string_util.hpp"

#include <stdio.h>

template<typename T>
constexpr me::string_t<T>::string_t(Const_Iterator str, Size len, Alloc_Type* alloc)
  : alloc_(alloc)
{
  begin_ = (Iterator) alloc_->malloc((len + 1) * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *str++;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::string_t(Type chr, Size len, Alloc_Type* alloc)
  : alloc_(alloc)
{
  begin_ = (Iterator) alloc_->malloc((len + 1) * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = chr;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::string_t(Const_Iterator str, Alloc_Type* alloc)
  : alloc_(alloc)
{
  Size len = strlen(str);

  begin_ = (Iterator) alloc_->malloc((len + 1) * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *str++;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::string_t(const string_view_t<T> &str, Alloc_Type* alloc)
  : alloc_(alloc)
{
  Const_Iterator iter = str.begin();

  begin_ = (Iterator) alloc_->malloc((str.size() + 1) * sizeof(Type));
  end_ = begin_ + str.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *iter++;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::string_t(const string_t<T> &copy)
  : alloc_(copy.alloc_)
{
  Const_Iterator iter = copy.begin();

  begin_ = (Iterator) alloc_->malloc((copy.size() + 1) * sizeof(Type));
  end_ = begin_ + copy.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
     *i = *iter++;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::string_t(string_t<T> &&move)
{
  begin_ = static_cast<string_t<T>&&>(move).begin_;
  end_ = static_cast<string_t<T>&&>(move).end_;
  capacity_ = static_cast<string_t<T>&&>(move).capacity_;
  alloc_ = static_cast<string_t<T>&&>(move).alloc_;
}

template<typename T>
constexpr me::string_t<T>::string_t(Alloc_Type* alloc)
  : alloc_(alloc)
{
  begin_ = (Iterator) alloc_->malloc(sizeof(Type));
  end_ = begin_;
  capacity_ = end_;
  *end_ = nullchar;
}

template<typename T>
constexpr me::string_t<T>::~string_t()
{
  if (this->capacity() != 0)
  {
#if LIBME_STRING_VALIDATE == 1
    if (begin_ == nullptr)
      puts("me::string_t::destroy(): freeing nullptr");
#endif
    alloc_->free(begin_);
  }
}

template<typename T>
constexpr T* me::string_t<T>::data() const
{
  return begin_;
}

template<typename T>
constexpr T* me::string_t<T>::begin() const
{
  return begin_;
}

template<typename T>
constexpr T* me::string_t<T>::end() const
{
  return end_;
}

template<typename T>
constexpr const T* me::string_t<T>::cbegin() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::string_t<T>::cend() const
{
  return end_;
}

template<typename T>
constexpr void me::string_t<T>::reserve(Size cap)
{
  if (cap > this->capacity())
  {
    Size count = this->size();

    Iterator begin = (Iterator) alloc_->malloc(cap * sizeof(Type));
#if LIBME_STRING_VALIDATE == 1
    if (begin == nullptr)
      printf("me::string_t::reserve(%lu): me::allocator::malloc returned nullptr\n", cap);
#endif
    memory::uninitialized_copy<Type>(begin, begin_, end_);
    alloc_->free(begin_);
    begin_ = begin;
    end_ = begin + count;
    capacity_ = begin + cap;
  }
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::insert(Size pos, const string_view_t<T> &str)
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

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::append(const string_view_t<T> &str)
{
  this->reserve(this->size() + str.size() + 1);

  Const_Iterator iter = str.begin();

  while (iter != str.end())
    *end_++ = *iter++;
  *end_ = nullchar;
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::append(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::push_back(Type chr)
{
  this->reserve(this->size() + 2);

  *end_++ = chr;
  *end_ = nullchar;
  return *this;
}

template<typename T>
constexpr T me::string_t<T>::pop_back()
{
  end_--;
  Type chr = *end_;
  *end_ = nullchar;
  return chr;
}

template<typename T>
constexpr void me::string_t<T>::erase(Iterator begin, Iterator end)
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

template<typename T>
constexpr void me::string_t<T>::erase(Iterator pos)
{
  this->erase(pos, pos + 1);
}

template<typename T>
constexpr void me::string_t<T>::clear()
{
  end_ = begin_;
  *end_ = nullchar;
}

template<typename T>
constexpr void me::string_t<T>::replace(Iterator begin, Iterator end, const string_view_t<T> &str)
{
  // TODO: faster?

  this->erase(begin, end);
  this->insert(begin - begin_, str);
}

template<typename T>
constexpr void me::string_t<T>::to_lowercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = lowercase(*begin);
}

template<typename T>
constexpr void me::string_t<T>::to_uppercase(Iterator begin, Iterator end)
{
  while (begin != end)
    *begin++ = uppercase(*begin);
}

template<typename T>
constexpr T& me::string_t<T>::at(Size pos) const
{
  return begin_[pos];
}

template<typename T>
template<typename I>
constexpr I me::string_t<T>::as_int(Size off, uint8_t base) const
{
  return strint<I>(begin_ + off, base);
}

template<typename T>
template<bool Seek>
constexpr void me::string_t<T>::split(Type delimiter, Size &len, string_view_t<T>* strs) const
{
  return split_string<Seek>(*this, delimiter, len, strs);
}

template<typename T>
constexpr bool me::string_t<T>::starts_with(const string_view_t<T> &str) const
{
  return this->size() >= str.size() && ::memcmp(begin_, str.begin(), str.size()) == 0;
}

template<typename T>
constexpr bool me::string_t<T>::ends_with(const string_view_t<T> &str) const
{
  return this->size() >= str.size() && ::memcmp(end_ - str.size(), str.begin(), str.size()) == 0;
}

template<typename T>
constexpr me::size_t me::string_t<T>::find(Type chr, Size off) const
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
constexpr me::size_t me::string_t<T>::rfind(Type chr, Size off) const
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
constexpr me::size_t me::string_t<T>::find(const string_view_t<T> &str, Size off) const
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
constexpr me::size_t me::string_t<T>::rfind(const string_view_t<T> &str, Size off) const
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
constexpr me::string_view_t<T> me::string_t<T>::substr(Size off, Size end) const
{
  return string_view_t<T>(begin_ + off, begin_ + off + end);
}

template<typename T>
constexpr me::string_view_t<T> me::string_t<T>::substr(Size off) const
{
  return string_view_t<T>(begin_ + off, end_);
}

template<typename T>
constexpr void me::string_t<T>::copy(Size off, Size len, Iterator dst) const
{
  for (Iterator i = begin_ + off; i != begin_ + off + len; i++)
    *dst++ = *i;
}

template<typename T>
constexpr const T* me::string_t<T>::c_str() const
{
  return begin_;
}

template<typename T>
constexpr me::size_t me::string_t<T>::capacity() const
{
  return capacity_ - begin_;
}

template<typename T>
constexpr me::size_t me::string_t<T>::size() const
{
  return end_ - begin_;
}

template<typename T>
constexpr bool me::string_t<T>::is_empty() const
{
  return end_ == begin_;
}

template<typename T>
constexpr const T& me::string_t<T>::operator[](Size pos) const
{
  return begin_[pos];
}

template<typename T>
constexpr bool me::string_t<T>::operator==(const string_t<T> &str) const
{
  return strcmp(begin_, str.begin_) == 0;
}

template<typename T>
constexpr bool me::string_t<T>::operator==(const string_view_t<T> &str) const
{
  return str == begin_;
}

template<typename T>
constexpr bool me::string_t<T>::operator==(Const_Iterator str) const
{
  return string_equals(begin_, str);
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::operator+=(Type chr)
{
  this->append(chr);
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::operator+=(const string_view_t<T> &str)
{
  this->append(str);
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::operator=(const string_t<T> &str)
{
  Size len = str.size();
  Const_Iterator iter = str.begin();

  alloc_ = str.alloc_;
  begin_ = (Iterator) alloc_->malloc((len + 1) * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;

  for (Iterator i = begin_; i != end_; i++)
    *i = *iter++;
  *end_ = nullchar;
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::operator=(string_t<T> &&str)
{
  begin_ = static_cast<string_t<T>&&>(str).begin_;
  end_ = static_cast<string_t<T>&&>(str).end_;
  capacity_ = static_cast<string_t<T>&&>(str).capacity_;
  alloc_ = static_cast<string_t<T>&&>(str).alloc_;
  return *this;
}

template<typename T>
constexpr me::string_t<T>& me::string_t<T>::operator=(Const_Iterator str)
{
  this->clear();
  return this->append(str);
}

inline me::string_t<char> operator "" _s(const char* str, me::size_t len)
{
  return me::string_t<char>(str, len);
}

#endif
