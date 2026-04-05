#ifndef OGL_HELPER_HPP
#define OGL_HELPER_HPP

constexpr int calculateMipLevels(const int width, const int height)
{
  const auto max = static_cast<unsigned>(width > height ? width : height);
  return std::bit_width(max);
}

#endif
