#ifndef LIBME_MATH_VECTOR3_HPP
  #define LIBME_MATH_VECTOR3_HPP

#include "../math.hpp"

namespace me::math {

  template<typename T>
  struct vec3 {

    typedef uint8_t _Index;
    typedef T _Type;

    union {
      struct { _Type x, y, z; };
      _Type axis[3];
    };
  
    vec3(_Type _x, _Type _y, _Type _z);
    vec3(_Type _val);
    vec3();

    [[nodiscard]] T& operator[](_Index _index);
    [[nodiscard]] const T operator[](_Index _index) const;
  
    vec3& operator+=(const vec3 &_vec);
    vec3& operator+=(_Type _val);
    vec3& operator-=(const vec3 &_vec);
    vec3& operator-=(_Type _val);
    vec3& operator*=(const vec3 &_vec);
    vec3& operator*=(_Type _val);
    vec3& operator/=(const vec3 &_vec);
    vec3& operator/=(_Type _val);
  
    [[nodiscard]] bool operator==(const vec3 &_vec) const;
    [[nodiscard]] bool operator!=(const vec3 &_vec) const;

    vec3& normalize();
  
    [[nodiscard]] _Type length() const;
    [[nodiscard]] _Type distance(const vec3 &_vec) const;
    [[nodiscard]] _Type dot(const vec3 &_vec) const;
    [[nodiscard]] vec3 cross(const vec3 &_vec) const;

#ifdef LIBME_MATH_VECTOR2_HPP
    vec3(const vec2<T> &_vec, _Type _z);
    [[nodiscard]] vec2<T> to_vec2() const;
#endif
  
  };

  typedef vec3<float> vec3f;
  typedef vec3<double> vec3d;
  typedef vec3<int> vec3i;
  typedef vec3<unsigned int> vec3u;

  template<typename T> [[nodiscard]] vec3<T> normalize(const vec3<T> &_vec);
  template<typename T> [[nodiscard]] T length(const vec3<T> &_vec1);
  template<typename T> [[nodiscard]] T distance(const vec3<T> &_vec1, const vec3<T> &_vec2);
  template<typename T> [[nodiscard]] T dot(const vec3<T> &_vec1, const vec3<T> &_vec2);
  template<typename T> [[nodiscard]] vec3<T> cross(const vec3<T> &_vec1, const vec3<T> &_vec2);

}

template<typename T> me::math::vec3<T> operator+(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2);
template<typename T> me::math::vec3<T> operator+(const me::math::vec3<T> &_vec, T _val);
template<typename T> me::math::vec3<T> operator-(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2);
template<typename T> me::math::vec3<T> operator-(const me::math::vec3<T> &_vec, T _val);
template<typename T> me::math::vec3<T> operator*(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2);
template<typename T> me::math::vec3<T> operator*(const me::math::vec3<T> &_vec, T _val);
template<typename T> me::math::vec3<T> operator/(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2);
template<typename T> me::math::vec3<T> operator/(const me::math::vec3<T> &_vec, T _val);
  

/* [ LOCAL functions ] */
template<typename T>
me::math::vec3<T>::vec3(_Type _x, _Type _y, _Type _z)
  : x(_x), y(_y), z(_z)
{
}

template<typename T>
me::math::vec3<T>::vec3(_Type _val)
  : x(_val), y(_val), z(_val)
{
}

template<typename T>
me::math::vec3<T>::vec3()
{
}

template<typename T>
T& me::math::vec3<T>::operator[](_Index _index)
{
  return axis[_index];
}

template<typename T>
const T me::math::vec3<T>::operator[](_Index _index) const
{
  return axis[_index];
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator+=(const vec3<T> &_vec)
{
  this->x += _vec.x;
  this->y += _vec.y;
  this->z += _vec.z;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator+=(T _val)
{
  this->x += _val;
  this->y += _val;
  this->z += _val;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator-=(const vec3<T> &_vec)
{
  this->x -= _vec.x;
  this->y -= _vec.y;
  this->z -= _vec.z;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator-=(T _val)
{
  this->x -= _val;
  this->y -= _val;
  this->z -= _val;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator*=(const vec3<T> &_vec)
{
  this->x *= _vec.x;
  this->y *= _vec.y;
  this->z *= _vec.z;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator*=(T _val)
{
  this->x *= _val;
  this->y *= _val;
  this->z *= _val;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator/=(const vec3<T> &_vec)
{
  this->x /= _vec.x;
  this->y /= _vec.y;
  this->z /= _vec.z;
  return *this;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::operator/=(T _val)
{
  this->x /= _val;
  this->y /= _val;
  this->z /= _val;
  return *this;
}

template<typename T>
bool me::math::vec3<T>::operator==(const vec3<T> &_vec) const
{
  return this->x == _vec.x && this->y == _vec.y && this->z == _vec.z;
}

template<typename T>
bool me::math::vec3<T>::operator!=(const vec3<T> &_vec) const
{
  return this->x != _vec.x || this->y != _vec.y || this->z != _vec.z;
}

template<typename T>
me::math::vec3<T>& me::math::vec3<T>::normalize()
{
  T _scalar = invsqrt(this->x * this->x + (this->y * this->y + (this->z * this->z)));
  this->x *= _scalar;
  this->y *= _scalar;
  this->z *= _scalar;
  return *this;
}

template<typename T>
T me::math::vec3<T>::length() const
{
  return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template<typename T>
T me::math::vec3<T>::distance(const vec3<T> &_vec) const
{
  T x = this->x - _vec.x;
  T y = this->y - _vec.y;
  T z = this->z - _vec.z;
  return sqrt(x * x + y * y + z * z);
}

template<typename T>
T me::math::vec3<T>::dot(const vec3<T> &_vec) const
{
  return this->x * _vec.x + this->y * _vec.y + this->x * _vec.z;
}

template<typename T>
me::math::vec3<T> me::math::vec3<T>::cross(const vec3 &_vec) const
{
  T _x = this->y * _vec.z -this->z * _vec.y;
  T _y = this->z * _vec.x -this->x * _vec.z;
  T _z = this->x * _vec.y -this->y * _vec.x;
  return {_x, _y, _z};
}

#ifdef LIBME_MATH_VECTOR2_HPP
template<typename T>
me::math::vec3<T>::vec3(const me::math::vec2<T> &_vec, _Type _z)
  : x(_vec.x), y(_vec.y), z(_z)
{
}

template<typename T>
me::math::vec2<T> me::math::vec3<T>::to_vec2() const
{
  return {x, y};
}
#endif


/* [ GLOBAL functions ] */
template<typename T>
me::math::vec3<T> me::math::normalize(const vec3<T> &_vec)
{
  T _scalar = invsqrt(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
  return {_vec.x * _scalar, _vec.y * _scalar, _vec.z * _scalar};
}

template<typename T>
T me::math::length(const vec3<T> &_vec)
{
  return sqrt(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
}

template<typename T>
T me::math::distance(const vec3<T> &_vec1, const vec3<T> &_vec2)
{
  T _x = _vec1.x - _vec2.x;
  T _y = _vec1.y - _vec2.y;
  T _z = _vec1.z - _vec2.z;
  return sqrt(_x * _x + _y * _y + _z * _z);
}

template<typename T>
T me::math::dot(const vec3<T> &_vec1, const vec3<T> &_vec2)
{
  return _vec1.x * _vec2.x + (_vec1.y * _vec2.y + (_vec1.z * _vec2.z));
}

template<typename T>
me::math::vec3<T> me::math::cross(const vec3<T> &_vec1, const vec3<T> &_vec2)
{
  T _x = _vec1.y * _vec2.z + (-_vec1.z * _vec2.y);
  T _y = _vec1.z * _vec2.x + (-_vec1.x * _vec2.z);
  T _z = _vec1.x * _vec2.y + (-_vec1.y * _vec2.x);
  return {_x, _y, _z};
}

template<typename T>
me::math::vec3<T> operator+(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2)
{
  return {_vec1.x + _vec2.x, _vec1.y + _vec2.y, _vec1.z + _vec2.z};
}

template<typename T>
me::math::vec3<T> operator+(const me::math::vec3<T> &_vec, T _val)
{
  return {_vec.x + _val, _vec.y + _val, _vec.z + _val};
}

template<typename T>
me::math::vec3<T> operator-(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2)
{
  return {_vec1.x - _vec2.x, _vec1.y - _vec2.y, _vec1.z + _vec2.z};
}

template<typename T>
me::math::vec3<T> operator-(const me::math::vec3<T> &_vec, T _val)
{
  return {_vec.x - _val, _vec.y - _val, _vec.z - _val};
}

template<typename T>
me::math::vec3<T> operator*(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2)
{
  return {_vec1.x * _vec2.x, _vec1.y * _vec2.y, _vec1.z * _vec2.z};
}

template<typename T>
me::math::vec3<T> operator*(const me::math::vec3<T> &_vec, T _val)
{
  return {_vec.x * _val, _vec.y * _val, _vec.z * _val};
}

template<typename T>
me::math::vec3<T> operator/(const me::math::vec3<T> &_vec1, const me::math::vec3<T> &_vec2)
{
  return {_vec1.x / _vec2.x, _vec1.y / _vec2.y, _vec1.z / _vec2.z};
}

template<typename T>
me::math::vec3<T> operator/(const me::math::vec3<T> &_vec, T _val)
{
  return {_vec.x / _val, _vec.y / _val, _vec.z / _val};
}

#endif
