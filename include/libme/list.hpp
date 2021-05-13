#ifndef LIBME_LIST_HPP
  #define LIBME_LIST_HPP

#include "type.hpp"
#include "allocator.hpp"

namespace me {

  template<typename T>
  class list {

  protected:

    struct _Node;

    typedef allocator _Alloc_Type;
    typedef T _Type;
    typedef const T _Const_Type;
    typedef T* _Pointer;
    typedef _Node* _Node_Pointer;

    struct _Node {

      _Type _value;

      _Node_Pointer _prev;
      _Node_Pointer _next;

    };

    _Alloc_Type* _alloc;
    _Node _next;

  public:

    _Node* __end_(_Node* _node)
    {
      if (_node->_next == nullptr)
	return _node;
      return __end_(_node->_next);
    }

    void push_back(_Type &&_value);
    void push_back(_Const_Type &_value);

  };

}

template<typename T>
void me::list<T>::push_back(_Type &&_value)
{
  _alloc->construct(__end_(&_next), _value);
}

template<typename T>
void me::list<T>::push_back(_Const_Type &_value)
{
  _alloc->construct(__end_(&_next), _value);
}
#endif
