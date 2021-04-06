#include "../../include/libme/format.hpp"

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
int test_format(const char* s, A&&... args)
{
  char temp[1024];
  me::format(temp, s, static_cast<A&&>(args)...);
  return 0;
}

unsigned long get_ms()
{
  struct timeval val;
  gettimeofday(&val, nullptr);
  return val.tv_usec / 1000L;
}

int test()
{
  const char* printf_str = "Hej jag heter %s och är bättre än %s! %s %s %s %s";
  const char* format_str = "Hej jag heter {} och är bättre än {}! {} {} {} {}";

  int total_tests = 1024 * 1024;
  int tests;
  unsigned long start, result;

  tests = 0;
  start = get_ms();
  while (tests++ < total_tests)
    test_printf(printf_str, "Edvin", "dig", "Hejsan", "hoppsan", "blablabla", "bla");
  result = get_ms() - start;
  printf("printf took %lu ms\n", result);

  tests = 0;
  start = get_ms();
  while (tests++ < total_tests)
    test_format(format_str, "Edvin", "dig", "Hejsan", "hoppsan", "blablabla", "bla");
  result = get_ms() - start;
  printf("format took %lu ms\n", result);

  int l = 15;
  if (result < 200)
    l++;
  char temp[1024];
  me::format(temp, "Jag heter {1} och är {0} år. {2} {3#x}", l, "Edvin", 1.75F, 0xFF);
  ::printf("[%s]\n", temp);
  return 0;
}

int main(int , char** )
{
  try {
    test();
  }catch (const me::exception &e)
  {
    ::printf("error: %s\n", e.get_message());
  }
  return 0;
}
