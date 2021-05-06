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

  template<typename T>
  class vector {

  protected:

    typedef allocator Alloc_Type;
    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Iterator begin_;
    Iterator end_;
    Iterator capacity_;
    Alloc_Type* alloc_;
    
  public:

    [[deprecated]] constexpr vector(Iterator begin, Iterator end, Iterator capacity, Alloc_Type* alloc = allocator::_default());
    constexpr vector(Iterator begin, Size lengh, Size _capacity, Alloc_Type* alloc = allocator::_default());
    constexpr vector(Iterator begin, Size lengh, Alloc_Type* alloc = allocator::_default());
    constexpr vector(Type &value, Size _length, Alloc_Type* alloc = allocator::_default());
    constexpr vector(std::initializer_list<T> elements, Alloc_Type* alloc = allocator::_default());
    constexpr vector(Size length, Alloc_Type* alloc = allocator::_default());
    constexpr vector(const vector &copy);
    constexpr vector(vector &&move);
    constexpr vector(Alloc_Type* alloc = allocator::_default());

    constexpr ~vector();

    [[nodiscard]] constexpr Alloc_Type* get_allocator() const;

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
    template<typename... A> constexpr Type& emplace(Const_Iterator pos, A&&... args);
    constexpr void push_back(Type&& value);
    constexpr void push_back(const Type &value);
    constexpr void push_back_vector(vector &&value);
    constexpr void push_back_vector(const vector &value);
    template<typename... A> constexpr Type& emplace_back(A&&... args);
    
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
    
    [[nodiscard]] constexpr bool operator==(const vector &vec) const;
    [[nodiscard]] constexpr bool operator!=(const vector &vec) const;

    constexpr vector& operator=(const vector &vec);
    constexpr vector& operator=(vector &&vec);
    constexpr vector& operator=(std::initializer_list<T> elements);

  };

}

template<typename T>
constexpr me::vector<T>::vector(Iterator begin, Iterator end, Iterator capacity, Alloc_Type* alloc)
  : begin_(begin), end_(end), capacity_(capacity), alloc_(alloc)
{
}

template<typename T>
constexpr me::vector<T>::vector(Iterator begin, Size length, Size capacity, Alloc_Type* alloc)
  : alloc_(alloc)
{
  Iterator iter = begin;

  begin_ = (Iterator) alloc->malloc(capacity);
  end_ = begin_ + length;
  capacity_ = capacity;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(*iter++);
}

template<typename T>
constexpr me::vector<T>::vector(Iterator begin, Size length, Alloc_Type* alloc)
  : alloc_(alloc)
{
  Iterator iter = begin;

  begin_ = (Iterator) alloc_->malloc(length * sizeof(Type));
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    ::new ((void*) i) Type(*iter++);
}

template<typename T>
constexpr me::vector<T>::vector(Type &value, Size length, Alloc_Type* alloc)
  : alloc_(alloc)
{
  begin_ = (Iterator) alloc_->malloc(length * sizeof(Type));
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = value;
}

template<typename T>
constexpr me::vector<T>::vector(std::initializer_list<Type> elements, Alloc_Type* alloc)
  : alloc_(alloc)
{
  Const_Iterator iter = elements.begin();

  begin_ = (Iterator) alloc_->malloc(elements.size() * sizeof(Type));
  end_ = begin_ + elements.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = *iter++;
}

template<typename T>
constexpr me::vector<T>::vector(Size length, Alloc_Type* alloc)
  : alloc_(alloc)
{
  begin_ = (Iterator) alloc_->malloc(length * sizeof(Type));
  end_ = begin_ + length;
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = Type();
}

template<typename T>
constexpr me::vector<T>::vector(const vector<T> &copy)
  : alloc_(copy.alloc_)
{
  begin_ = (Iterator) alloc_->malloc(copy.size() * sizeof(Type));
  end_ = begin_ + copy.size();
  capacity_ = end_;
  memory::safe_copy<Type>(begin_, copy.begin_, copy.end_);
}

template<typename T>
constexpr me::vector<T>::vector(vector<T> &&move)
{
  begin_ = static_cast<vector<T>&&>(move).begin_;
  end_ = static_cast<vector<T>&&>(move).end_;
  capacity_ = static_cast<vector<T>&&>(move).capacity_;
  alloc_ = static_cast<vector<T>&&>(move).alloc_;
}

template<typename T>
constexpr me::vector<T>::vector(Alloc_Type* alloc)
  : begin_(nullptr), end_(nullptr), capacity_(nullptr), alloc_(alloc)
{
}

template<typename T>
constexpr me::vector<T>::~vector()
{
  if (capacity() != 0)
  {
    alloc_->mdealloc<Type>(begin_, end_);
    alloc_->free(begin_);
  }
}

template<typename T>
constexpr me::allocator* me::vector<T>::get_allocator() const
{
  return alloc_;
}

template<typename T>
constexpr T* me::vector<T>::data() const
{
  return begin_;
}

template<typename T>
constexpr T* me::vector<T>::begin() const
{
  return begin_;
}

template<typename T>
constexpr T* me::vector<T>::end() const
{
  return end_;
}

template<typename T>
constexpr const T* me::vector<T>::cbegin() const
{
  return begin_;
}

template<typename T>
constexpr const T* me::vector<T>::cend() const
{
  return end_;
}

template<typename T>
constexpr T& me::vector<T>::front() const
{
  return begin_[0];
}

template<typename T>
constexpr T& me::vector<T>::back() const
{
  return *(end_ - 1);
}

template<typename T>
constexpr void me::vector<T>::reserve(Size capacity)
{
  if (capacity > this->capacity())
  {
    Size count = this->size();

    Iterator begin = (Iterator) alloc_->malloc(capacity * sizeof(Type));
    memory::safe_copy<Type>(begin, begin_, end_);
    alloc_->free(begin_);
    begin_ = begin;
    end_ = begin + count;
    capacity_ = begin + capacity;
  }
}

template<typename T>
constexpr void me::vector<T>::resize(Size len, Type &value)
{
  this->reserve(len);

  end_ = begin_ + len;

  /* is this correct? */
  for (Iterator i = begin_; i != end_; i++)
    *i = value;
}

template<typename T>
constexpr void me::vector<T>::resize(Size len)
{
  this->reserve(len);

  end_ = begin_ + len;

  for (Iterator i = begin_; i != end_; i++)
    *i = Type();
}

template<typename T>
constexpr void me::vector<T>::shrink_to_fit()
{
  Size size = this->size();

  if (this->capacity() > size)
  {
    Iterator begin = (Iterator) alloc_->malloc(size * sizeof(Type));
    memory::safe_copy<Type>(begin, begin_, end_);
    alloc_->free(begin_);
    begin_ = begin;
    end_ = begin + size;
    capacity_ = begin + size;
  }
}

template<typename T>
constexpr void me::vector<T>::insert(Const_Iterator pos, Type &&value)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  ::new ((void*) pos) Type(value);
  end_++;
}

template<typename T>
constexpr void me::vector<T>::insert(Const_Iterator pos, const Type &value)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  ::new ((void*) pos) Type(value);
  end_++;
}

template<typename T>
constexpr void me::vector<T>::insert(Const_Iterator pos, Size count, const Type &value)
{
  Size len = this->size();

  this->reserve(len + count);
  memory::safe_move<Type>(pos + count, pos, len);
  for (Size i = 0; i != count; i++)
    ::new ((void*) (pos + i)) Type(value);
  end_ += count;
}

template<typename T>
template<typename It>
constexpr void me::vector<T>::insert(Const_Iterator pos, It begin, It end)
{
  Size len = this->size();
  Size count = end - begin;

  this->reserve(len + count);
  memory::safe_move<Type>(pos + count, pos, len);
  while (begin != end)
    ::new ((void*) (pos++)) Type(*begin++);
  end_ += count;
}

template<typename T>
constexpr void me::vector<T>::insert(Const_Iterator pos, std::initializer_list<T> &elements)
{
  Size len = this->size();

  this->reserve(len + elements.size());
  memory::safe_move<Type>(pos + elements.size(), pos, len);
  Const_Iterator iter = elements.begin();
  while (iter != elements.end())
    ::new ((void*) (pos++)) Type(*iter++);
  end_ += elements.size();
}

template<typename T>
template<typename... A>
constexpr T& me::vector<T>::emplace(Const_Iterator pos, A&&... args)
{
  Size len = this->size();

  this->reserve(len + 1);
  memory::safe_move<Type>(pos + 1, pos, len);
  T* ptr = alloc_->construct(pos, static_cast<A&&>(args)...);
  end_++;
  return *ptr;
}

template<typename T>
constexpr void me::vector<T>::push_back(Type &&value)
{
  this->reserve(this->size() + 1);
  ::new ((void*) end_) Type(value);
  end_++;
}

template<typename T>
constexpr void me::vector<T>::push_back(const Type &value)
{
  reserve(this->size() + 1);
  ::new ((void*) end_) Type(value);
  end_++;
}

template<typename T>
constexpr void me::vector<T>::push_back_vector(vector &&value)
{
  reserve(this->size() + static_cast<vector&&>(value).size());
  for (Type &i : static_cast<vector&&>(value))
    ::new ((void*) end_++) Type(i);
}

template<typename T>
constexpr void me::vector<T>::push_back_vector(const vector &value)
{
  reserve(this->size() + value.size());
  for (const Type &i : value)
    ::new ((void*) end_++) Type(i);
}

template<typename T>
template<typename... A>
constexpr T& me::vector<T>::emplace_back(A&&... args)
{
  reserve(this->size() + 1);
  T* ptr = alloc_->construct(end_, static_cast<A&&>(args)...);
  end_++;
  return *ptr;
}

template<typename T>
constexpr T me::vector<T>::pop_back()
{
  if (this->is_empty())
    throw exception("me::vector::pop_back(): no elements to pop back");
  end_--;
  return end_;
}

template<typename T>
constexpr void me::vector<T>::erase(Iterator begin, Iterator end)
{
  Size len = (end - begin);
  Size tail_len = (end_ - end);

  for (Iterator i = begin; i != end; i++)
    alloc_->destruct(i);
  while (tail_len--)
    *begin++ = *end++;
  end_ -= len;
}

template<typename T>
constexpr void me::vector<T>::erase(Iterator pos)
{
  erase(pos, pos + 1);
}

template<typename T>
constexpr void me::vector<T>::clear()
{
  for (Iterator i = begin_; i != end_; i++)
    alloc_->destruct(i);
  end_ = begin_;
}

template<typename T>
constexpr bool me::vector<T>::contains(const Type &value) const
{
  for (Iterator i = begin_; i != end_; i++)
  {
    if (*i == value)
      return true;
  }
  return false;
}

template<typename T>
constexpr me::size_t me::vector<T>::count(const Type &value) const
{
  Size count = 0;
  for (Iterator i = begin_; i != end_; i++)
  {
    if (*i == value)
      count++;
  }
  return count;
}

template<typename T>
constexpr T& me::vector<T>::at(Size pos)
{
  if (pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T>
constexpr const T& me::vector<T>::at(Size pos) const
{
  if (pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T>
constexpr me::size_t me::vector<T>::capacity() const
{
  return capacity_ - begin_;
}

template<typename T>
constexpr me::size_t me::vector<T>::size() const
{
  return end_ - begin_;
}

template<typename T>
constexpr bool me::vector<T>::is_empty() const
{
  return this->size() == 0;
}

template<typename T>
constexpr T& me::vector<T>::operator[](Size pos)
{
  if (pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T>
constexpr const T& me::vector<T>::operator[](Size pos) const
{
  if (pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", pos, pos, this->size());
  return begin_[pos];
}

template<typename T>
constexpr bool me::vector<T>::operator==(const vector<T> &vec) const
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

template<typename T>
constexpr bool me::vector<T>::operator!=(const vector<T> &vec) const
{
  if (vec.size() == 0 && this->size() == 0)
    return false;
  if (vec.size() == 0 || this->size() == 0)
    return true;
  return !operator==(vec);
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(const vector<T> &vec)
{
  Size len = vec.size();

  alloc_ = vec.alloc_;
  begin_ = (Iterator) alloc_->malloc(len * sizeof(Type));
  end_ = begin_ + len;
  capacity_ = end_;
  memory::safe_copy<Type>(begin_, vec.begin_, vec.end_);
  return *this;
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(vector<T> &&vec)
{
  begin_ = static_cast<vector<T>&&>(vec).begin_;
  end_ = static_cast<vector<T>&&>(vec).end_;
  capacity_ = static_cast<vector<T>&&>(vec).capacity_;
  alloc_ = static_cast<vector<T>&&>(vec).alloc_;
  return *this;
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(std::initializer_list<T> elements)
{
  Const_Iterator _iter = elements.begin();

  alloc_ = Alloc_Type::_default();
  begin_ = (Iterator) alloc_->malloc(elements.size() * sizeof(Type));
  end_ = begin_ + elements.size();
  capacity_ = end_;
  for (Iterator i = begin_; i != end_; i++)
    *i = *_iter++;
  return *this;
}

#endif
