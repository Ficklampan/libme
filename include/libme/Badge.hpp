#ifndef LIBME_BADGE_HPP
  #define LIBME_BADGE_HPP

namespace me {

  template<class T>
  class Badge {

  private:

    friend T;

    Badge() = default;

  };

}

#endif
