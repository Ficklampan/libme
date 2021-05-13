#include "filesystem.hpp"
#include "exception.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

static constexpr me::uint8_t _FILE_TYPE_DIRENT_LEN = 15;
static constexpr me::filesystem::file_type _FILE_TYPE_DIRENT[_FILE_TYPE_DIRENT_LEN] = {
  /* UNKNOWN 0  */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* FIFO    1  */ me::filesystem::FILE_TYPE_FIFO,
  /* CHR     2  */ me::filesystem::FILE_TYPE_CHARACTER,
  /* ------- 3  */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* DIR     4  */ me::filesystem::FILE_TYPE_DIRECTORY,
  /* ------- 5  */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* BLK     6  */ me::filesystem::FILE_TYPE_BLOCK,
  /* ------- 7  */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* REG     8  */ me::filesystem::FILE_TYPE_REGULAR,
  /* ------- 9  */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* LNK     10 */ me::filesystem::FILE_TYPE_SYMBOLIC_LINK,
  /* ------- 11 */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* SOCK    12 */ me::filesystem::FILE_TYPE_SOCKET,
  /* ------- 13 */ me::filesystem::FILE_TYPE_UNKNOWN,
  /* WHT     14 */ me::filesystem::FILE_TYPE_UNKNOWN /* what is WHT */
};

#define _ME_FILESYSTEM_IS_HIDDEN_FILE(p) ((p)[0] == '.')


bool me::filesystem::exists(const _path_type &_path)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  return ::access(_temp, F_OK) == 0;
}

void me::filesystem::create_file(const _path_type &_path)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  ::FILE* _file = ::fopen(_temp, "wb");
  if (!_file)
    throw exception("me::filesystem::create_file() -> path[%s] %s", _temp, ::strerror(errno));

  ::fwrite(nullptr, 0, 0, _file);
  ::fclose(_file);
}

void me::filesystem::make_directory(const _path_type &_path, uint16_t _mode)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  if (::mkdir(_temp, _mode) != 0)
    throw exception("me::filesystem::make_directory() -> path[%s] %s", _temp, ::strerror(errno));
}

void me::filesystem::make_directories(const _path_type &_path)
{
}

void me::filesystem::status(const _path_type &_path, file_status &_status)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  struct stat _stat;
  ::stat(_temp, &_stat);

  _status.device = _stat.st_dev;
  _status.mode = _stat.st_mode;
  _status.user_id = _stat.st_uid;
  _status.group_id = _stat.st_gid;
  _status.device_id = _stat.st_rdev;
  _status.device = _stat.st_dev;
  _status.size = _stat.st_size;
  _status.block_size = _stat.st_blksize;
  _status.block_count = _stat.st_blocks;
}

void me::filesystem::enumerate_entries(const _path_type &_path, size_t &_count, directory_entry* _entries, uint8_t _options)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  ::DIR* _dir = ::opendir(_temp);
  if (!_dir)
    throw exception("me::filesystem::enumerate_entries() -> path[%s] %s", _temp, ::strerror(errno));

  _count = 0;
  while (true)
  {
    ::dirent* _dirent = ::readdir(_dir);
    if (!_dirent)
    {
      if (errno != 0)
	throw exception("me::filesystem::enumerate_entries() -> path[%s] %s", ::strerror(errno));
      break;
    }

    bool junk = strcmp(".", _dirent->d_name) || strcmp("..", _dirent->d_name);
    if (!(_options & ENUMERATE_ENTRIES_JUNK) && junk)
      continue;

    if (_options & ENUMERATE_ENTRIES_IGNORE_HIDDEN_FILES && _ME_FILESYSTEM_IS_HIDDEN_FILE(_dirent->d_name))
      continue;

    if (!junk && _options & ENUMERATE_ENTRIES_SUB_DIRECTORIES && _dirent->d_type == DT_DIR)
    {
      enumerate_entries(_path, _count, _entries, _options);
      continue;
    }

    if (_entries != nullptr)
    {
      directory_entry &_entry = _entries[_count];
      _entry.type = _FILE_TYPE_DIRENT[_dirent->d_type];
      memcpy(_entry.name, _dirent->d_name, 256);
    }
    _count++;
  }
}

void me::filesystem::change_extension(const _path_type &_path, char* _str)
{
}

void me::filesystem::change_name(const _path_type &_path, char* _str)
{
}

void me::filesystem::extension(const _path_type &_path, char* _str)
{
}

void me::filesystem::directory(const _path_type &_path, char* _str)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  const char* _iter = _temp;
  const char* _last = _temp;
  while (*_iter != '\0') /* TODO: reverse faster? */
  {
    if (*_iter == _ME_FILESYSTEM_PATH_SEPARATOR)
      _last = _iter;
    _iter++;
  }

  size_t _len = _last - _temp;
  if (_len == 0 && _path[0] == _ME_FILESYSTEM_PATH_SEPARATOR)
    _len = 1;

  if (_str != nullptr)
  {
    for (size_t i = 0; i < _len; i++)
      _str[i] = _path[i];
  }
  _str[_len] = '\0';
}

void me::filesystem::name(const _path_type &_path, char* _str)
{
}

void me::filesystem::absolute(const _path_type &_path, char* _str)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  ::realpath(_temp, _str);
}

void me::filesystem::canonical(const _path_type &_path, char* _str)
{
}

void me::filesystem::relative(const _path_type &_path, char* _str)
{
}

void me::filesystem::move(const _path_type &_src, const _path_type &_dest, size_t _buff_size, uint8_t _options)
{
}

void me::filesystem::copy(const _path_type &_src, const _path_type &_dest, size_t _buff_size, uint8_t _options)
{
  char _src_temp[_src.size() + 1];
  _src.c_str(_src_temp);
  char _dest_temp[_dest.size() + 1];
  _dest.c_str(_dest_temp);

  ::FILE* _src_file = ::fopen(_src_temp, "rb");
  if (!_src_file)
    throw exception("me::filesystem::copy() -> (source) path[%s] %s", _src_temp, ::strerror(errno));

  ::FILE* _dest_file = ::fopen(_dest_temp, "ab");
  if (!_dest_file)
    throw exception("me::filesystem::copy() -> (destination) path[%s] %s", _dest_temp, ::strerror(errno));

  ::fseek(_src_file, 0, SEEK_END);
  size_t _len = ::ftell(_src_file);
  ::rewind(_src_file);

  size_t _pos = 0;
  char _buffer[_buff_size];
  while (_pos < _len)
  {
    size_t _buffer_size = MIN(_buff_size, _len - _pos);
    ::fread(_buffer, _buffer_size, 1, _src_file);
    ::fwrite(_buffer, _buffer_size, 1, _dest_file);
  }

  ::fclose(_src_file);
  ::fclose(_dest_file);
}

void me::filesystem::remove(const _path_type &_path)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  if (::remove(_temp) != 0)
    throw exception("me::filesystem::remove() -> path[%s] %s", _temp, ::strerror(errno));
}

void me::filesystem::read(const _path_type &_path, size_t _len, void* _data)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  ::FILE* _file = ::fopen(_temp, "rb");
  if (!_file)
    throw exception("me::filesystem::read() -> path[%s] %s", _temp, ::strerror(errno));

  /* read file */
  ::fread(_data, 1, _len, _file);
  ::fclose(_file);
}

void me::filesystem::write(const _path_type &_path, size_t _len, void* _data)
{
  char _temp[_path.size() + 1];
  _path.c_str(_temp);

  ::FILE* _file = ::fopen(_temp, "wb");
  if (!_file)
    throw exception("me::filesystem::write() -> path[%s] %s", _temp, ::strerror(errno));

  /* write file */
  ::fwrite(_data, 1, _len, _file);
  ::fclose(_file);
}
