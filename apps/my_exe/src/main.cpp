#include "my_lib/my_lib.hpp"
#include <fmt/base.h>

int main(const int argc, [[maybe_unused]] char **argv)
{
  auto some_num = some_fun();
  fmt::println("some_num is: {}", some_num);

  fmt::println("argc is: {}", argc);

  const int array[1] = { -1 };
  int i = array[argc];
  fmt::println("at index argc: {}", i);
}