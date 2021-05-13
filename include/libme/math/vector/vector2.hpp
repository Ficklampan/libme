#ifndef LIBME_MATH_VECTOR2_HPP
  #define LIBME_MATH_VECTOR2_HPP

#include "../math.hpp"

namespace me::math {

  template<typename T>
  struct vec2 {

    typedef uint8_t _Index;
    typedef T _Type;

    union {
      struct { _Type x, y; };
      _Type axis[2];
    };
  
    vec2(_Type _x, _Type _y);
    vec2(_Type _val);
    vec2();

    [[nodiscard]] T& operator[](_Index _index);
    [[nodiscard]] const T operator[](_Index _index) const;
  
    vec2& operator+=(const vec2 &_vec);
    vec2& operator+=(_Type _val);
    vec2& operator-=(const vec2 &_vec);
    vec2& operator-=(_Type _val);
    vec2& operator*=(const vec2 &_vec);
    vec2& operator*=(_Type _val);
    vec2& operator/=(const vec2 &_vec);
    vec2& operator/=(_Type _val);
  
    [[nodiscard]] bool operator==(const vec2 &_vec) const;
    [[nodiscard]] bool operator!=(const vec2 &_vec) const;
  
    [[nodiscard]] _Type length() const;
    [[nodiscard]] _Type distance(const vec2 &vec) const;
    [[nodiscard]] _Type dot(const vec2 &vec) const;
  
  };

  typedef vec2<float> vec2f;
  typedef vec2<double> vec2d;
  typedef vec2<int> vec2i;
  typedef vec2<unsigned int> vec2u;

}

template<typename T> me::math::vec2<T>& operator+=(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2);
template<typename T> me::math::vec2<T>& operator+=(const me::math::vec2<T> &_vec1, T _val);
template<typename T> me::math::vec2<T>& operator-=(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2);
template<typename T> me::math::vec2<T>& operator-=(const me::math::vec2<T> &_vec1, T _val);
template<typename T> me::math::vec2<T>& operator*=(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2);
template<typename T> me::math::vec2<T>& operator*=(const me::math::vec2<T> &_vec1, T _val);
template<typename T> me::math::vec2<T>& operator/=(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2);
template<typename T> me::math::vec2<T>& operator/=(const me::math::vec2<T> &_vec1, T _val);
  

/* [ LOCAL functions ] */
template<typename T>
me::math::vec2<T>::vec2(_Type _x, _Type _y)
  : x(_x), y(_y)
{
}

template<typename T>
me::math::vec2<T>::vec2(_Type _val)
  : x(_val), y(_val)
{
}

template<typename T>
me::math::vec2<T>::vec2()
{
}

template<typename T>
T& me::math::vec2<T>::operator[](_Index _index)
{
  return axis[_index];
}

template<typename T>
const T me::math::vec2<T>::operator[](_Index _index) const
{
  return axis[_index];
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator+=(const vec2<T> &_vec)
{
  this->x += _vec.x;
  this->y += _vec.y;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator+=(T _val)
{
  this->x += _val;
  this->y += _val;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator-=(const vec2<T> &_vec)
{
  this->x -= _vec.x;
  this->y -= _vec.y;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator-=(T _val)
{
  this->x -= _val;
  this->y -= _val;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator*=(const vec2<T> &_vec)
{
  this->x *= _vec.x;
  this->y *= _vec.y;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator*=(T _val)
{
  this->x *= _val;
  this->y *= _val;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator/=(const vec2<T> &_vec)
{
  this->x /= _vec.x;
  this->y /= _vec.y;
  return *this;
}

template<typename T>
me::math::vec2<T>& me::math::vec2<T>::operator/=(T _val)
{
  this->x /= _val;
  this->y /= _val;
  return *this;
}

template<typename T>
bool me::math::vec2<T>::operator==(const vec2<T> &_vec) const
{
  return this->x == _vec.x && this->y == _vec.y;
}

template<typename T>
bool me::math::vec2<T>::operator!=(const vec2<T> &_vec) const
{
  return this->x != _vec.x || this->y != _vec.y;
}

template<typename T>
T me::math::vec2<T>::length() const
{
  return sqrt(this->x * this->x + this->y * this->y);
}

template<typename T>
T me::math::vec2<T>::distance(const vec2<T> &_vec) const
{
  T x = this->x - _vec.x;
  T y = this->y - _vec.y;
  return sqrt(x * x + y * y);
}

template<typename T>
T me::math::vec2<T>::dot(const vec2<T> &_vec) const
{
  return this->x * _vec.x + this->y * _vec.y;
}


/* [ GLOBAL functions ] */
template<typename T>
me::math::vec2<T> operator+(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2)
{
  return {_vec1.x + _vec2.x, _vec1.y + _vec2.y};
}

template<typename T>
me::math::vec2<T> operator+(const me::math::vec2<T> &_vec, T _val)
{
  return {_vec.x + _val, _vec.y + _val};
}

template<typename T>
me::math::vec2<T> operator-(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2)
{
  return {_vec1.x - _vec2.x, _vec1.y - _vec2.y};
}

template<typename T>
me::math::vec2<T> operator-(const me::math::vec2<T> &_vec, T _val)
{
  return {_vec.x - _val, _vec.y - _val};
}

template<typename T>
me::math::vec2<T> operator*(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2)
{
  return {_vec1.x * _vec2.x, _vec1.y * _vec2.y};
}

template<typename T>
me::math::vec2<T> operator*(const me::math::vec2<T> &_vec, T _val)
{
  return {_vec.x * _val, _vec.y * _val};
}

template<typename T>
me::math::vec2<T> operator/(const me::math::vec2<T> &_vec1, const me::math::vec2<T> &_vec2)
{
  return {_vec1.x / _vec2.x, _vec1.y / _vec2.y};
}

template<typename T>
me::math::vec2<T> operator/(const me::math::vec2<T> &_vec, T _val)
{
  return {_vec.x / _val, _vec.y / _val};
}

#endif
