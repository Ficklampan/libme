#ifndef LIBME_SHARED_PTR_HPP
  #define LIBME_SHARED_PTR_HPP

#include "allocator.hpp"

namespace me {

  template<typename T>
  class shared_ptr {

  private:

    typedef allocator _Alloc_Type;
    typedef size_t _Size;
    typedef size_t* _Counter;
    typedef T _Type;
    typedef T* _Pointer;

    mutable _Counter _counter;
    _Pointer _pointer;

  public:

    shared_ptr(_Counter _counter, _Pointer _pointer);
    shared_ptr(const shared_ptr &_copy);
    shared_ptr(shared_ptr &&_move);
    shared_ptr();
    ~shared_ptr();

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] _Size ref_count() const;
    [[nodiscard]] _Pointer get_ptr() const;

    void reset(_Pointer _pointer);

    _Pointer operator->() const;

    shared_ptr& operator=(const shared_ptr &_copy);
    shared_ptr& operator=(shared_ptr &&_move);

    template<typename... A>
    [[nodiscard]] static shared_ptr make(A&&... _args, _Alloc_Type* _alloc = _Alloc_Type::_default());

    [[nodiscard]] static shared_ptr make(T &&_value, _Alloc_Type* _alloc = _Alloc_Type::_default());
    [[nodiscard]] static shared_ptr make(T* _value, _Alloc_Type* _alloc = _Alloc_Type::_default());

  };

}

template<typename T>
me::shared_ptr<T>::shared_ptr(_Counter _counter, _Pointer _pointer)
  : _counter(_counter), _pointer(_pointer)
{
}

template<typename T>
me::shared_ptr<T>::shared_ptr(const shared_ptr &_copy)
{
  *_copy._count++;

  this->_counter = _copy._counter;
  this->_pointer = _copy._pointer;
}

template<typename T>
me::shared_ptr<T>::shared_ptr(shared_ptr &&_move)
{
  *_move._counter++;

  this->_counter = _move._count;
  this->_pointer = _move._ptr;
}

template<typename T>
me::shared_ptr<T>::shared_ptr()
{
  this->_counter = nullptr;
}

template<typename T>
me::shared_ptr<T>::~shared_ptr()
{
  *this->_counter--;

  if (*this->_counter == 0)
    delete this->_pointer;
}

template<typename T>
bool me::shared_ptr<T>::is_valid() const
{
  return this->_counter != nullptr;
}

template<typename T>
me::size_t me::shared_ptr<T>::ref_count() const
{
  return *this->_counter;
}

template<typename T>
T* me::shared_ptr<T>::get_ptr() const
{
  return this->_pointer;
}

template<typename T>
void me::shared_ptr<T>::reset(_Pointer _pointer)
{
  this->_pointer = _pointer;
}

template<typename T>
T* me::shared_ptr<T>::operator->() const
{
  return this->_pointer;
}

template<typename T>
me::shared_ptr<T>& me::shared_ptr<T>::operator=(const shared_ptr<T> &_copy)
{
  this->_counter = _copy._counter;
  this->_pointer = _copy._pointer;
  *this->_counter++;
  return *this;
}

template<typename T>
me::shared_ptr<T>& me::shared_ptr<T>::operator=(shared_ptr<T> &&_move)
{
  this->_counter = _move._counter;
  this->_pointer = _move._pointer;
  *this->_counter++;
  return *this;
}

template<typename T>
template<typename... A>
me::shared_ptr<T> me::shared_ptr<T>::make(A&&... _args, _Alloc_Type* _alloc)
{
  return shared_ptr(_alloc->calloc<_Size>(1), _alloc->calloc<_Type>(static_cast<A&&>(_args)...));
}

template<typename T>
me::shared_ptr<T> me::shared_ptr<T>::make(T &&_value, _Alloc_Type* _alloc)
{
  return shared_ptr(_alloc->calloc<_Size>(1), _alloc->calloc<_Type>(static_cast<T&&>(_value)));
}

template<typename T>
me::shared_ptr<T> me::shared_ptr<T>::make(T* _value, _Alloc_Type* _alloc)
{
  return shared_ptr(_alloc->calloc<_Size>(1), _value);
}

#endif
