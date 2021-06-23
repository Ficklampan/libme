#ifndef LIBME_MAP_HPP
  #define LIBME_MAP_HPP

#include "type.hpp"
#include "pair.hpp"
#include "vector.hpp"

namespace me {

  template<typename K, typename V, class A = Allocator>
  class Map {

  protected:

    typedef A Alloc;
    typedef size_t Size;
    typedef K Key;
    typedef V Value;
    typedef Vector<Pair<K, V>> Container;
    typedef const Vector<Pair<K, V>> Const_Container;
    typedef Pair<K, V>* Iterator;
    typedef const Pair<K, V>* Const_Iterator;

    Container entries_;

  public:

    Map(std::initializer_list<me::Pair<Key, Value>> elements);
    Map(const Map<K, V, A> &copy);
    Map(Map<K, V, A> &&move);
    Map();

    [[nodiscard]] Iterator data() const;
    [[nodiscard]] Iterator begin() const;
    [[nodiscard]] Iterator end() const;
    [[nodiscard]] Const_Iterator cbegin() const;
    [[nodiscard]] Const_Iterator cend() const;

    Pair<Key, Value>& put(const Key &key, Value &&value);
    Pair<Key, Value>& put(const Key &key, const Value &value);

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

    Value& operator[](const Key &key);

    template<class A2> [[nodiscard]] bool operator==(const Map<K, V, A2> &Map) const;
    template<class A2> [[nodiscard]] bool operator!=(const Map<K, V, A2> &Map) const;

  };

}

#include "exception.hpp"

template<typename K, typename V, class A>
me::Map<K, V, A>::Map(std::initializer_list<me::Pair<Key, Value>> elements)
  : entries_(elements)
{
}

template<typename K, typename V, class A>
me::Map<K, V, A>::Map(const Map<K, V, A> &copy)
  : entries_(copy.entries_)
{
}

template<typename K, typename V, class A>
me::Map<K, V, A>::Map(Map<K, V, A> &&move)
  : entries_(move.entries_)
{
}

template<typename K, typename V, class A>
me::Map<K, V, A>::Map()
  : entries_()
{
}

template<typename K, typename V, class A>
me::Pair<K, V>* me::Map<K, V, A>::data() const
{
  return entries_.data();
}

template<typename K, typename V, class A>
me::Pair<K, V>* me::Map<K, V, A>::begin() const
{
  return entries_.begin();
}

template<typename K, typename V, class A>
me::Pair<K, V>* me::Map<K, V, A>::end() const
{
  return entries_.end();
}

template<typename K, typename V, class A>
me::Pair<K, V>& me::Map<K, V, A>::put(const Key &key, Value &&value)
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

template<typename K, typename V, class A>
me::Pair<K, V>& me::Map<K, V, A>::put(const Key &key, const Value &value)
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

template<typename K, typename V, class A>
void me::Map<K, V, A>::erase(const Key &key)
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

template<typename K, typename V, class A>
void me::Map<K, V, A>::clear()
{
  entries_.clear();
}

template<typename K, typename V, class A>
me::Pair<K, V>* me::Map<K, V, A>::find(const Key &key) const
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i;
  }
  return nullptr;
}

template<typename K, typename V, class A>
bool me::Map<K, V, A>::contains(const Key &key) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return true;
  }
  return false;
}

template<typename K, typename V, class A>
V& me::Map<K, V, A>::at(const Key &key)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }

  throw RuntimeError("me::Map::at(): key not found");
}

template<typename K, typename V, class A>
const V& me::Map<K, V, A>::at(const Key &key) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }

  throw RuntimeError("me::Map::at(): key not found");
}

template<typename K, typename V, class A>
V& me::Map<K, V, A>::at(const Key &key, Value &default_value)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return default_value;
}

template<typename K, typename V, class A>
const V& me::Map<K, V, A>::at(const Key &key, const Value &default_value) const
{
  for (Const_Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return default_value;
}

template<typename K, typename V, class A>
me::size_t me::Map<K, V, A>::capacity() const
{
  return entries_.capacity();
}

template<typename K, typename V, class A>
me::size_t me::Map<K, V, A>::size() const
{
  return entries_.size();
}

template<typename K, typename V, class A>
bool me::Map<K, V, A>::is_empty() const
{
  return entries_.is_empty();
}

template<typename K, typename V, class A>
V& me::Map<K, V, A>::operator[](const Key &key)
{
  for (Iterator i = entries_.begin(); i != entries_.end(); i++)
  {
    if (i->first == key)
      return i->second;
  }
  return this->put(key, Value()).second;
}

template<typename K, typename V, class A>
template<class A2>
bool me::Map<K, V, A>::operator==(const Map<K, V, A2> &Map) const
{
  return entries_ == Map.entries_;
}

template<typename K, typename V, class A>
template<class A2>
bool me::Map<K, V, A>::operator!=(const Map<K, V, A2> &Map) const
{
  return entries_ != Map.entries_;
}

#endif
