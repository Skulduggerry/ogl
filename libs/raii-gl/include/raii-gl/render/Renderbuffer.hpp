#ifndef OGL_RENDERBUFFER_HPP
#define OGL_RENDERBUFFER_HPP

#include "raii-gl/ImageFormat.hpp"

#include <string_view>

class Renderbuffer
{
  GLuint m_id;

  InternalImageFormat m_internalFormat = InternalImageFormat::RGBA8;
  GLsizei m_width = 0;
  GLsizei m_height = 0;
  bool m_allocated = false;

public:
  Renderbuffer();
  Renderbuffer(InternalImageFormat internalFormat, GLsizei width, GLsizei height);
  ~Renderbuffer();
  Renderbuffer(const Renderbuffer &other) = delete;
  Renderbuffer(Renderbuffer &&other) noexcept;

  Renderbuffer &operator=(const Renderbuffer &other) = delete;
  Renderbuffer &operator=(Renderbuffer &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }
  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }
  [[nodiscard]] bool isAllocated() const noexcept { return m_allocated; }
  [[nodiscard]] InternalImageFormat getInternalFormat() const { return m_internalFormat; }
  [[nodiscard]] GLsizei getWidth() const { return m_width; }
  [[nodiscard]] GLsizei getHeight() const { return m_height; }

  void storage(InternalImageFormat internalFormat, GLsizei width, GLsizei height);

  void debugLabel(std::string_view name) const;
};

#endif
