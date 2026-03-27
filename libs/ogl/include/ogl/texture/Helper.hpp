#ifndef OGL_HELPER_HPP
#define OGL_HELPER_HPP

#include <algorithm>
#include <cmath>
#include <glad/glad.h>

constexpr GLsizei calculateMipLevels(const GLsizei width, const GLsizei height)
{
  ASSERT(width > 0 && height > 0);
  return static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;
}

#endif
