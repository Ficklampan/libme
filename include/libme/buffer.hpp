#ifndef LIBME_BYTEBUFF_HPP
  #define LIBME_BYTEBUFF_HPP

#include "allocator.hpp"

namespace me {

  enum BufferFlags {
    BUFFER_FLAG_READ_ONLY = 0x01
  };

  enum ByteOrder {
    BYTE_ORDER_LITTLE_ENDIAN,
    BYTE_ORDER_BIG_ENDIAN
  };


  class buffer {

  protected:

    typedef size_t _Length;
    typedef size_t _Index;
    typedef uint8_t* _Position;
    typedef uint8_t _Byte;
    typedef uint8_t* _Data;
    typedef uint8_t _State;

    _Position _begin;
    _Position _end;
    _Position _pos;
    _State _state;

  public:

    inline buffer(_Length _length, _Data _data, _State _state);

    inline _Length remaining() const;
    inline _Index position() const;

    inline void position(_Index _pos);
    inline void set(BufferFlags _flag, bool _value);
    inline void flip();
    inline void rewind();

    /* writing */
    inline buffer& write(_Byte _byte);

    template<typename T>
    inline buffer& write(T &_data, ByteOrder order = BYTE_ORDER_LITTLE_ENDIAN);
    inline buffer& write(_Data _begin, const _Data _end);

    /* reading */
    inline _Byte read();
    inline _Byte peek() const;

    template<typename T>
    inline void read(T &_data, ByteOrder order = BYTE_ORDER_LITTLE_ENDIAN);
    inline void read(_Data _begin, const _Data _end);

  };

}

#include "exception.hpp"

me::buffer::buffer(_Length _length, _Data _data, _State _state)
  : _begin(_data), _end(_data + _length), _pos(_data), _state(_state)
{
}

me::size_t me::buffer::remaining() const
{
  return this->_end - this->_pos;
}

me::size_t me::buffer::position() const
{
  return this->_pos - this->_begin;
}

void me::buffer::position(_Index _pos)
{
  this->_pos = this->_begin + _pos;
}

void me::buffer::set(BufferFlags _flag, bool _value)
{
  if (_value)
    this->_state |= _flag;
  else
    this->_state &= (~_flag);
}

void me::buffer::flip()
{
  if (this->_state & BUFFER_FLAG_READ_ONLY)
    this->_state &= (~BUFFER_FLAG_READ_ONLY);
  else
    this->_state |= BUFFER_FLAG_READ_ONLY;
  this->_pos = this->_begin;
}

void me::buffer::rewind()
{
  this->_pos = this->_begin;
}

me::buffer& me::buffer::write(_Byte _byte)
{
  *this->_pos++ = _byte;
  return *this;
}

template<typename T>
me::buffer& me::buffer::write(T &_data, ByteOrder order)
{
  static constexpr _Length _len = sizeof(T);

  if (this->remaining() < _len)
    throw me::exception("me::buffer: required bytes: %lu, remaining bytes: %lu", _len, this->remaining());

  if (order == BYTE_ORDER_BIG_ENDIAN)
  {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static constexpr char* _end = (char*) &_data;
    char* _iter = (char*) &_data + _len;
    while (_iter != _end)
      *this->_pos++ = *_iter++;
#else
    static constexpr char* _end = (char*) &_data + _len;
    char* _iter = (char*) &_data;
    while (_iter != _end)
      *this->_pos++ = *_iter--;
#endif
  }else
  {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static constexpr char* _end = (char*) &_data + _len;
    char* _iter = (char*) &_data;
    while (_iter != _end)
      *_iter++ = *this->_pos++;
#else
    static constexpr char* _end = (char*) &_data;
    char* _iter = (char*) &_data + _len;
    while (_iter != _end)
      *_iter-- = *this->_pos++;
#endif
  }
  return *this;
}

me::buffer& me::buffer::write(_Data _begin, const _Data _end)
{
  const _Length _len = _end - _begin;

  if (this->remaining() < _len)
    throw me::exception("me::buffer: required bytes: %lu, remaining bytes: %lu", _len, this->remaining());

  while (_begin != _end)
    *this->_pos++ = *_begin++;
  return *this;
}

inline me::uint8_t me::buffer::read()
{
  return *this->_pos++;
}

inline me::uint8_t me::buffer::peek() const
{
  return *this->_pos;
}

template<typename T>
void me::buffer::read(T &_data, ByteOrder order)
{
  static constexpr _Length _len = sizeof(T);

  if (this->remaining() < _len)
    throw me::exception("me::buffer: required bytes: %lu, remaining bytes: %lu", _len, this->remaining());

  if (order == BYTE_ORDER_BIG_ENDIAN)
  {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static constexpr char* _end = (char*) &_data;
    char* _iter = (char*) &_data + _len;
    while (_iter != _end)
      *_iter++ = *this->_pos++;
#else
    static constexpr char* _end = (char*) &_data + _len;
    char* _iter = (char*) &_data;
    while (_iter != _end)
      *_iter-- = *this->_pos++;
#endif
  }else
  {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    static constexpr char* _end = (char*) &_data + _len;
    char* _iter = (char*) &_data;
    while (_iter != _end)
      *_iter++ = *this->_pos++;
#else
    static constexpr char* _end = (char*) &_data;
    char* _iter = (char*) &_data + _len;
    while (_iter != _end)
      *_iter-- = *this->_pos++;
#endif
  }
}

void me::buffer::read(_Data _begin, const _Data _end)
{
  const _Length _len = _end - _begin;

  if (this->remaining() < _len)
    throw me::exception("me::buffer: required bytes: %lu, remaining bytes: %lu", _len, this->remaining());

  while (_begin != _end)
    *_begin++ = *this->_pos++;
}

#endif
