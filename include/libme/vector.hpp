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

    typedef allocator _Alloc_Type;
    typedef size_t _Size;
    typedef T* _Iterator;
    typedef const T* _Const_Iterator;
    typedef T _Type;

    _Iterator _begin;
    _Iterator _end;
    _Iterator _capacity;
    _Alloc_Type* _alloc;
    
  public:

    [[deprecated]] constexpr vector(_Iterator _begin, _Iterator _end, _Iterator _capacity, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(_Iterator _begin, _Size _lengh, _Size _capacity, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(_Iterator _begin, _Size _lengh, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(_Type &_value, _Size _length, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(std::initializer_list<T> _elements, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(_Size _length, _Alloc_Type* _alloc = allocator::_default());
    constexpr vector(const vector &_copy);
    constexpr vector(vector &&_move);
    constexpr vector(_Alloc_Type* _alloc = allocator::_default());

    constexpr ~vector();

    [[nodiscard]] constexpr _Alloc_Type* get_allocator() const;

    [[nodiscard]] constexpr _Iterator data() const;

    [[nodiscard]] constexpr _Iterator begin() const;
    [[nodiscard]] constexpr _Iterator end() const;

    [[nodiscard]] constexpr _Const_Iterator cbegin() const;
    [[nodiscard]] constexpr _Const_Iterator cend() const;

    [[nodiscard]] constexpr _Type& front() const;
    [[nodiscard]] constexpr _Type& back() const;
    
    constexpr void reserve(_Size _capacity);
    constexpr void resize(_Size _len, _Type &_value);
    constexpr void resize(_Size _len);
    constexpr void shrink_to_fit();

    constexpr void insert(_Const_Iterator _pos, _Type &&_value);
    constexpr void insert(_Const_Iterator _pos, const _Type &_value);
    constexpr void insert(_Const_Iterator _pos, _Size _count, const _Type &_value);
    template<typename It> constexpr void insert(_Const_Iterator _pos, It _first, It _last);
    constexpr void insert(_Const_Iterator _pos, std::initializer_list<T> &_elements);
    template<typename... A> constexpr _Type& emplace(_Const_Iterator _pos, A&&... _args);
    constexpr void push_back(_Type&& _value);
    constexpr void push_back(const _Type &_value);
    template<typename... A> constexpr _Type& emplace_back(A&&... _args);
    
    constexpr _Type pop_back();
    constexpr void erase(_Iterator _begin, _Iterator _end);
    constexpr void erase(_Iterator _pos);
    constexpr void clear();
    
    [[nodiscard]] constexpr bool contains(_Type &&_value) const;
    [[nodiscard]] constexpr _Size count(_Type &&_value) const;

    [[nodiscard]] constexpr _Type& at(_Size _pos);
    [[nodiscard]] constexpr const _Type& at(_Size _pos) const;
    
    [[nodiscard]] constexpr _Size capacity() const;
    [[nodiscard]] constexpr _Size size() const;
    [[nodiscard]] constexpr bool is_empty() const;
    
    [[nodiscard]] constexpr _Type& operator[](_Size _pos);
    [[nodiscard]] constexpr const _Type& operator[](_Size _pos) const;
    
    [[nodiscard]] constexpr bool operator==(const vector &_vec) const;
    [[nodiscard]] constexpr bool operator!=(const vector &_vec) const;

    constexpr vector& operator=(const vector &_vec);
    constexpr vector& operator=(vector &&_vec);
    constexpr vector& operator=(std::initializer_list<T> _elements);

  };

}

template<typename T>
constexpr me::vector<T>::vector(_Iterator _begin, _Iterator _end, _Iterator _capacity, _Alloc_Type* _alloc)
  : _begin(_begin), _end(_end), _capacity(_capacity), _alloc(_alloc)
{
}

template<typename T>
constexpr me::vector<T>::vector(_Iterator _begin, _Size _length, _Size _capacity, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Iterator _iter = _begin;

  this->_begin = (_Iterator) _alloc->malloc(_capacity);
  this->_end = this->_begin + _length;
  this->_capacity = _capacity;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    ::new ((void*) i) _Type(*_iter++);
}

template<typename T>
constexpr me::vector<T>::vector(_Iterator _begin, _Size _length, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Iterator _iter = _begin;

  this->_begin = (_Iterator) _alloc->malloc(_length * sizeof(_Type));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    ::new ((void*) i) _Type(*_iter++);
}

template<typename T>
constexpr me::vector<T>::vector(_Type &_value, _Size _length, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  this->_begin = (_Iterator) _alloc->malloc(_length * sizeof(_Type));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = _value;
}

template<typename T>
constexpr me::vector<T>::vector(std::initializer_list<_Type> _elements, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  _Const_Iterator _iter = _elements.begin();

  this->_begin = (_Iterator) _alloc->malloc(_elements.size() * sizeof(_Type));
  this->_end = this->_begin + _elements.size();
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = *_iter++;
}

template<typename T>
constexpr me::vector<T>::vector(_Size _length, _Alloc_Type* _alloc)
  : _alloc(_alloc)
{
  this->_begin = (_Iterator) _alloc->malloc(_length * sizeof(_Type));
  this->_end = this->_begin + _length;
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = _Type();
}

template<typename T>
constexpr me::vector<T>::vector(const vector<T> &_copy)
  : _alloc(_copy._alloc)
{
  this->_begin = (_Iterator) _alloc->malloc(_copy.size() * sizeof(_Type));
  this->_end = this->_begin + _copy.size();
  this->_capacity = this->_end;
  memory::safe_copy(this->_begin, _copy._begin, _copy._end);
}

template<typename T>
constexpr me::vector<T>::vector(vector<T> &&_move)
{
  this->_begin = static_cast<vector<T>&&>(_move)._begin;
  this->_end = static_cast<vector<T>&&>(_move)._end;
  this->_capacity = static_cast<vector<T>&&>(_move)._capacity;
  this->_alloc = static_cast<vector<T>&&>(_move)._alloc;
}

template<typename T>
constexpr me::vector<T>::vector(_Alloc_Type* _alloc)
  : _begin(nullptr), _end(nullptr), _capacity(nullptr), _alloc(_alloc)
{
}

template<typename T>
constexpr me::vector<T>::~vector()
{
  _alloc->mdealloc(this->_begin, this->_end);
  _alloc->free(this->_begin);
}

template<typename T>
constexpr me::allocator* me::vector<T>::get_allocator() const
{
  return _alloc;
}

template<typename T>
constexpr T* me::vector<T>::data() const
{
  return this->_begin;
}

template<typename T>
constexpr T* me::vector<T>::begin() const
{
  return this->_begin;
}

template<typename T>
constexpr T* me::vector<T>::end() const
{
  return this->_end;
}

template<typename T>
constexpr const T* me::vector<T>::cbegin() const
{
  return this->_begin;
}

template<typename T>
constexpr const T* me::vector<T>::cend() const
{
  return this->_end;
}

template<typename T>
constexpr T& me::vector<T>::front() const
{
  return this->_begin[0];
}

template<typename T>
constexpr T& me::vector<T>::back() const
{
  return *(this->_end - 1);
}

template<typename T>
constexpr void me::vector<T>::reserve(_Size _capacity)
{
  if (_capacity > this->capacity())
  {
    _Size _count = this->size();

    _Iterator _begin = (_Iterator) _alloc->malloc(_capacity * sizeof(_Type));
    memory::safe_copy(_begin, this->_begin, this->_end);
    _alloc->free(this->_begin);
    this->_begin = _begin;
    this->_end = _begin + _count;
    this->_capacity = _begin + _capacity;
  }
}

template<typename T>
constexpr void me::vector<T>::resize(_Size _len, _Type &_value)
{
  this->reserve(_len);

  this->_end = this->_begin + _len;

  /* is this correct? */
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = _value;
}

template<typename T>
constexpr void me::vector<T>::resize(_Size _len)
{
  this->reserve(_len);

  this->_end = this->_begin + _len;

  for (_Iterator i = this->_begin; i < this->_end; i++)
    *i = _Type();
}

template<typename T>
constexpr void me::vector<T>::shrink_to_fit()
{
  _Size _size = this->size();

  if (this->capacity() > _size)
  {
    _Iterator _begin = (_Iterator) _alloc->malloc(_size * sizeof(_Type));
    memory::safe_copy(_begin, this->_begin, this->_end);
    _alloc->free(this->_begin);
    this->_begin = _begin;
    this->_end = _begin + _size;
    this->_capacity = _begin + _size;
  }
}

template<typename T>
constexpr void me::vector<T>::insert(_Const_Iterator _pos, _Type &&_value)
{
  _Size _len = this->size();

  this->reserve(_len + 1);
  memory::safe_move(_pos + 1, _pos, _len);
  ::new ((void*) _pos) _Type(_value);
  this->_end++;
}

template<typename T>
constexpr void me::vector<T>::insert(_Const_Iterator _pos, const _Type &_value)
{
  _Size _len = this->size();

  this->reserve(_len + 1);
  memory::safe_move(_pos + 1, _pos, _len);
  ::new ((void*) _pos) _Type(_value);
  this->_end++;
}

template<typename T>
constexpr void me::vector<T>::insert(_Const_Iterator _pos, _Size _count, const _Type &_value)
{
  _Size _len = this->size();

  this->reserve(_len + _count);
  memory::safe_move(_pos + _count, _pos, _len);
  for (_Size i = 0; i < _count; i++)
    ::new ((void*) (_pos + i)) _Type(_value);
  this->_end += _count;
}

template<typename T>
template<typename It>
constexpr void me::vector<T>::insert(_Const_Iterator _pos, It _begin, It _end)
{
  _Size _len = this->size();
  _Size _count = _end - _begin;

  this->reserve(_len + _count);
  memory::safe_move(_pos + _count, _pos, _len);
  while (_begin != _end)
    ::new ((void*) (_pos++)) _Type(*_begin++);
  this->_end += _count;
}

template<typename T>
constexpr void me::vector<T>::insert(_Const_Iterator _pos, std::initializer_list<T> &_elements)
{
  _Size _len = this->size();

  this->reserve(_len + _elements.size());
  memory::safe_move(_pos + _elements.size(), _pos, _len);
  _Const_Iterator _iter = _elements.begin();
  while (_iter != _elements.end())
    ::new ((void*) (_pos++)) _Type(*_iter++);
  this->_end += _elements.size();
}

template<typename T>
template<typename... A>
constexpr T& me::vector<T>::emplace(_Const_Iterator _pos, A&&... _args)
{
  _Size _len = this->size();

  this->reserve(_len + 1);
  memory::safe_move(_pos + 1, _pos, _len);
  T* _ptr = _alloc->construct(_pos, static_cast<A&&>(_args)...);
  this->_end++;
  return *_ptr;
}

template<typename T>
constexpr void me::vector<T>::push_back(_Type &&_value)
{
  this->reserve(this->size() + 1);
  ::new ((void*) this->_end) _Type(_value);
  this->_end++;
}

template<typename T>
constexpr void me::vector<T>::push_back(const _Type &_value)
{
  reserve(this->size() + 1);
  ::new ((void*) this->_end) _Type(_value);
  this->_end++;
}

template<typename T>
template<typename... A>
constexpr T& me::vector<T>::emplace_back(A&&... _args)
{
  reserve(this->size() + 1);
  T* _ptr = _alloc->construct(this->_end, static_cast<A&&>(_args)...);
  this->_end++;
  return *_ptr;
}

template<typename T>
constexpr T me::vector<T>::pop_back()
{
  if (this->is_empty())
    throw exception("me::vector::pop_back(): no elements to pop back");
  this->_end--;
  return this->_end;
}

template<typename T>
constexpr void me::vector<T>::erase(_Iterator _begin, _Iterator _end)
{
  for (_Iterator i = _begin; i < _end; i++)
    _alloc->destruct(i);
  memory::safe_move(_begin, _end + 1, (this->_end - _end));
  this->_end -= (_end - _begin);
}

template<typename T>
constexpr void me::vector<T>::erase(_Iterator _pos)
{
  _alloc->destruct(_pos);
  memory::safe_move(_pos, _pos + 1, 1);
  this->_end -= 1;
}

template<typename T>
constexpr void me::vector<T>::clear()
{
  for (_Iterator i = this->_begin; i < this->_end; i++)
    _alloc->destruct(i);
  this->_end = this->_begin;
}

template<typename T>
constexpr bool me::vector<T>::contains(_Type &&_value) const
{
  return this->find(_value);
}

template<typename T>
constexpr me::size_t me::vector<T>::count(_Type &&_value) const
{
  _Size _count = 0;
  for (_Iterator i = this->_begin; i < this->_end; i++)
  {
    if (*i == static_cast<_Type&&>(_value))
      _count++;
  }
  return _count;
}

template<typename T>
constexpr T& me::vector<T>::at(_Size _pos)
{
  if (_pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", _pos, _pos, this->size());
  return this->_begin[_pos];
}

template<typename T>
constexpr const T& me::vector<T>::at(_Size _pos) const
{
  if (_pos >= this->size())
    throw exception("me::vector::at(%lu): out of range %lu > %lu", _pos, _pos, this->size());
  return this->_begin[_pos];
}

template<typename T>
constexpr me::size_t me::vector<T>::capacity() const
{
  return this->_capacity - this->_begin;
}

template<typename T>
constexpr me::size_t me::vector<T>::size() const
{
  return this->_end - this->_begin;
}

template<typename T>
constexpr bool me::vector<T>::is_empty() const
{
  return this->size() == 0;
}

template<typename T>
constexpr T& me::vector<T>::operator[](_Size _pos)
{
  if (_pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", _pos, _pos, this->size());
  return this->_begin[_pos];
}

template<typename T>
constexpr const T& me::vector<T>::operator[](_Size _pos) const
{
  if (_pos >= this->size())
    throw exception("me::vector::operator[](%lu): out of range %lu > %lu", _pos, _pos, this->size());
  return this->_begin[_pos];
}

template<typename T>
constexpr bool me::vector<T>::operator==(const vector<T> &_vec) const
{
  if (_vec.size() != this->size())
    return false;
  for (_Size i = 0; i < this->size(); i++)
  {
    if (_vec.at(i) != this->at(i))
      return false;
  }
  return true;
}

template<typename T>
constexpr bool me::vector<T>::operator!=(const vector<T> &_vec) const
{
  if (_vec.size() == 0 && this->size() == 0)
    return false;
  if (_vec.size() == 0 || this->size() == 0)
    return true;
  return !operator==(this->vec);
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(const vector<T> &_vec)
{
  _Size _len = _vec.size();

  this->_alloc = _vec._alloc;
  this->_begin = (_Iterator) _alloc->malloc(_len * sizeof(_Type));
  this->_end = this->_begin + _len;
  this->_capacity = this->_end;
  memory::safe_copy(this->_begin, _vec._begin, _vec._end);
  return *this;
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(vector<T> &&_vec)
{
  this->_begin = static_cast<vector<T>&&>(_vec)._begin;
  this->_end = static_cast<vector<T>&&>(_vec)._end;
  this->_capacity = static_cast<vector<T>&&>(_vec)._capacity;
  this->_alloc = static_cast<vector<T>&&>(_vec)._alloc;
  return *this;
}

template<typename T>
constexpr me::vector<T>& me::vector<T>::operator=(std::initializer_list<T> _elements)
{
  _Const_Iterator _iter = _elements.begin();

  this->_alloc = _Alloc_Type::_default();
  this->_begin = (_Iterator) _alloc->malloc(_elements.size() * sizeof(_Type));
  this->_end = this->_begin + _elements.size();
  this->_capacity = this->_end;
  for (_Iterator i = this->_begin; i != this->_end; i++)
    *i = *_iter++;
  return *this;
}

#endif
