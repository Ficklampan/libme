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


bool me::filesystem::exists(const path_type &path)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  return ::access(temp, F_OK) == 0;
}

void me::filesystem::create_file(const path_type &path)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  ::FILE* file = ::fopen(temp, "wb");
  if (!file)
    throw exception("me::filesystem::create_file() -> path[%s] %s", temp, ::strerror(errno));

  ::fwrite(nullptr, 0, 0, file);
  ::fclose(file);
}

void me::filesystem::make_directory(const path_type &path, uint16_t mode)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  if (::mkdir(temp, mode) != 0)
    throw exception("me::filesystem::make_directory() -> path[%s] %s", temp, ::strerror(errno));
}

void me::filesystem::make_directories(const path_type &path)
{
}

void me::filesystem::status(const path_type &path, file_status &status)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  struct stat stat;
  ::stat(temp, &stat);

  status.device = stat.st_dev;
  status.mode = stat.st_mode;
  status.user_id = stat.st_uid;
  status.group_id = stat.st_gid;
  status.device_id = stat.st_rdev;
  status.device = stat.st_dev;
  status.size = stat.st_size;
  status.block_size = stat.st_blksize;
  status.block_count = stat.st_blocks;
}

void me::filesystem::enumerate_entries(const path_type &path, size_t &count, directory_entry* entries, uint8_t options)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  ::DIR* dir = ::opendir(temp);
  if (!dir)
    throw exception("me::filesystem::enumerate_entries() -> path[%s] %s", temp, ::strerror(errno));

  count = 0;
  while (true)
  {
    ::dirent* dirent = ::readdir(dir);
    if (!dirent)
    {
      if (errno != 0)
	throw exception("me::filesystem::enumerate_entries() -> path[%s] %s", temp, ::strerror(errno));
      break;
    }

    bool junk = strcmp(".", dirent->d_name) || strcmp("..", dirent->d_name);
    if (!(options & ENUMERATE_ENTRIES_JUNK) && junk)
      continue;

    if (options & ENUMERATE_ENTRIES_IGNORE_HIDDEN_FILES && _ME_FILESYSTEM_IS_HIDDEN_FILE(dirent->d_name))
      continue;

    if (!junk && options & ENUMERATE_ENTRIES_SUB_DIRECTORIES && dirent->d_type == DT_DIR)
    {
      enumerate_entries(path, count, entries, options);
      continue;
    }

    if (entries != nullptr)
    {
      directory_entry &_entry = entries[count];
      _entry.type = _FILE_TYPE_DIRENT[dirent->d_type];
      ::memcpy(_entry.name, dirent->d_name, 256);
    }
    count++;
  }
}

void me::filesystem::change_extension(const path_type &path, char* str)
{
}

void me::filesystem::change_name(const path_type &path, char* str)
{
}

void me::filesystem::extension(const path_type &path, char* str)
{
}

void me::filesystem::directory(const path_type &path, char* str)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  const char* iter = temp;
  const char* last = temp;
  while (*iter != '\0') /* TODO: reverse faster? */
  {
    if (*iter == _ME_FILESYSTEM_PATH_SEPARATOR)
      last = iter;
    iter++;
  }

  size_t len = last - temp;
  if (len == 0 && path[0] == _ME_FILESYSTEM_PATH_SEPARATOR)
    len = 1;

  if (str != nullptr)
  {
    for (size_t i = 0; i < len; i++)
      str[i] = path[i];
  }
  str[len] = '\0';
}

void me::filesystem::name(const path_type &path, char* str)
{
}

void me::filesystem::absolute(const path_type &path, char* str)
{
  char _temp[path.size() + 1];
  path.c_str(_temp);

  ::realpath(_temp, str);
}

void me::filesystem::canonical(const path_type &path, char* str)
{
}

void me::filesystem::relative(const path_type &path, char* str)
{
}

void me::filesystem::move(const path_type &src, const path_type &dest, size_t buff_size, uint8_t options)
{
}

void me::filesystem::copy(const path_type &src, const path_type &dest, size_t buff_size, uint8_t options)
{
  char src_temp[src.size() + 1];
  src.c_str(src_temp);
  char dest_temp[dest.size() + 1];
  dest.c_str(dest_temp);

  ::FILE* src_file = ::fopen(src_temp, "rb");
  if (!src_file)
    throw exception("me::filesystem::copy() -> (source) path[%s] %s", src_temp, ::strerror(errno));

  ::FILE* dest_file = ::fopen(dest_temp, "ab");
  if (!dest_file)
    throw exception("me::filesystem::copy() -> (destination) path[%s] %s", dest_temp, ::strerror(errno));

  ::fseek(src_file, 0, SEEK_END);
  size_t len = ::ftell(src_file);
  ::rewind(src_file);

  size_t pos = 0;
  char buffer[buff_size];
  while (pos != len)
  {
    size_t buffer_size = MIN(buff_size, len - pos++);
    ::fread(buffer, buffer_size, 1, src_file);
    ::fwrite(buffer, buffer_size, 1, dest_file);
  }

  ::fclose(src_file);
  ::fclose(dest_file);
}

void me::filesystem::remove(const path_type &path)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  if (::remove(temp) != 0)
    throw exception("me::filesystem::remove() -> path[%s] %s", temp, ::strerror(errno));
}

void me::filesystem::read(const path_type &path, size_t len, void* data)
{
  char _temp[path.size() + 1];
  path.c_str(_temp);

  ::FILE* file = ::fopen(_temp, "rb");
  if (!file)
    throw exception("me::filesystem::read() -> path[%s] %s", _temp, ::strerror(errno));

  /* read file */
  ::fread(data, 1, len, file);
  ::fclose(file);
}

void me::filesystem::write(const path_type &path, size_t len, void* data)
{
  char temp[path.size() + 1];
  path.c_str(temp);

  ::FILE* file = ::fopen(temp, "wb");
  if (!file)
    throw exception("me::filesystem::write() -> path[%s] %s", temp, ::strerror(errno));

  /* write file */
  ::fwrite(data, 1, len, file);
  ::fclose(file);
}
