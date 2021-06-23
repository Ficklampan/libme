#include "../../include/libme/format.hpp"
#include <stdio.h>

//#define NO_TIMER

#ifndef NO_TIMER

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

int test_printf(const char* s, ...)
{
  va_list args;
  va_start(args, s);
  char temp[1024];
  vsprintf(temp, s, args);
  va_end(args);
  return 0;
}

template<typename... A>
int test_format(const char* s, const A&... args)
{
  char temp[1024];
  me::format::print(temp, s, args...);
  return 0;
}

me::uint64_t get_time()
{
  struct timeval val;
  gettimeofday(&val, nullptr);
  return val.tv_usec;
}

#endif

inline int test()
{
#ifndef NO_TIMER
  const char* printf_str = "Hej jag heter %s och är bättre än %s! %s %s %s %s";
  const char* format_str = "Hej jag heter {} och är bättre än {}! {} {} {} {}";

  int total_tests = 1024;
  int tests;
  me::uint64_t start;

  tests = 0;
  start = get_time();
  while (tests++ < total_tests)
    test_printf(printf_str, "Edvin", "dig", "Hejsan", "hoppsan", "blablabla", "bla");
  unsigned long printf_result = get_time() - start;

  tests = 0;
  start = get_time();
  while (tests++ < total_tests)
    test_format(format_str, "Edvin", "dig", "Hejsan", "hoppsan", "blablabla", "bla");
  me::uint64_t format_result = get_time() - start;

  long double amount = static_cast<long double>(printf_result) / static_cast<long double>(format_result);

  printf("printf: %lu, format: %lu\n", printf_result, format_result);
  printf("format is %Lf%% faster than printf\n", amount);

#endif
  return 0;
}

int main(int , char** )
{
  test();

  char buffer[1024];
  me::format::print(buffer, "Hello {} and hello {}! :D", "Edvin", "Vilmer");
  ::printf("string: [%s]\n", buffer);
  return 0;
}

//int main(int , char** )
//{
//  char buffer[1024];
//  me::format::print(buffer, "Hello {} and hello {}! :D", "Edvin", "Vilmer");
//  ::printf("string: [%s]\n", buffer);
//  return 0;
//}
