#ifndef LIBME_SPAN_HPP
  #define LIBME_SPAN_HPP

#include "type.hpp"

namespace me {

  template<typename T, size_t Extent>
  class span {

  protected:

    typedef size_t Size;
    typedef T* Pointer;
    typedef const T* Const_Pointer;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    static constexpr Size extent_ = Extent;

    Pointer data_;

  public:

    constexpr span() noexcept = default;
    template<typename It> constexpr explicit(extent_ != dynamic_extent) span(It data, Size count);
    template<typename It, typename End> constexpr explicit(extent_ != dynamic_extent) span(It first, End last);

    [[nodiscard]] constexpr Iterator begin() const;
    [[nodiscard]] constexpr Iterator end() const;

    [[nodiscard]] constexpr Type& front() const;
    [[nodiscard]] constexpr Type& back() const;
    [[nodiscard]] constexpr Type& operator[](Size pos) const;
    [[nodiscard]] constexpr Pointer data() const;

    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;

  };

  template<typename T>
  class span<T, dynamic_extent> {

  protected:

    typedef size_t Size;
    typedef T* Pointer;
    typedef const T* Const_Pointer;
    typedef T* Iterator;
    typedef const T* Const_Iterator;
    typedef T Type;

    Pointer data_;
    Size count_;

  public:

    template<size_t N> constexpr span(Type (&arr)[N]) noexcept;

    [[nodiscard]] constexpr Iterator begin() const;
    [[nodiscard]] constexpr Iterator end() const;

    [[nodiscard]] constexpr Type& front() const;
    [[nodiscard]] constexpr Type& back() const;
    [[nodiscard]] constexpr Type& operator[](Size pos) const;
    [[nodiscard]] constexpr Pointer data() const;

    [[nodiscard]] constexpr Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;

  };

}

template<typename T, me::size_t Extent>
template<typename It>
constexpr me::span<T, Extent>::span(It data, Size count)
  : data_(to_address(data))
{
  (void)count;
  LIBME_ASSERT(count == extent_, "errorrrss");
}

template<typename T, me::size_t Extent>
template<typename It, typename End>
constexpr me::span<T, Extent>::span(It first, End last)
  : data_(to_address(first))
{
  (void)last;
  LIBME_ASSERT(count == distance(first, to_address(last)), "more errors");
}

template<typename T, me::size_t Extent>
constexpr T* me::span<T, Extent>::begin() const
{
  return data_;
}

template<typename T, me::size_t Extent>
constexpr T* me::span<T, Extent>::end() const
{
  return data_ + this->size();
}

template<typename T, me::size_t Extent>
constexpr T& me::span<T, Extent>::front() const
{
  return data_[0];
}

template<typename T, me::size_t Extent>
constexpr T& me::span<T, Extent>::back() const
{
  return data_[extent_ - 1];
}

template<typename T, me::size_t Extent>
constexpr T& me::span<T, Extent>::operator[](Size pos) const
{
  return data_[pos];
}

template<typename T, me::size_t Extent>
constexpr T* me::span<T, Extent>::data() const
{
  return data_;
}

template<typename T, me::size_t Extent>
constexpr me::size_t me::span<T, Extent>::size() const
{
  return extent_;
}

template<typename T, me::size_t Extent>
constexpr bool me::span<T, Extent>::is_empty() const
{
  return extent_ == 0;
}


/* Dynamic */

template<typename T>
template<me::size_t N>
constexpr me::span<T, me::dynamic_extent>::span(Type (&arr)[N]) noexcept
  : data_(arr), count_(N)
{
}

template<typename T>
constexpr T* me::span<T, me::dynamic_extent>::begin() const
{
  return data_;
}

template<typename T>
constexpr T* me::span<T, me::dynamic_extent>::end() const
{
  return data_ + count_;
}

template<typename T>
constexpr T& me::span<T, me::dynamic_extent>::front() const
{
  return data_[0];
}

template<typename T>
constexpr T& me::span<T, me::dynamic_extent>::back() const
{
  return data_[count_ - 1];
}

template<typename T>
constexpr T& me::span<T, me::dynamic_extent>::operator[](Size pos) const
{
  return data_[pos];
}

template<typename T>
constexpr T* me::span<T, me::dynamic_extent>::data() const
{
  return data_;
}

template<typename T>
constexpr me::size_t me::span<T, me::dynamic_extent>::size() const
{
  return count_;
}

template<typename T>
constexpr bool me::span<T, me::dynamic_extent>::is_empty() const
{
  return count_ == 0;
}

#endif
