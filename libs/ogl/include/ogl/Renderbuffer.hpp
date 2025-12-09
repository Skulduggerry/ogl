#ifndef OGL_RENDERBUFFER_HPP
#define OGL_RENDERBUFFER_HPP
#include "ImageFormats.hpp"
#include <glad/glad.h>

class Framebuffer;

class Renderbuffer
{
  GLuint m_id;

  InternalFormat m_format = InternalFormat::DEPTH_COMPONENT32F;
  GLsizei m_width = 0;
  GLsizei m_height = 0;

public:
  Renderbuffer();
  Renderbuffer(InternalFormat format, GLsizei width, GLsizei height);
  ~Renderbuffer();
  Renderbuffer(const Renderbuffer &other) = delete;
  Renderbuffer(Renderbuffer &&other) noexcept;

  Renderbuffer &operator=(const Renderbuffer &other) = delete;
  Renderbuffer &operator=(Renderbuffer &&other) noexcept;

  void storage(InternalFormat format, GLsizei width, GLsizei height);

  [[nodiscard]] InternalFormat getFormat() const { return m_format; }
  [[nodiscard]] GLsizei getWidth() const { return m_width; }
  [[nodiscard]] GLsizei getHeight() const { return m_height; }

  friend class Framebuffer;
};

#endif
