#ifndef OGL_RENDERBUFFER_HPP
#define OGL_RENDERBUFFER_HPP
#include "ImageFormat.hpp"

#include <glad/glad.h>

class Renderbuffer
{
  GLuint m_id;

  InternalImageFormat m_format = InternalImageFormat::DEPTH_COMPONENT32F;
  GLsizei m_width = 0;
  GLsizei m_height = 0;

public:
  Renderbuffer();
  Renderbuffer(InternalImageFormat format, GLsizei width, GLsizei height);
  ~Renderbuffer();
  Renderbuffer(const Renderbuffer &other) = delete;
  Renderbuffer(Renderbuffer &&other) noexcept;

  Renderbuffer &operator=(const Renderbuffer &other) = delete;
  Renderbuffer &operator=(Renderbuffer &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }

  void storage(InternalImageFormat format, GLsizei width, GLsizei height);

  [[nodiscard]] InternalImageFormat getFormat() const { return m_format; }
  [[nodiscard]] GLsizei getWidth() const { return m_width; }
  [[nodiscard]] GLsizei getHeight() const { return m_height; }
};

#endif
