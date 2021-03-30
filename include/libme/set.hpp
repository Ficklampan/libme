#ifndef LIBME_SET_HPP
  #define LIBME_SET_HPP

#include "type.hpp"
#include "allocator.hpp"

#include <initializer_list>

namespace me {

  template<typename T> class vector;

  template<typename T>
  class set /* WIP */ {

  protected:

    typedef allocator _Alloc_Type;
    typedef size_t _Length;
    typedef size_t _Index;
    typedef T* _Position;
    typedef T _Type;
    typedef const T _Const_Type;
    typedef T* _Pointer;
    typedef vector<T> _Entries;

    _Entries _entries;

  public:

    set(_Length _capacity, _Length _length, _Pointer _first, _Alloc_Type* _alloc = allocator::_default());
    //set(vector<T, S> &_elements, _Alloc_Type* _alloc = allocator::_default());
    set(std::initializer_list<T> _elements, _Alloc_Type* _alloc = allocator::_default());
    set(const set &_copy);
    set(_Alloc_Type* _alloc = allocator::_default());

    _Pointer data() const;
    _Pointer begin() const;
    _Pointer end() const;

    void push_back(_Type&&);
    void push_back(_Const_Type&);

    _Type& at(_Index _index);
    const _Type& at(_Index _index) const;

    _Length capacity() const;
    _Length size() const;
    bool is_empty() const;

    _Type& operator[](_Index _index);
    const _Type& operator[](_Index _index) const;

  };

}

#include "vector.hpp"

template<typename T>
me::set<T>::set(_Length _capacity, _Length _length, _Pointer _first, _Alloc_Type* _alloc)
  : _entries(_capacity, _length, nullptr, _alloc)
{
  _Type* _last = _first + _length;

  for (_Position i = _first; i < _last; i++)
  {
    if (this->_entries.find(*i) == vector<T>::npos)
      this->_entries.push_back(*i);
  }
}

/*
template<typename T>
me::set<T>::set(vector<T> &_elements, _Alloc_Type* _alloc)
  : vector<T>(_alloc)
{
  for (_Type &element : _elements)
  {
    if (this->find(element) == SIZE_MAX)
      this->push_back(element);
  }
}
*/

template<typename T>
me::set<T>::set(const set<T> &_copy)
  : set(_copy._elements, _copy._alloc)
{
}

template<typename T>
me::set<T>::set(std::initializer_list<T> _elements, _Alloc_Type* _alloc)
  : _entries(_alloc)
{
  for (_Index i = 0; i < _elements.size(); i++)
  {
    if (this->_entries.find(_elements.begin()[i]) == vector<T>::npos)
      this->_entries.push_back(_elements.begin()[i]);
  }
}

template<typename T>
me::set<T>::set(_Alloc_Type* _alloc)
  : set(0, nullptr, _alloc)
{
}

template<typename T>
T* me::set<T>::data() const
{
  return _entries.data();
}

template<typename T>
T* me::set<T>::begin() const
{
  return _entries.begin();
}

template<typename T>
T* me::set<T>::end() const
{
  return _entries.end();
}

template<typename T>
void me::set<T>::push_back(_Type &&_value)
{
  _Position _pos = vector<T>::find(_value);
  if (_pos == vector<T>::npos)
    this->_entries.push_back(_value);
}

template<typename T>
void me::set<T>::push_back(_Const_Type &_value)
{
  _Position _pos = vector<T>::find(_value);
  if (_pos == vector<T>::npos)
    this->_entries.push_back(_value);
}

template<typename T>
T& me::set<T>::at(_Index _index)
{
  return _entries._first[_index];
}

template<typename T>
const T& me::set<T>::at(_Index _index) const
{
  return _entries._first[_index];
}

template<typename T>
me::size_t me::set<T>::capacity() const
{
  return _entries.capacity();
}

template<typename T>
me::size_t me::set<T>::size() const
{
  return _entries.size();
}

template<typename T>
bool me::set<T>::is_empty() const
{
  return _entries.is_empty();
}

template<typename T>
T& me::set<T>::operator[](_Index _index)
{
  return _entries._first[_index];
}

template<typename T>
const T& me::set<T>::operator[](_Index _index) const
{
  return _entries._first[_index];
}
#endif
