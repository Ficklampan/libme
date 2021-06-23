#ifndef LIBME_PAIR_HPP
  #define LIBME_PAIR_HPP

#include "type.hpp"

namespace me {

  template<class T1, class T2>
  struct Pair {

  protected:

    typedef T1 First_Type;
    typedef const T1 Const_First_Type;
    typedef T2 Second_Type;
    typedef const T2 Const_Second_Type;

  public:

    First_Type first;
    Second_Type second;

    Pair(Const_First_Type &f, Const_Second_Type &s)
      : first(f), second(s)
    {
    }
    
    Pair(First_Type &&f, Second_Type &&s)
    {
      first = static_cast<First_Type&&>(f);
      second = static_cast<Const_Second_Type&&>(s);
    }

    bool operator==(const Pair<T1, T2> &pair) const
    {
      return first == pair.first && second == pair.second;
    }

    bool operator!=(const Pair<T1, T2> &pair) const
    {
      return !operator==(pair);
    }

  };

}

#endif
