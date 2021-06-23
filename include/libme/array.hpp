#ifndef LIBME_ARRAY_HPP
  #define LIBME_ARRAY_HPP

#include "type.hpp"

namespace me {

  template<typename T, size_t Count>
  class Array {

  protected:

    typedef size_t Size;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Type elements_[Count];

  public:

    [[nodiscard]] Iterator data() const;
    [[nodiscard]] Iterator begin() const;
    [[nodiscard]] Iterator end() const;
    [[nodiscard]] Const_Iterator cbegin() const;
    [[nodiscard]] Const_Iterator cend() const;

    void swap(Size src, Size dst);

    void copy(Size off, Size len, Iterator dst) const;

    [[nodiscard]] Type& at(Size pos);
    [[nodiscard]] const Type& at(Size pos) const;

    [[nodiscard]] Size size() const;

    [[nodiscard]] Type& operator[](Size pos);
    [[nodiscard]] const Type& operator[](Size pos) const;

    [[nodiscard]] bool operator==(const Array<T, Count> &arr) const;
    [[nodiscard]] bool operator!=(const Array<T, Count> &arr) const;

  };
}

template<typename T, me::size_t Count>
T* me::Array<T, Count>::data() const
{
  return elements_;
}

template<typename T, me::size_t Count>
T* me::Array<T, Count>::begin() const
{
  return elements_;
}

template<typename T, me::size_t Count>
T* me::Array<T, Count>::end() const
{
  return elements_ + Count;
}

template<typename T, me::size_t Count>
const T* me::Array<T, Count>::cbegin() const
{
  return elements_;
}

template<typename T, me::size_t Count>
const T* me::Array<T, Count>::cend() const
{
  return elements_ + Count;
}

template<typename T, me::size_t Count>
void me::Array<T, Count>::swap(Size src, Size dst)
{
  Type &temp = elements_[src];
  elements_[src] = elements_[dst];
  elements_[dst] = temp;
}

template<typename T, me::size_t Count>
void me::Array<T, Count>::copy(Size off, Size len, Iterator dst) const
{
  for (Size i = 0; i != len; i++)
    dst[i] = elements_[i + off];
}

template<typename T, me::size_t Count>
T& me::Array<T, Count>::at(Size pos)
{
  return elements_[pos];
}

template<typename T, me::size_t Count>
const T& me::Array<T, Count>::at(Size pos) const
{
  return elements_[pos];
}

template<typename T, me::size_t Count>
me::size_t me::Array<T, Count>::size() const
{
  return Count;
}

template<typename T, me::size_t Count>
T& me::Array<T, Count>::operator[](Size pos)
{
  return elements_[pos];
}

template<typename T, me::size_t Count>
const T& me::Array<T, Count>::operator[](Size pos) const
{
  return elements_[pos];
}

template<typename T, me::size_t Count>
bool me::Array<T, Count>::operator==(const Array<T, Count> &arr) const
{
  for (Size i = 0; i != Count; i++)
  {
    if (arr.elements_[i] != elements_[i])
      return false;
  }
  return true;
}

template<typename T, me::size_t Count>
bool me::Array<T, Count>::operator!=(const Array<T, Count> &arr) const
{
  if (Count == 0)
    return false;

  return elements_[0] != arr.elements_[0];
}

#endif
