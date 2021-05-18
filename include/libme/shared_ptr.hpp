#ifndef LIBME_SHARED_PTR_HPP
  #define LIBME_SHARED_PTR_HPP

#include "allocator.hpp"

namespace me {

  template<typename T, class A = allocator>
  class shared_ptr {

  private:

    typedef allocator Alloc;
    typedef size_t Size;
    typedef size_t* Counter;
    typedef T Type;
    typedef T* Pointer;

    mutable Counter counter_;
    Pointer pointer_;

  public:

    shared_ptr(Counter counter, Pointer pointer);
    shared_ptr(const shared_ptr<T, A> &copy);
    shared_ptr(shared_ptr<T, A> &&move);
    shared_ptr();
    ~shared_ptr();

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] Size ref_count() const;
    [[nodiscard]] Pointer get_ptr() const;

    void reset(Pointer pointer);

    Pointer operator->() const;

    shared_ptr<T, A>& operator=(const shared_ptr &copy);
    shared_ptr<T, A>& operator=(shared_ptr &&move);

    template<typename... Args>
    [[nodiscard]] static shared_ptr make(Args&&... args);

    [[nodiscard]] static shared_ptr make(Type &&value);
    [[nodiscard]] static shared_ptr make(Type* value);

  };

}

template<typename T, class A>
me::shared_ptr<T, A>::shared_ptr(Counter counter, Pointer pointer)
  : counter_(counter), pointer_(pointer)
{
}

template<typename T, class A>
me::shared_ptr<T, A>::shared_ptr(const shared_ptr<T, A> &copy)
{
  *copy.counter_++;

  counter_ = copy.counter_;
  pointer_ = copy.pointer_;
}

template<typename T, class A>
me::shared_ptr<T, A>::shared_ptr(shared_ptr<T, A> &&move)
{
  *move.counter_++;

  counter_ = move.counter__;
  pointer_ = move.pointer_;
}

template<typename T, class A>
me::shared_ptr<T, A>::shared_ptr()
{
  counter_ = nullptr;
}

template<typename T, class A>
me::shared_ptr<T, A>::~shared_ptr()
{
  *counter_ -= 1;

  if (*counter_ == 0)
    Alloc::free(pointer_);
}

template<typename T, class A>
bool me::shared_ptr<T, A>::is_valid() const
{
  return counter_ != nullptr;
}

template<typename T, class A>
me::size_t me::shared_ptr<T, A>::ref_count() const
{
  return *counter_;
}

template<typename T, class A>
T* me::shared_ptr<T, A>::get_ptr() const
{
  return pointer_;
}

template<typename T, class A>
void me::shared_ptr<T, A>::reset(Pointer pointer)
{
  pointer_ = pointer;
}

template<typename T, class A>
T* me::shared_ptr<T, A>::operator->() const
{
  return pointer_;
}

template<typename T, class A>
me::shared_ptr<T, A>& me::shared_ptr<T, A>::operator=(const shared_ptr<T, A> &copy)
{
  counter_ = copy.counter_;
  pointer_ = copy.pointer_;
  *counter_ += 1;
  return *this;
}

template<typename T, class A>
me::shared_ptr<T, A>& me::shared_ptr<T, A>::operator=(shared_ptr<T, A> &&move)
{
  counter_ = move.counter_;
  pointer_ = move.pointer_;
  *counter_ += 1;
  return *this;
}

template<typename T, class A>
template<typename... Args>
me::shared_ptr<T, A> me::shared_ptr<T, A>::make(Args&&... args)
{
  return shared_ptr<T, A>(Alloc::calloc<Size>(1), Alloc::calloc<Type>(static_cast<Args&&>(args)...));
}

template<typename T, class A>
me::shared_ptr<T, A> me::shared_ptr<T, A>::make(Type &&value)
{
  return shared_ptr<T, A>(Alloc::calloc<Size>(1), Alloc::calloc<Type>(static_cast<Type&&>(value)));
}

template<typename T, class A>
me::shared_ptr<T, A> me::shared_ptr<T, A>::make(Type* value)
{
  return shared_ptr<T, A>(Alloc::calloc<Size>(1), value);
}

#endif
