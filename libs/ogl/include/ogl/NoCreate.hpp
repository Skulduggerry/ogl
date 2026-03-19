#ifndef OGL_NOCREATE_HPP
#define OGL_NOCREATE_HPP

struct NoCreate_t
{
  explicit NoCreate_t() noexcept = default;
};

inline constexpr NoCreate_t NoCreate{};

#endif// OGL_NOCREATE_HPP
