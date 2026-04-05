#ifndef OGL_FRAMEBUFFER_HPP
#define OGL_FRAMEBUFFER_HPP

#include "Renderbuffer.hpp"
#include "raii-gl/texture/TextureAttachment.hpp"

#include <glad/glad.h>
#include <span>

enum struct Attachment : GLenum {
  NONE = GL_NONE,
  COLOR_0 = GL_COLOR_ATTACHMENT0,
  COLOR_1 = GL_COLOR_ATTACHMENT1,
  COLOR_2 = GL_COLOR_ATTACHMENT2,
  COLOR_3 = GL_COLOR_ATTACHMENT3,
  DEPTH = GL_DEPTH_ATTACHMENT,
  STENCIL = GL_STENCIL_ATTACHMENT,
  DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
};

enum struct FramebufferTarget : GLenum {
  DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
  READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER,
  FRAMEBUFFER = GL_FRAMEBUFFER,
};

enum struct FramebufferStatus : GLenum {
  FRAMEBUFFER_COMPLETE = GL_FRAMEBUFFER_COMPLETE,
  FRAMEBUFFER_UNDEFINED = GL_FRAMEBUFFER_UNDEFINED,
  FRAMEBUFFER_INCOMPLETE_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
  FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
  FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
  FRAMEBUFFER_INCOMPLETE_READ_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
  FRAMEBUFFER_UNSUPPORTED = GL_FRAMEBUFFER_UNSUPPORTED,
  FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
  FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
};

class Framebuffer
{
  GLuint m_id;

public:
  Framebuffer();
  ~Framebuffer();
  Framebuffer(const Framebuffer &other) = delete;
  Framebuffer(Framebuffer &&other) noexcept;
  Framebuffer &operator=(const Framebuffer &other) = delete;
  Framebuffer &operator=(Framebuffer &&other) noexcept;

  [[nodiscard]] FramebufferStatus getStatus(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const;
  [[nodiscard]] bool isComplete(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const;
  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }

  void drawBuffer(Attachment buffer) const;
  void drawBuffers(std::span<const Attachment> buffers) const;
  void readBuffer(Attachment buffer) const;

  void attach(const Renderbuffer &renderbuffer, Attachment attachment) const;
  void attach(const TextureLevelAttachment &info, Attachment attachment) const;
  void attach(const TextureLayerAttachment &info, Attachment attachment) const;
  void detach(Attachment attachment) const;

  void bind(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER) const;
  static void bindDefault(FramebufferTarget target = FramebufferTarget::FRAMEBUFFER);

  void debugLabel(std::string_view name) const;
};

#endif
