#ifndef OGL_FRAMEBUFFER_HPP
#define OGL_FRAMEBUFFER_HPP
#include "Logging.hpp"
#include "Renderbuffer.hpp"
#include "texture/Texture2D.hpp"
#include "texture/Texture2DArray.hpp"

#include <glad/glad.h>
#include <ranges>

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

class FramebufferBase
{
protected:
  GLuint m_id;

  // don't allow construction of this base class
  explicit FramebufferBase(GLuint id);

public:
  // Base class needs virtual destructor
  virtual ~FramebufferBase() = default;

  // prevent slicing and direct instantiation
  FramebufferBase(const FramebufferBase &other) = delete;
  FramebufferBase(FramebufferBase &&other) = delete;
  FramebufferBase &operator=(const FramebufferBase &other) = delete;
  FramebufferBase &operator=(FramebufferBase &&other) = delete;

  // bind the framebuffer to the GL_FRAMEBUFFER target
  void bind() const;
};

class DefaultFramebuffer final : public FramebufferBase
{
  // don't allow creation of this class
  DefaultFramebuffer();
  ~DefaultFramebuffer() override = default;

public:
  static DefaultFramebuffer &getInstance();
};

class Framebuffer final : public FramebufferBase
{
public:
  Framebuffer();
  ~Framebuffer() override;
  Framebuffer(const Framebuffer &other) = delete;
  Framebuffer(Framebuffer &&other) noexcept;

  Framebuffer &operator=(const Framebuffer &other) = delete;
  Framebuffer &operator=(Framebuffer &&other) noexcept;

  void drawBuffer(Attachment buffer) const;
  template<std::ranges::random_access_range R> void drawBuffers(const R &buffer) const
  {
    using namespace std::ranges;
    GLCall(glNamedFramebufferDrawBuffers(m_id, size(buffer), reinterpret_cast<const GLenum *>(data(buffer))));
  }

  void readBuffer(Attachment buffer) const;

  void attach(const Renderbuffer &renderbuffer, Attachment attachment) const;

  void attach(const Texture2D &texture, Attachment attachment, GLint level = 0) const;

  void attach(const Texture2DArray &texture, Attachment attachment, GLint level = 0) const;

  [[nodiscard]] bool isComplete() const;
};

#endif
