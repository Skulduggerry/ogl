#ifndef OGL_HELPER_HPP
#define OGL_HELPER_HPP

#include <bit>

constexpr int calculateMipLevels(const int width, const int height)
{
  ASSERT(width > 0 && height > 0);
  const auto max = static_cast<unsigned>(width > height ? width : height);
  return std::bit_width(max);
}

#endif
