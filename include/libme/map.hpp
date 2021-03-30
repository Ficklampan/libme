#ifndef LIBME_MAP_HPP
  #define LIBME_MAP_HPP

#include "type.hpp"
#include "pair.hpp"
#include "vector.hpp"

namespace me {

  template<typename K, typename V>
  class map {

  protected:

    typedef allocator _Alloc_Type;
    typedef size_t _Size;
    typedef const K _Key;
    typedef V _Value;
    typedef const V _Const_Value;
    typedef pair<K, V> _Pair;
    typedef const pair<K, V> _Const_Pair;
    typedef vector<pair<K, V>> _Container;
    typedef const vector<pair<K, V>> _Const_Container;

    _Container _entries;

  public:

    map(std::initializer_list<_Pair> _elements, _Alloc_Type* _alloc = allocator::_default());
    map(const map &_copy);
    map(map &&_move);
    map(_Alloc_Type* _alloc = allocator::_default());

    [[nodiscard]] _Pair* data() const;
    [[nodiscard]] _Pair* begin() const;
    [[nodiscard]] _Pair* end() const;

    _Pair& put(_Key &_key, _Value &&_value);
    _Pair& put(_Key &_key, _Const_Value &_value);

    void erase(_Key &_key);
    void clear();

    [[nodiscard]] _Const_Pair* find(_Key &_key) const;
    [[nodiscard]] bool contains(_Key &_key) const;

    [[nodiscard]] _Value& at(_Key &_key);
    [[nodiscard]] _Const_Value& at(_Key &_key) const;
    [[nodiscard]] _Value& at(_Key &_key, _Value &_default_value);
    [[nodiscard]] _Const_Value& at(_Key &_key, _Const_Value &_default_value) const;

    [[nodiscard]] _Size capacity() const;
    [[nodiscard]] _Size size() const;
    [[nodiscard]] bool is_empty() const;

    _Value& operator[](_Key &_key);

  };

}

#include "exception.hpp"

template<typename K, typename V>
me::map<K, V>::map(std::initializer_list<_Pair> _elements, _Alloc_Type* _alloc)
  : _entries(_elements, _alloc)
{
}

template<typename K, typename V>
me::map<K, V>::map(const map &_copy)
  : _entries(_copy._entries)
{
}

template<typename K, typename V>
me::map<K, V>::map(map &&_move)
  : _entries(_move._entries)
{
}

template<typename K, typename V>
me::map<K, V>::map(_Alloc_Type* _alloc)
  : _entries(_alloc)
{
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::data() const
{
  return this->_entries.data();
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::begin() const
{
  return this->_entries.begin();
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::end() const
{
  return this->_entries.end();
}

template<typename K, typename V>
me::pair<K, V>& me::map<K, V>::put(_Key &_key, _Value &&_value)
{
  return this->_entries.emplace_back(_key, _value);
}

template<typename K, typename V>
me::pair<K, V>& me::map<K, V>::put(_Key &_key, _Const_Value &_value)
{
  return this->_entries.emplace_back(_key, _value);
}

template<typename K, typename V>
void me::map<K, V>::erase(_Key &_key)
{
  for (_Pair* i = this->_entries._first; i < this->_entries._last; i++)
  {
    if (i->first == _key)
    {
      this->_entries.erase(i);
      break;
    }
  }
}

template<typename K, typename V>
void me::map<K, V>::clear()
{
  this->_entries.clear();
}

template<typename K, typename V>
const me::pair<K, V>* me::map<K, V>::find(_Key &_key) const
{
  for (_Const_Pair &entry : this->_entries)
  {
    if (entry.first == _key)
      return &entry;
  }
  return nullptr;
}

template<typename K, typename V>
bool me::map<K, V>::contains(_Key &_key) const
{
  return this->find(_key);
}

template<typename K, typename V>
V& me::map<K, V>::at(_Key &_key)
{
  for (_Pair &_entry : this->_entries)
  {
    if (_entry.first == _key)
      return _entry.second;
  }

  throw exception("me::map::at(): key not found");
}

template<typename K, typename V>
const V& me::map<K, V>::at(_Key &_key) const
{
  for (_Const_Pair &_entry : this->_entries)
  {
    if (_entry.first == _key)
      return _entry.second;
  }

  throw exception("me::map::at(): key not found");
}

template<typename K, typename V>
V& me::map<K, V>::at(_Key &_key, _Value &_default_value)
{
  for (_Pair &_entry : this->_entries)
  {
    if (_entry.first == _key)
      return _entry.second;
  }
  return _default_value;
}

template<typename K, typename V>
const V& me::map<K, V>::at(_Key &_key, _Const_Value &_default_value) const
{
  for (_Const_Pair &_entry : this->_entries)
  {
    if (_entry.first == _key)
      return _entry.second;
  }
  return _default_value;
}

template<typename K, typename V>
me::size_t me::map<K, V>::capacity() const
{
  return this->_entries.capacity();
}

template<typename K, typename V>
me::size_t me::map<K, V>::size() const
{
  return this->_entries.size();
}

template<typename K, typename V>
bool me::map<K, V>::is_empty() const
{
  return this->_entries.is_empty();
}

template<typename K, typename V>
V& me::map<K, V>::operator[](_Key &_key)
{
  for (_Pair &_entry : this->_entries)
  {
    if (_entry.first == _key)
      return _entry.second;
  }
  return this->put(_key, _Value()).second;
}

#endif
