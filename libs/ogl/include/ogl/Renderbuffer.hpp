#ifndef OGL_RENDERBUFFER_HPP
#define OGL_RENDERBUFFER_HPP
#include <glad/glad.h>
#include "InternalFormats.hpp"

class Framebuffer;

// TODO: put into external file
enum struct StorageFormats: GLenum {
  DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
  DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
};

class Renderbuffer
{
  GLuint m_id = 0;

  StorageFormats m_format = StorageFormats::DEPTH_COMPONENT32F;
  GLsizei m_width = 0;
  GLsizei m_height = 0;

public:
  Renderbuffer();
  Renderbuffer(StorageFormats format, GLsizei width, GLsizei height);
  ~Renderbuffer();
  Renderbuffer(const Renderbuffer &other) = delete;
  Renderbuffer(Renderbuffer &&other) noexcept;

  Renderbuffer &operator=(const Renderbuffer &other) = delete;
  Renderbuffer &operator=(Renderbuffer &&other) noexcept;

  void storage(StorageFormats format, GLsizei width, GLsizei height);

  [[nodiscard]] StorageFormats getFormat() const { return m_format; }
  [[nodiscard]] GLsizei getWidth() const { return m_width; }
  [[nodiscard]] GLsizei getHeight() const { return m_height; }

  friend class Framebuffer;
};

#endif
