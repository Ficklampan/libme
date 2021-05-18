#ifndef LIBME_OPTIONS_HPP
  #define LIBME_OPTIONS_HPP

#include "string.hpp"

namespace me {

  enum option_value_type {
    OPTION_VALUE_NONE = 1,
    OPTION_VALUE_REQUIRED = 2,
    OPTION_VALUE_OPTIONAL = 0,
  };


  struct option_info {
    const int identifier;
    const string_view long_name;
    const char short_name;
    const option_value_type value;
  };

  struct option {
    const option_info* info;
    const string_view value1;
    const string_view value2;
  };

  /*
   * program --help
   * program --file=test.txt
   * program -h
   * program -f=test.txt
   * program -f test.txt
   * program -Dname=value
   */

  inline const option next_option(size_t option_count, const option_info options[], size_t argc, char** argv, size_t &index);
  inline const option_info* get_option(size_t option_count, const option_info options[], const char* &arg, string_view &name, bool &short_opt);
  inline const option_info* find_option(size_t option_count, const option_info options[], const string_view &long_name);
  inline const option_info* find_option(size_t option_count, const option_info options[], char short_name);

}

const me::option me::next_option(size_t option_count, const option_info options[], size_t argc, char** argv, size_t &index)
{
  const char* arg = argv[index++];

  if (arg[0] != '-')
    return {nullptr, arg, ""};
  arg++;

  string_view name;
  bool short_opt;
  const option_info* option = get_option(option_count, options, arg, name, short_opt);
  if (option == nullptr)
  {
    char temp[name.size() + 1];
    throw exception("invalid option: %s", name.c_str(temp));
  }

  if (*arg == '=')
  {
    if (option->value == OPTION_VALUE_NONE)
    {
      char temp[name.size() + 1];
      throw exception("value not allowed for option: %s", name.c_str(temp));
    }

    arg++;

    const char* val_begin = arg;
    while (*arg != '\0')
      arg++;
    const char* val_end = arg;

    return {option, string_view(val_begin, val_end), ""};

  }else if (short_opt)
  {
    if (*arg != '\0')
    {
      if (option->value == OPTION_VALUE_NONE)
      {
	char temp[name.size() + 1];
	throw exception("value not allowed for option: %s", name.c_str(temp));
      }

      const char* val_begin = arg;
      while (*arg != '=' && *arg != '\0')
        arg++;
      const char* val_end = arg;

      if (*arg == '=')
      {
	arg++;
	const char* val2_begin = arg;
      	while (*arg != '=' && *arg != '\0')
      	  arg++;
      	const char* val2_end = arg;

	return {option, string_view(val_begin, val_end), string_view(val2_begin, val2_end)};
      }

      return {option, string_view(val_begin, val_end), ""};
    }else if (index != argc)
    {
      return {option, string_view(argv[index++]), ""};
    }
  }

  if (option->value == OPTION_VALUE_REQUIRED)
  {
    char temp[option->long_name.size() + 1];
    throw exception("value required for option: %s", name.c_str(temp));
  }
  return {option, "", ""};
}

const me::option_info* me::get_option(size_t option_count, const option_info options[], const char* &arg, string_view &name, bool &short_opt)
{
  /* long option name */
  if (*arg == '-')
  {
    short_opt = false;

    arg++;

    const char* opt_begin = arg;
    while (*arg != '=' && *arg != '\0')
      arg++;
    const char* opt_end = arg;

    name = opt_begin;
    name = string_view(opt_begin, opt_end);
    return find_option(option_count, options, name);
  }

  /* short option name */
  short_opt = true;
  name = string_view(arg, 1);
  const char short_name = *arg++;
  return find_option(option_count, options, short_name);
}

const me::option_info* me::find_option(size_t option_count, const option_info options[], const string_view &long_name)
{
  for (size_t i = 0; i < option_count; i++)
  {
    if (options[i].long_name == long_name)
      return &options[i];
  }
  return nullptr;
}

const me::option_info* me::find_option(size_t option_count, const option_info options[], char short_name)
{
  for (size_t i = 0; i < option_count; i++)
  {
    if (options[i].short_name == short_name)
      return &options[i];
  }
  return nullptr;
}

#endif
