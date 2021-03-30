#include <stdio.h>
#include <unistd.h>

#include "string.hpp"

int main()
{
  me::string str = " ";
  size_t length;
  str.split(' ', length, nullptr);
  me::string_view* split[length];
  str.split(' ', length, split);

  for (size_t i = 0; i < length; i++)
  {
    char s[split[i]->size() + 1];
    printf("[%s]\n", split[i]->c_str(s));
  }
  return 0;
}
