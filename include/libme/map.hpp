#ifndef LIBME_MAP_HPP
  #define LIBME_MAP_HPP

#include "type.hpp"
#include "pair.hpp"
#include "vector.hpp"

namespace me {

  template<typename K, typename V>
  class map {

  protected:

    typedef allocator Alloc_Type;
    typedef size_t Size;
    typedef K Key;
    typedef V Value;
    typedef vector<pair<K, V>> Container;
    typedef const vector<pair<K, V>> Const_Container;
    typedef pair<K, V>* Iterator;
    typedef const pair<K, V>* Const_Iterator;

    Container entries_;

  public:

    map(std::initializer_list<me::pair<Key, Value>> elements, Alloc_Type* alloc = allocator::_default());
    map(const map &copy);
    map(map &&move);
    map(Alloc_Type* alloc = allocator::_default());

    [[nodiscard]] Iterator data() const;
    [[nodiscard]] Iterator begin() const;
    [[nodiscard]] Iterator end() const;
    [[nodiscard]] Const_Iterator cbegin() const;
    [[nodiscard]] Const_Iterator cend() const;

    pair<Key, Value>& put(const Key &key, Value &&value);
    pair<Key, Value>& put(const Key &key, const Value &value);

    void erase(const Key &key);
    void clear();

    [[nodiscard]] Iterator find(const Key &key) const;
    [[nodiscard]] bool contains(const Key &key) const;

    [[nodiscard]] Value& at(const Key &key);
    [[nodiscard]] const Value& at(const Key &key) const;
    [[nodiscard]] Value& at(const Key &key, Value &default_value);
    [[nodiscard]] const Value& at(const Key &key, const Value &default_value) const;

    [[nodiscard]] Size capacity() const;
    [[nodiscard]] Size size() const;
    [[nodiscard]] bool is_empty() const;

    Value& operator[](const Key &_key);

    [[nodiscard]] bool operator==(const map _map) const;

  };

}

#include "exception.hpp"

template<typename K, typename V>
me::map<K, V>::map(std::initializer_list<me::pair<Key, Value>> elements, Alloc_Type* alloc)
  : entries_(elements, alloc)
{
}

template<typename K, typename V>
me::map<K, V>::map(const map &copy)
  : entries_(copy.entries_)
{
}

template<typename K, typename V>
me::map<K, V>::map(map &&move)
  : entries_(move.entries_)
{
}

template<typename K, typename V>
me::map<K, V>::map(Alloc_Type* alloc)
  : entries_(alloc)
{
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::data() const
{
  return entries_.data();
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::begin() const
{
  return entries_.begin();
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::end() const
{
  return entries_.end();
}

template<typename K, typename V>
me::pair<K, V>& me::map<K, V>::put(const Key &key, Value &&value)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
    {
      i->second = static_cast<Value&&>(value);
      return *i;
    }
  }
  return entries_.emplace_back(key, value);
}

template<typename K, typename V>
me::pair<K, V>& me::map<K, V>::put(const Key &key, const Value &value)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
    {
      i->second = value;
      return *i;
    }
  }
  return entries_.emplace_back(key, value);
}

template<typename K, typename V>
void me::map<K, V>::erase(const Key &key)
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
    {
      entries_.erase(i);
      break;
    }
  }
}

template<typename K, typename V>
void me::map<K, V>::clear()
{
  entries_.clear();
}

template<typename K, typename V>
me::pair<K, V>* me::map<K, V>::find(const Key &key) const
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i;
  }
  return nullptr;
}

template<typename K, typename V>
bool me::map<K, V>::contains(const Key &key) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return true;
  }
  return false;
}

template<typename K, typename V>
V& me::map<K, V>::at(const Key &key)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }

  throw exception("me::map::at(): key not found");
}

template<typename K, typename V>
const V& me::map<K, V>::at(const Key &key) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }

  throw exception("me::map::at(): key not found");
}

template<typename K, typename V>
V& me::map<K, V>::at(const Key &key, Value &default_value)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return default_value;
}

template<typename K, typename V>
const V& me::map<K, V>::at(const Key &key, const Value &default_value) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return default_value;
}

template<typename K, typename V>
me::size_t me::map<K, V>::capacity() const
{
  return entries_.capacity();
}

template<typename K, typename V>
me::size_t me::map<K, V>::size() const
{
  return entries_.size();
}

template<typename K, typename V>
bool me::map<K, V>::is_empty() const
{
  return entries_.is_empty();
}

template<typename K, typename V>
V& me::map<K, V>::operator[](const Key &key)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return this->put(key, Value()).second;
}

template<typename K, typename V>
bool me::map<K, V>::operator==(const map map) const
{
  return entries_ == map.entries_;
}

#endif
