#ifndef OGL_FRAMEBUFFER_HPP
#define OGL_FRAMEBUFFER_HPP
#include <glad/glad.h>

class Framebuffer
{
  GLuint m_id = 0;

public:
  Framebuffer();
  ~Framebuffer();
  Framebuffer(const Framebuffer &other) = delete;
  Framebuffer(Framebuffer &&other) noexcept;

  Framebuffer &operator=(const Framebuffer &other) = delete;
  Framebuffer &operator=(Framebuffer &&other) noexcept;

  void bind() const;
  static void unbind();
};

#endif
