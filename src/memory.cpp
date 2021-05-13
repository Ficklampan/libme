#include "allocator.hpp"

#include <stdlib.h>

static me::allocator _alloc;

me::allocator* me::allocator::_default()
{
  return &_alloc;
}
