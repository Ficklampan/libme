#ifndef LIBME_MATH_VECTOR4_HPP
  #define LIBME_MATH_VECTOR4_HPP

namespace me::math {

  template<typename T>
  struct vec4 {

    typedef uint8_t _Index;
    typedef T _Type;

    union {
      struct { _Type x, y, z, w; };
      _Type axis[3];
    };
  
    vec4(_Type _x, _Type _y, _Type _z, _Type _w);
    vec4(_Type _val);
    vec4();

    [[nodiscard]] T& operator[](_Index _index);
    [[nodiscard]] const T operator[](_Index _index) const;
  
    vec4& operator+=(const vec4 &_vec);
    vec4& operator+=(_Type _val);
    vec4& operator-=(const vec4 &_vec);
    vec4& operator-=(_Type _val);
    vec4& operator*=(const vec4 &_vec);
    vec4& operator*=(_Type _val);
    vec4& operator/=(const vec4 &_vec);
    vec4& operator/=(_Type _val);
  
    [[nodiscard]] bool operator==(const vec4 &_vec) const;
    [[nodiscard]] bool operator!=(const vec4 &_vec) const;
  
    [[nodiscard]] _Type length() const;
    [[nodiscard]] _Type distance(const vec4 &vec) const;
    [[nodiscard]] _Type dot(const vec4 &vec) const;

#ifdef LIBME_MATH_VECTOR2_HPP
    vec4(const vec2<T> &_vec, _Type _z, _Type _w);
    [[nodiscard]] vec2<T> to_vec2() const;
#endif
#ifdef LIBME_MATH_VECTOR3_HPP
    vec4(const vec3<T> &_vec, _Type _w);
    [[nodiscard]] vec3<T> to_vec3() const;
#endif
  
  };

  typedef vec4<float> vec4f;
  typedef vec4<double> vec4d;
  typedef vec4<int> vec4i;
  typedef vec4<unsigned int> vec4u;

}

template<typename T> me::math::vec4<T>& operator+=(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2);
template<typename T> me::math::vec4<T>& operator+=(const me::math::vec4<T> &_vec1, T _val);
template<typename T> me::math::vec4<T>& operator-=(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2);
template<typename T> me::math::vec4<T>& operator-=(const me::math::vec4<T> &_vec1, T _val);
template<typename T> me::math::vec4<T>& operator*=(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2);
template<typename T> me::math::vec4<T>& operator*=(const me::math::vec4<T> &_vec1, T _val);
template<typename T> me::math::vec4<T>& operator/=(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2);
template<typename T> me::math::vec4<T>& operator/=(const me::math::vec4<T> &_vec1, T _val);
  

/* [ LOCAL functions ] */
template<typename T>
me::math::vec4<T>::vec4(_Type _x, _Type _y, _Type _z, _Type _w)
  : x(_x), y(_y), z(_z), w(_w)
{
}

template<typename T>
me::math::vec4<T>::vec4(_Type _val)
  : x(_val), y(_val), z(_val), w(_val)
{
}

template<typename T>
me::math::vec4<T>::vec4()
{
}

template<typename T>
T& me::math::vec4<T>::operator[](_Index _index)
{
  return axis[_index];
}

template<typename T>
const T me::math::vec4<T>::operator[](_Index _index) const
{
  return axis[_index];
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator+=(const vec4<T> &_vec)
{
  this->x += _vec.x;
  this->y += _vec.y;
  this->z += _vec.z;
  this->w += _vec.w;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator+=(T _val)
{
  this->x += _val;
  this->y += _val;
  this->z += _val;
  this->w += _val;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator-=(const vec4<T> &_vec)
{
  this->x -= _vec.x;
  this->y -= _vec.y;
  this->z -= _vec.z;
  this->w -= _vec.w;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator-=(T _val)
{
  this->x -= _val;
  this->y -= _val;
  this->z -= _val;
  this->w -= _val;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator*=(const vec4<T> &_vec)
{
  this->x *= _vec.x;
  this->y *= _vec.y;
  this->z *= _vec.z;
  this->w *= _vec.w;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator*=(T _val)
{
  this->x *= _val;
  this->y *= _val;
  this->z *= _val;
  this->w *= _val;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator/=(const vec4<T> &_vec)
{
  this->x /= _vec.x;
  this->y /= _vec.y;
  this->z /= _vec.z;
  this->w /= _vec.w;
  return *this;
}

template<typename T>
me::math::vec4<T>& me::math::vec4<T>::operator/=(T _val)
{
  this->x /= _val;
  this->y /= _val;
  this->z /= _val;
  this->w /= _val;
  return *this;
}

template<typename T>
bool me::math::vec4<T>::operator==(const vec4<T> &_vec) const
{
  return this->x == _vec.x && this->y == _vec.y && this->z == _vec.z && this->w == _vec.w;
}

template<typename T>
bool me::math::vec4<T>::operator!=(const vec4<T> &_vec) const
{
  return this->x != _vec.x || this->y != _vec.y || this->z != _vec.z || this->w != _vec.w;
}

template<typename T>
T me::math::vec4<T>::length() const
{
  return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

template<typename T>
T me::math::vec4<T>::distance(const vec4<T> &_vec) const
{
  T x = this->x - _vec.x;
  T y = this->y - _vec.y;
  T z = this->z - _vec.z;
  T w = this->w - _vec.w;
  return sqrt(x * x + y * y + z * z + w * w);
}

template<typename T>
T me::math::vec4<T>::dot(const vec4<T> &_vec) const
{
  return this->x * _vec.x + this->y * _vec.y + this->x * _vec.z + this->w * _vec.w;
}

#ifdef LIBME_MATH_VECTOR2_HPP
template<typename T>
me::math::vec4<T>::vec4(const me::math::vec2<T> &_vec, _Type _z, _Type _w)
  : x(_vec.x), y(_vec.y), z(_z), w(_w)
{
}

template<typename T>
me::math::vec2<T> me::math::vec4<T>::to_vec2() const
{
  return {x, y};
}
#endif

#ifdef LIBME_MATH_VECTOR3_HPP
template<typename T>
me::math::vec4<T>::vec4(const me::math::vec3<T> &_vec, _Type _w)
  : x(_vec.x), y(_vec.y), z(_vec.z), w(_w)
{
}

template<typename T>
me::math::vec3<T> me::math::vec4<T>::to_vec3() const
{
  return {x, y, z};
}
#endif


/* [ GLOBAL functions ] */
template<typename T>
me::math::vec4<T> operator+(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2)
{
  return {_vec1.x + _vec2.x, _vec1.y + _vec2.y, _vec1.z + _vec2.z, _vec1.w + _vec2.w};
}

template<typename T>
me::math::vec4<T> operator+(const me::math::vec4<T> &_vec, T _val)
{
  return {_vec.x + _val, _vec.y + _val, _vec.z + _val, _vec.w + _val};
}

template<typename T>
me::math::vec4<T> operator-(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2)
{
  return {_vec1.x - _vec2.x, _vec1.y - _vec2.y, _vec1.z + _vec2.z, _vec1.w + _vec2.w};
}

template<typename T>
me::math::vec4<T> operator-(const me::math::vec4<T> &_vec, T _val)
{
  return {_vec.x - _val, _vec.y - _val, _vec.z - _val, _vec.w - _val};
}

template<typename T>
me::math::vec4<T> operator*(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2)
{
  return {_vec1.x * _vec2.x, _vec1.y * _vec2.y, _vec1.z * _vec2.z, _vec1.w * _vec2.w};
}

template<typename T>
me::math::vec4<T> operator*(const me::math::vec4<T> &_vec, T _val)
{
  return {_vec.x * _val, _vec.y * _val, _vec.z * _val, _vec.w * _val};
}

template<typename T>
me::math::vec4<T> operator/(const me::math::vec4<T> &_vec1, const me::math::vec4<T> &_vec2)
{
  return {_vec1.x / _vec2.x, _vec1.y / _vec2.y, _vec1.z / _vec2.z, _vec1.w / _vec2.w};
}

template<typename T>
me::math::vec4<T> operator/(const me::math::vec4<T> &_vec, T _val)
{
  return {_vec.x / _val, _vec.y / _val, _vec.z / _val, _vec.w / _val};
}

#endif
