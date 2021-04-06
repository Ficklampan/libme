#ifndef LIBME_BIT_HPP
  #define LIBME_BIT_HPP

#include "type.hpp"

#include <type_traits>

namespace me::bit {

  template<typename T> inline bool is_signed(T _i) requires std::is_signed_v<T>;
  template<typename T> [[nodiscard]] inline T unsign(T _i) requires std::is_signed_v<T>;

}

template<typename T>
bool me::bit::is_signed(T _i) requires std::is_signed_v<T>
{
  static constexpr T _sig_bit = static_cast<T>(-1);
  return _i & _sig_bit;
}

template<typename T>
T me::bit::unsign(T _i) requires std::is_signed_v<T>
{
  static constexpr T _sig_bit = static_cast<T>(-1);
  return _i & (~_sig_bit);
}

#endif
