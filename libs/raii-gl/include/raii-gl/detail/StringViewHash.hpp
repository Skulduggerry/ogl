#ifndef OGL_STRINGVIEWHASH_HPP
#define OGL_STRINGVIEWHASH_HPP

#include <string>
#include <string_view>

struct SvHash
{
  using is_transparent = void;
  std::size_t operator()(const std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
  std::size_t operator()(const std::string &s) const noexcept { return std::hash<std::string>{}(s); }
};
struct SvEq
{
  using is_transparent = void;
  bool operator()(const std::string_view a, const std::string_view b) const noexcept { return a == b; }
  bool operator()(const std::string &a, const std::string_view b) const noexcept { return a == b; }
  bool operator()(const std::string_view a, const std::string &b) const noexcept { return a == b; }
  bool operator()(const std::string &a, const std::string &b) const noexcept { return a == b; }
};

#endif
