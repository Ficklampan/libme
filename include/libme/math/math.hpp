#ifndef LIBME_MATH_HPP
  #define LIBME_MATH_HPP

#include "../type.hpp"

#include <cmath>

#define TO_RADIANS    0.017453292519943295
#define TO_DEGREES    57.29577951308232

namespace me::math {

#ifdef ME_REAL_T_FLOAT
  typedef double real_t;
#else
  typedef float real_t;
#endif

  inline double invsqrt(double v)
  {
    return 1.0 / ::sqrt(v);
  }

  inline float invsqrtf(float v)
  {
    return 1.0F / ::sqrt(v);
  }

  inline size_t max(size_t a, size_t b)
  {
    return a > b ? a : b;
  }

  inline size_t min(size_t a, size_t b)
  {
    return a < b ? a : b;
  }

  inline double cos(double v)
  {
    return ::cos(v);
  }

  inline float cos(float v)
  {
    return static_cast<float>(::cos(v));
  }

  inline double sin(double v)
  {
    return ::sin(v);
  }

  inline float sin(float v)
  {
    return static_cast<float>(::sin(v));
  }

  inline double tan(double v)
  {
    return ::tan(v);
  }

  inline float tan(float v)
  {
    return static_cast<float>(::tan(v));
  }

  inline float radians(float deg)
  {
    return deg * (float) TO_RADIANS;
  }

  inline double radians(double deg)
  {
    return deg * (double) TO_RADIANS;
  }

  inline float degrees(float rad)
  {
    return rad * (float) TO_DEGREES;
  }

  inline double degrees(double rad)
  {
    return rad * (double) TO_DEGREES;
  }

}

#endif
