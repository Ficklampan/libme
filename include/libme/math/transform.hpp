#ifndef LIBME_MATH_TRANSFORM_HPP
  #define LIBME_MATH_TRANSFORM_HPP

#include "vector.hpp"

namespace me::math {

  template<typename T>
  struct transform3 {
    vec3<T> position;
    vec3<T> rotation;
    vec3<T> scale;
  };

  template<typename T>
  struct transform2 {
    vec2<T> position;
    T rotation;
    vec2<T> scale;
  };

  typedef transform3<float> transform3f;
  typedef transform3<double> transform3d;
  typedef transform2<float> transform2f;
  typedef transform2<double> transform2d;

}

#endif
