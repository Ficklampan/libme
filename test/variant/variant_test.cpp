#include "../../include/libme/variant.hpp"

#include <stdio.h>

int main(int argc, char** argv)
{
  typedef me::variant<int, float, double, long> Type;
  Type test(83);

  int i = test.get<int>();

  printf("i: %i\n", i);
  return 0;
}
