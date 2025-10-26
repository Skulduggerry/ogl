#ifndef OGL_FRAMEBUFFER_HPP
#define OGL_FRAMEBUFFER_HPP
#include "Logging.hpp"

#include <array>
#include <glad/glad.h>
#include <ranges>

enum struct ColorBuffer : GLenum {
  NONE = GL_NONE,
  COLOR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0,
  COLOR_ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
  COLOR_ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
  COLOR_ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,
};

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

  void drawBuffer(ColorBuffer buffer) const;
  template<std::ranges::random_access_range R> void drawBuffers(const R &buffer) const
  {
    using namespace std::ranges;
    GLCall(glNamedFramebufferDrawBuffers(m_id, size(buffer), reinterpret_cast<const GLenum *>(data(buffer))));
  }

  [[nodiscard]] bool isComplete() const;
};

#endif
