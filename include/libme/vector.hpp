#ifndef LIBME_VECTOR_HPP
  #define LIBME_VECTOR_HPP

#define LIBME_VECTOR_FILE "vector.hpp"

#include "type.hpp"
#include "allocator.hpp"
#include "memory.hpp"
#include "exception.hpp"

#include <initializer_list>
#include <type_traits>

namespace me {

  template<typename T, class A = allocator>
  class vector {

  protected:

    typedef A Alloc;
    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_;
    Iterator end_;
    Iterator capacity_;
    
  public:

    [[deprecated]] constexpr vector(Iterator begin, Iterator end, Iterator capacity);
    constexpr vector(Iterator begin, Size lengh, Size _capacity);
    constexpr vector(Iterator begin, Size lengh);
    constexpr vector(Type &value, Size _length);
    constexpr vector(std::initializer_list<T> elements);
    constexpr vector(Size length);
    constexpr vector(const vector<T, A> &copy);
    constexpr vector(vector<T, A> &&move);
    constexpr vector();
    constexpr ~vector();

    [[nodiscard]] constexpr Iterator data() const;

    [[nodiscard]] constexpr Iterator begin() const;
    [[nodiscard]] constexpr Iterator end() const;

    [[nodiscard]] constexpr Const_Iterator cbegin() const;
    [[nodiscard]] constexpr Const_Iterator cend() const;

    [[nodiscard]] constexpr Type& front() const;
    [[nodiscard]] constexpr Type& back() const;
    
    constexpr void reserve(Size capacity);
    constexpr void resize(Size len, Type &value);
    constexpr void resize(Size len);
    constexpr void shrink_to_fit();

    constexpr void insert(Const_Iterator pos, Type &&value);
    constexpr void insert(Const_Iterator pos, const Type &value);
    constexpr void insert(Const_Iterator pos, Size count, const Type &value);
    template<typename It> constexpr void insert(Const_Iterator pos, It first, It last);
    constexpr void insert(Const_Iterator pos, std::initializer_list<T> &elements);
    template<typename... Args> constexpr Type& emplace(Const_Iterator pos, Args&&... args);
    constexpr void push_back(Type&& value);
    constexpr void push_back(const Type &value);
    constexpr void push_back_vector(vector &&value);
    constexpr void push_back_vector(const vector &value);
    template<typename... Args> constexpr Type& emplace_back(Args&&... args);
    
    constexpr Type pop_back();
    constexpr void erase(Iterator begin, Iterator end);
    constexpr void erase(Iterator pos);
    constexpr void clear();
    
    [[nodiscard]] constexpr bool contains(const Type &value) const;
    [[nodiscard]] constexpr Size count(const Type &value) const;

    [[nodiscard]] constexpr Type& at(Size pos);
    [[nodiscard]] constexpr const Type& at(Size pos) const;
    
    [[nodiscard]] constexpr Size capacity() const;
    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;
    
    [[nodiscard]] constexpr Type& operator[](Size pos);
    [[nodiscard]] constexpr const Type& operator[](Size pos) const;
    
    template<class A2> [[nodiscard]] constexpr bool operator==(const vector<T, A2> &vec) const;
    template<class A2> [[nodiscard]] constexpr bool operator!=(const vector<T, A2> &vec) const;

    constexpr vector& operator=(const vector<T, A> &vec);
    constexpr vector& operator=(vector<T, A> &&vec);
    constexpr vector& operator=(std::initializer_list<T> elements);

  };

}

template<typename T, class A>
constexpr me::vector<T, A>::vector(Iterator begin, Iterator end, Iterator capacity)
  : begin_(begin), end_(end), capacity_(capacity)
{
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(Iterator begin, Size length, Size capacity)
{
  Iterator iter = begin;

  begin_ = Alloc::template malloc<Type>(capacity);
  end_ = begin_ + length;
  capacity_ = capacity;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(*iter++);
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(Iterator begin, Size length)
{
  Iterator iter = begin;

  begin_ = Alloc::template malloc<Type>(length);
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(*iter++);
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(Type &value, Size length)
{
  begin_ = Alloc::template malloc<Type>(length);
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(value);
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(std::initializer_list<Type> elements)
{
  Const_Iterator iter = elements.begin();

  begin_ = Alloc::template malloc<Type>(elements.size());
  end_ = begin_ + elements.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(*iter++);
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(Size length)
{
  begin_ = Alloc::template malloc<Type>(length);
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = Type();
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(const vector<T, A> &copy)
{
  begin_ = Alloc::template malloc<Type>(copy.size());
  end_ = begin_ + copy.size();
  capacity_ = end_;
  memory::safe_copy<Type>(begin_, copy.begin_, copy.end_);
}

template<typename T, class A>
constexpr me::vector<T, A>::vector(vector<T, A> &&move)
{
  begin_ = static_cast<vector<T, A>&&>(move).begin_;
  end_ = static_cast<vector<T, A>&&>(move).end_;
  capacity_ = static_cast<vector<T, A>&&>(move).capacity_;
}

template<typename T, class A>
constexpr me::vector<T, A>::vector()
  : begin_(nullptr), end_(nullptr), capacity_(nullptr)
{
}

template<typename T, class A>
constexpr me::vector<T, A>::~vector()
{
  if (capacity() != 0)
  {
    Iterator iter = begin_;
    while (iter != end_)
      Alloc::template destruct<Type>(iter++);
    Alloc::free(begin_);
  }
}

template<typename T, class A>
constexpr T* me::vector<T, A>::data() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::vector<T, A>::begin() const
{
  return begin_;
}

template<typename T, class A>
constexpr T* me::vector<T, A>::end() const
{
  return end_;
}

template<typename T, class A>
constexpr const T* me::vector<T, A>::cbegin() const
{
  return begin_;
}

template<typename T, class A>
constexpr const T* me::vector<T, A>::cend() const
{
  return end_;
}

template<typename T, class A>
constexpr T& me::vector<T, A>::front() const
{
  return begin_[0];
}

template<typename T, class A>
constexpr T& me::vector<T, A>::back() const
{
  return *(end_ - 1);
}

template<typename T, class A>
constexpr void me::vector<T, A>::reserve(Size capacity)
{
  if (capacity > this->capacity())
  {
    Size count = this->size();

    Iterator begin = (Iterator) Alloc::template malloc<Type>(capacity);
    memory::safe_copy<Type>(begin, begin_, end_);
    Alloc::free(begin_);
    begin_ = begin;
    end_ = begin + count;
    capacity_ = begin + capacity;
  }
}

template<typename T, class A>
constexpr void me::vector<T, A>::resize(Size len, Type &value)
{
  this->reserve(len);

  end_ = begin_ + len;

  /* is this correct? */
  for (Iterator i = begin_; i != end_; i++)
    *i = value;
}

template<typename T, class A>
constexpr void me::vector<T, A>::resize(Size len)
{
  this->reserve(len);

  end_ = begin_ + len;

  for (Iterator i = begin_; i != end_; i++)
    *i = Type();
}

template<typename T, class A>
constexpr void me::vector<T, A>::shrink_to_fit()
{
  Size size = this->size();

  if (this->capacity() > size)
  {
    Iterator begin = (Iterator) Alloc::template malloc<Type>(size * sizeof(Type));
    memory::safe_copy<Type>(begin, begin_, end_);
    Alloc::free(begin_);
    begin_ = begin;
    end_ = begin + size;
    capacity_ = begin + size;
  }
}

template<typename T, class A>
constexpr void me::vector<T, A>::insert(Const_Iterator pos, Type &&value)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  ::new ((void*) pos) Type(value);
  end_++;
}

template<typename T, class A>
constexpr void me::vector<T, A>::insert(Const_Iterator pos, const Type &value)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  ::new ((void*) pos) Type(value);
  end_++;
}

template<typename T, class A>
constexpr void me::vector<T, A>::insert(Const_Iterator pos, Size count, const Type &value)
{
  Size len = this->size();

  this->reserve(len + count);
  memory::safe_move<Type>(pos + count, pos, len);
  for (Size i = 0; i != count; i++)
    ::new ((void*) (pos + i)) Type(value);
  end_ += count;
}

template<typename T, class A>
template<typename It>
constexpr void me::vector<T, A>::insert(Const_Iterator pos, It begin, It end)
{
  Size len = this->size();
  Size count = end - begin;

  this->reserve(len + count);
  memory::safe_move<Type>(pos + count, pos, len);
  while (begin != end)
    ::new ((void*) (pos++)) Type(*begin++);
  end_ += count;
}

template<typename T, class A>
constexpr void me::vector<T, A>::insert(Const_Iterator pos, std::initializer_list<T> &elements)
{
  Size len = this->size();

  this->reserve(len + elements.size());
  memory::safe_move<Type>(pos + elements.size(), pos, len);
  Const_Iterator iter = elements.begin();
  while (iter != elements.end())
    ::new ((void*) (pos++)) Type(*iter++);
  end_ += elements.size();
}

template<typename T, class A>
template<typename... Args>
constexpr T& me::vector<T, A>::emplace(Const_Iterator pos, Args&&... args)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  T* ptr = Alloc::construct(pos, static_cast<Args&&>(args)...);
  end_++;
  return *ptr;
}

template<typename T, class A>
constexpr void me::vector<T, A>::push_back(Type &&value)
{
  this->reserve(this->size() + 1);
  ::new ((void*) end_) Type(value);
  end_++;
}

template<typename T, class A>
constexpr void me::vector<T, A>::push_back(const Type &value)
{
  reserve(this->size() + 1);
  ::new ((void*) end_) Type(value);
  end_++;
}

template<typename T, class A>
constexpr void me::vector<T, A>::push_back_vector(vector &&value)
{
  reserve(this->size() + static_cast<vector&&>(value).size());
  for (Type &i : static_cast<vector&&>(value))
    ::new ((void*) end_++) Type(i);
}

template<typename T, class A>
constexpr void me::vector<T, A>::push_back_vector(const vector &value)
{
  reserve(this->size() + value.size());
  for (const Type &i : value)
    ::new ((void*) end_++) Type(i);
}

template<typename T, class A>
template<typename... Args>
constexpr T& me::vector<T, A>::emplace_back(Args&&... args)
{
  reserve(this->size() + 1);
  T* ptr = Alloc::construct(end_, static_cast<Args&&>(args)...);
  end_++;
  return *ptr;
}

template<typename T, class A>
constexpr T me::vector<T, A>::pop_back()
{
  if (this->is_empty())
    throw exception("me::vector::pop_back(): no elements to pop back");
  end_--;
  return *end_;
}

template<typename T, class A>
constexpr void me::vector<T, A>::erase(Iterator begin, Iterator end)
{
  Size len = (end - begin);
  Size tail_len = (end_ - end);

  for (Iterator i = begin; i != end; i++)
    Alloc::destruct(i);
  while (tail_len--)
    *begin++ = *end++;
  end_ -= len;
}

template<typename T, class A>
constexpr void me::vector<T, A>::erase(Iterator pos)
{
  erase(pos, pos + 1);
}

template<typename T, class A>
constexpr void me::vector<T, A>::clear()
{
  for (Iterator i = begin_; i != end_; i++)
    Alloc::destruct(i);
  end_ = begin_;
}

template<typename T, class A>
constexpr bool me::vector<T, A>::contains(const Type &value) const
{
  for (Iterator i = begin_; i != end_; i++)
  {
    if (*i == value)
      return true;
  }
  return false;
}

template<typename T, class A>
constexpr me::size_t me::vector<T, A>::count(const Type &value) const
{
  Size count = 0;
  for (Iterator i = begin_; i != end_; i++)
  {
    if (*i == value)
      count++;
  }
  return count;
}

template<typename T, class A>
constexpr T& me::vector<T, A>::at(Size pos)
{
  if (pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T, class A>
constexpr const T& me::vector<T, A>::at(Size pos) const
{
  if (pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T, class A>
constexpr me::size_t me::vector<T, A>::capacity() const
{
  return capacity_ - begin_;
}

template<typename T, class A>
constexpr me::size_t me::vector<T, A>::size() const
{
  return end_ - begin_;
}

template<typename T, class A>
constexpr bool me::vector<T, A>::is_empty() const
{
  return this->size() == 0;
}

template<typename T, class A>
constexpr T& me::vector<T, A>::operator[](Size pos)
{
  if (pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T, class A>
constexpr const T& me::vector<T, A>::operator[](Size pos) const
{
  if (pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T, class A>
template<class A2>
constexpr bool me::vector<T, A>::operator==(const vector<T, A2> &vec) const
{
  if (vec.size() != this->size())
    return false;
  for (Size i = 0; i < this->size(); i++)
  {
    if (vec.at(i) != this->at(i))
      return false;
  }
  return true;
}

template<typename T, class A>
template<class A2>
constexpr bool me::vector<T, A>::operator!=(const vector<T, A2> &vec) const
{
  if (vec.size() == 0 && this->size() == 0)
    return false;
  if (vec.size() == 0 || this->size() == 0)
    return true;
  return !operator==(vec);
}

template<typename T, class A>
constexpr me::vector<T, A>& me::vector<T, A>::operator=(const vector<T, A> &vec)
{
  Size len = vec.size();

  begin_ = (Iterator) Alloc::template malloc<Type>(len * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;
  memory::safe_copy<Type>(begin_, vec.begin_, vec.end_);
  return *this;
}

template<typename T, class A>
constexpr me::vector<T, A>& me::vector<T, A>::operator=(vector<T, A> &&vec)
{
  begin_ = static_cast<vector<T>&&>(vec).begin_;
  end_ = static_cast<vector<T>&&>(vec).end_;
  capacity_ = static_cast<vector<T>&&>(vec).capacity_;
  return *this;
}

template<typename T, class A>
constexpr me::vector<T, A>& me::vector<T, A>::operator=(std::initializer_list<T> elements)
{
  Const_Iterator _iter = elements.begin();

  begin_ = (Iterator) Alloc::template malloc<Type>(elements.size() * sizeof(Type));
  end_ = begin_ + elements.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = *_iter++;
  return *this;
}

#endif
