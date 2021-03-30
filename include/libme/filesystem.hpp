#ifndef LIBME_FILESYSTEM_HPP
  #define LIBME_FILESYSTEM_HPP

#ifndef _ME_FILESYSTEM_PATH_SEPARATOR
  #if (defined _WIN32 || defined _WIN64)
    #define _ME_FILESYSTEM_PATH_SEPARATOR '\\'
  #else
    #define _ME_FILESYSTEM_PATH_SEPARATOR '/'
  #endif
#endif

#include "type.hpp"

#include <limits.h>

namespace me::filesystem {

  typedef uint64_t _device_type;
  typedef uint32_t _mode_type;
  typedef uint32_t _user_type;
  typedef uint32_t _group_type;
  typedef uint64_t _size_type;
  typedef uint64_t _block_size_type;
  typedef uint64_t _block_count_type;
  typedef uint64_t _time_type;

  enum file_type : uint8_t {
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_CHARACTER,
    FILE_TYPE_BLOCK,
    FILE_TYPE_REGULAR,
    FILE_TYPE_FIFO,
    FILE_TYPE_SYMBOLIC_LINK,
    FILE_TYPE_SOCKET,
    FILE_TYPE_UNKNOWN
  };

  enum permissions : uint8_t {
    FILE_PERMISSION_NONE = 0x00,
    FILE_PERMISSION_READ = 0x01,
    FILE_PERMISSION_WRITE = 0x02,
    FILE_PERMISSION_EXECUTE = 0x04
  };

  enum enumerate_entries_options {
    ENUMERATE_ENTRIES_SUB_DIRECTORIES = 0x01,
    ENUMERATE_ENTRIES_IGNORE_HIDDEN_FILES = 0x02,
    ENUMERATE_ENTRIES_JUNK = 0x04
  };

  enum copy_options : uint8_t {
    COPY_OPTION_FORCE = 0x01,
    COPY_OPTION_RECURSIVE = 0x02,
    COPY_OPTION_ONE_FILE_SYSTEM = 0x04
  };

  enum move_options : uint8_t {
    MOVE_OPTION_FORCE = 0x01,
    MOVE_OPTION_ONE_FILE_SYSTEM = 0x04
  };


  struct file_status {

    _device_type device;
    _mode_type mode;
    _user_type user_id;
    _group_type group_id;
    _device_type device_id;
    _size_type size;
    _block_size_type block_size;
    _block_count_type block_count;

  };

  struct directory_entry {

    file_type type;
    char name[PATH_MAX];

  };

  /* C string */
  bool exists(const char* _path);

  void create_file(const char* _path);
  void make_directory(const char* _path, uint16_t _mode = 0);
  void make_directories(const char* _path);

  void status(const char* _path, file_status &_status);
  void enumerate_entries(const char *_path, size_t &_count, directory_entry* _entries, uint8_t _options = 0);

  void change_extension(const char* _path, char* _str);
  void change_name(const char* _path, char* _str);

  void extension(const char* _path, char* _str);
  void directory(const char* _path, char* _str);
  void name(const char* _path, char* _str);

  void absolute(const char* _path, char* _str);
  void canonical(const char* _path, char* _str);
  void relative(const char* _path, char* _str);

  void move(const char* _src, const char* _dest, size_t _buff_size = 4096, uint8_t _options = 0);
  void copy(const char*_src, const char* _dest, size_t _buff_size = 4096, uint8_t _options = 0);
  void remove(const char* _path);

  void read(const char* _path, size_t _len, void* _data);
  void write(const char* _path, size_t _len, void* _data);

}

#endif
