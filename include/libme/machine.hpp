#ifndef LIBME_SYSTEM_HPP
  #define LIBME_SYSTEM_HPP

#include "type.hpp"

namespace me {

  enum MachineFlags {
  };


  struct Battery {
    bool charging;
    float capacity;
  };

  class Machine {

  protected:

    uint8_t flags;

  public:

    struct Battery {
      bool is_charging() const;
      float get_capacity() const;
    } const battery;

  };

}

#endif
