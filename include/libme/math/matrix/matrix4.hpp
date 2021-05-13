#ifndef LIBME_MATH_MATRIX4_HPP
  #define LIBME_MATH_MATRIX4_HPP

#include "../vector.hpp"

namespace me::math {

  template<typename T>
  struct mat4 {

    typedef uint8_t _Index;
    typedef T _Type;

    _Type matrix[16];

    _Type& at(_Index _index);
    _Type at(_Index _index) const;

    mat4& operator+=(const mat4 &_mat);
    mat4& operator+=(const vec4<T> &_mat);
    mat4& operator*=(const mat4 &_mat);
    mat4& operator*=(const vec4<T> &_mat);

    _Type& operator[](_Index _index);
    _Type operator[](_Index _index) const;

  };

  typedef mat4<float> mat4f;
  typedef mat4<double> mat4d;

  template<typename T> int transpose_mat4(mat4<T> &_mat);
  template<typename T> int identify_mat4(mat4<T> &_mat);
  template<typename T> int perspective_mat4(mat4<T> &_mat, T _fov, T _aspect, T _znear, T _zfar);
  template<typename T> int look_at_mat4(mat4<T> &_mat, const vec3<T> &_eye, const vec3<T> &_center, const vec3<T> &_up);
  template<typename T> int translate_mat4(mat4<T> &_mat, const vec3<T> &_vec);
  template<typename T> int scale_mat4(mat4<T> &_mat, const vec3<T>& _vec);
  template<typename T> int rotate_mat4(mat4<T> &_mat, T _angle, const vec3<T>& _vec);
  template<typename T> int rotate_x_mat4(mat4<T> &_mat, T _angle);
  template<typename T> int rotate_y_mat4(mat4<T> &_mat, T _angle);
  template<typename T> int rotate_z_mat4(mat4<T> &_mat, T _angle);

}

template<typename T> me::math::mat4<T> operator+(const me::math::mat4<T> &_mat1, const me::math::mat4<T> &_mat2);
template<typename T> me::math::vec4<T> operator*(const me::math::vec4<T> &_vec, const me::math::mat4<T> &_mat);


/* [ LOCAL functions ] */
template<typename T>
T& me::math::mat4<T>::at(_Index _index)
{
  return this->matrix[_index];
}

template<typename T>
T me::math::mat4<T>::at(_Index _index) const
{
  return this->matrix[_index];
}

template<typename T>
me::math::mat4<T>& me::math::mat4<T>::operator+=(const mat4 &_mat)
{
  for (uint8_t i = 0; i < 16; i++)
    this->at(i) = _mat[i];
}

template<typename T>
me::math::mat4<T>& me::math::mat4<T>::operator+=(const vec4<T> &_mat)
{
}

template<typename T>
me::math::mat4<T>& me::math::mat4<T>::operator*=(const mat4 &_mat)
{
}

template<typename T>
me::math::mat4<T>& me::math::mat4<T>::operator*=(const vec4<T> &_mat)
{
}

template<typename T>
T& me::math::mat4<T>::operator[](_Index _index)
{
  return matrix[_index];
}

template<typename T>
T me::math::mat4<T>::operator[](_Index _index) const
{
  return matrix[_index];
}


/* [ GLOBAL functions ] */
template<typename T>
me::math::mat4<T> operator+(const me::math::mat4<T> &_mat1, const me::math::mat4<T> &_mat2)
{
  me::math::mat4<T> _out;
  for (me::uint8_t i = 0; i < 16; i++)
    _out[i] = _mat1[i] + _mat2[i];
}

template<typename T>
me::math::vec4<T> operator*(const me::math::vec4<T> &_vec, const me::math::mat4<T> &_mat)
{
  T x = _mat[0] * _vec.x + _mat[4] * _vec.y + _mat[8] * _vec.z + _mat[12] * _vec.w;
  T y = _mat[1] * _vec.x + _mat[5] * _vec.y + _mat[9] * _vec.z + _mat[13] * _vec.w;
  T z = _mat[2] * _vec.x + _mat[6] * _vec.y + _mat[10] * _vec.z + _mat[14] * _vec.w;
  T w = _mat[3] * _vec.x + _mat[7] * _vec.y + _mat[11] * _vec.z + _mat[15] * _vec.w;
  if (w != 0)
  {
    x /= w;
    y /= w;
    z /= w;
  }
  return {x, y, z, w};
}

template<typename T>
int me::math::transpose_mat4(mat4<T> &_mat)
{
  mat4<T> _new;
  _new[0] = _mat[12];
  _new[1] = _mat[8];
  _new[2] = _mat[4];
  _new[3] = _mat[0];
  _new[4] = _mat[13];
  _new[5] = _mat[9];
  _new[6] = _mat[5];
  _new[7] = _mat[1];
  _new[8] = _mat[14];
  _new[9] = _mat[10];
  _new[10] = _mat[6];
  _new[11] = _mat[2];
  _new[12] = _mat[15];
  _new[13] = _mat[11];
  _new[14] = _mat[7];
  _new[15] = _mat[3];
  return 0;
}

template<typename T>
int me::math::identify_mat4(mat4<T> &_mat)
{
  for (uint8_t i = 0; i < 16; i++)
    _mat[i] = 0.0;

  _mat[0] = 1.0;
  _mat[5] = 1.0;
  _mat[10] = 1.0;
  _mat[15] = 1.0;
  return 0;
}

template<typename T>
int me::math::perspective_mat4(mat4<T> &_mat, T _fov, T _aspect, T _znear, T _zfar)
{
  T _depth = _zfar - _znear;
  T _inv_depth = 1.0 / _depth;
  T _inv_fov = 1.0 / tan(0.5 * _fov);

  _mat[0] = _inv_fov / _aspect;
  _mat[5] = _inv_fov;
  _mat[10] = -(_zfar * _inv_depth);
  _mat[14] = (-_zfar * _znear) * _inv_depth;
  _mat[11] = -1.0;
  _mat[15] = 0.0;
  return 0;
}

template<typename T>
int me::math::look_at_mat4(mat4<T> &_mat, const vec3<T> &_eye, const vec3<T> &_center, const vec3<T> &_up)
{
  const vec3<T> _f1(normalize(_center - _eye));
  const vec3<T> _f2(normalize(cross(_f1, _up)));
  const vec3<T> _f3(cross(_f2, _f1));

  _mat[0] = _f2.x;
  _mat[4] = _f2.y;
  _mat[8] = _f2.z;
  _mat[1] = _f3.x;
  _mat[5] = _f3.y;
  _mat[9] = _f3.z;
  _mat[2] = -_f1.x;
  _mat[6] = -_f1.y;
  _mat[10] = -_f1.z;

  _mat[12] = -dot(_f2, _eye);
  _mat[13] = -dot(_f3, _eye);
  _mat[14] = dot(_f1, _eye);

  /*
  const vec3<T> _f1 = normalize(_center - _eye);
  const vec3<T> _f2 = normalize(cross(_up, _f1));
  const vec3<T> _f3 = cross(_f1, _f2);

  _mat[0] = _f2.x;
  _mat[4] = _f2.y;
  _mat[8] = _f2.z;
  _mat[1] = _f3.x;
  _mat[5] = _f3.y;
  _mat[9] = _f3.z;
  _mat[2] = _f1.x;
  _mat[6] = _f1.y;
  _mat[10] = _f1.z;

  _mat[12] = -dot(_f2, _eye);
  _mat[13] = -dot(_f3, _eye);
  _mat[14] = -dot(_f1, _eye);
  */
  return 0;
}

template<typename T>
int me::math::translate_mat4(mat4<T> &_mat, const vec3<T> &_vec)
{
  /* multiply? */
  _mat[3] = _vec.x;
  _mat[7] = _vec.y;
  _mat[11] = _vec.z;
  return 0;
}

template<typename T>
int me::math::scale_mat4(mat4<T> &_mat, const vec3<T> &_vec)
{
  _mat[0] = _vec.x;
  _mat[5] = _vec.y;
  _mat[10] = _vec.z;
  return 0;
}

template<typename T>
int me::math::rotate_mat4(mat4<T> &_mat, T _angle, const vec3<T> &_vec)
{
  return 0;
}

template<typename T>
int me::math::rotate_x_mat4(mat4<T> &_mat, T _angle)
{
  _mat[5] = cos(_angle);
  _mat[6] = -sin(_angle);
  _mat[9] = sin(_angle);
  _mat[10] = cos(_angle);
  return 0;
}

template<typename T>
int me::math::rotate_y_mat4(mat4<T> &_mat, T _angle)
{
  _mat[0] = cos(_angle);
  _mat[2] = sin(_angle);
  _mat[8] = -sin(_angle);
  _mat[10] = cos(_angle);
  return 0;
}

template<typename T>
int me::math::rotate_z_mat4(mat4<T> &_mat, T _angle)
{
  _mat[0] = cos(_angle);
  _mat[1] = -sin(_angle);
  _mat[4] = sin(_angle);
  _mat[5] = cos(_angle);
  return 0;
}

#endif
