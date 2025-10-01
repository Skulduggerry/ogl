#include "my_lib/my_lib.hpp"
#include <fmt/base.h>

int some_fun()
{
  fmt::println("Hello {}!", "World");
  return 0;
}
