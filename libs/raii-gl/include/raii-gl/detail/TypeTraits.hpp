#ifndef OGL_TYPETRAITS_HPP
#define OGL_TYPETRAITS_HPP

#include <type_traits>

template<typename T>
concept GpuValue =
  std::is_trivially_default_constructible_v<std::remove_const_t<T>>
  && std::is_trivially_copyable_v<std::remove_const_t<T>> && std::is_standard_layout_v<std::remove_const_t<T>>;

#endif
