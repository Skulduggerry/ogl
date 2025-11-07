#ifndef OGL_FRAMEBUFFER_HPP
#define OGL_FRAMEBUFFER_HPP
#include "Logging.hpp"
#include "Renderbuffer.hpp"

#include <glad/glad.h>
#include <ranges>

enum struct Attachment : GLenum {
  NONE = GL_NONE,
  COLOR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0,
  COLOR_ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
  COLOR_ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
  COLOR_ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,
  DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
  STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
  DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT,
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

  void drawBuffer(Attachment buffer) const;
  template<std::ranges::random_access_range R> void drawBuffers(const R &buffer) const
  {
    using namespace std::ranges;
    GLCall(glNamedFramebufferDrawBuffers(m_id, size(buffer), reinterpret_cast<const GLenum *>(data(buffer))));
  }

  void attach(const Renderbuffer &renderbuffer, Attachment attachment) const;
  void detach(Attachment attachment) const;

  [[nodiscard]] bool isComplete() const;
};

#endif
