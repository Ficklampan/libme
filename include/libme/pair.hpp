#ifndef LIBME_PAIR_HPP
  #define LIBME_PAIR_HPP

#include "type.hpp"

namespace me {

  template<class T1, class T2>
  struct pair {

    typedef T1 _First_Type;
    typedef const T1 _Const_First_Type;
    typedef T2 _Second_Type;
    typedef const T2 _Const_Second_Type;

    _First_Type first;
    _Second_Type second;

    pair(_Const_First_Type &_first, _Const_Second_Type &_second)
      : first(_first), second(_second)
    {
    }
    
    pair(_First_Type &&first, _Second_Type &&second)
    {
      this->first = static_cast<_First_Type&&>(first);
      this->second = static_cast<_Const_Second_Type&&>(second);
    }

    bool operator==(const pair &_pair) const
    {
      return first = _pair.first && second == _pair.second;
    }

    bool operator!=(const pair &_pair) const
    {
      return !operator==(_pair);
    }

  };

}

#endif
