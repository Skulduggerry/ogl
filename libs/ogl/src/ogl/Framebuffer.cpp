#include "ogl/Framebuffer.hpp"
#include "ogl/Logging.hpp"

#include <utility>

GLuint createFramebuffer()
{
  GLuint id = 0;
  GLCall(glCreateFramebuffers(1, &id));
  return id;
}

Framebuffer::Framebuffer() : m_id(createFramebuffer()) {}

Framebuffer::~Framebuffer() { GLCall(glDeleteFramebuffers(1, &m_id)); }

Framebuffer::Framebuffer(Framebuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void Framebuffer::bind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id)); }

void Framebuffer::unbind() { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

void Framebuffer::drawBuffer(ColorBuffer buffer) const
{
  GLCall(glNamedFramebufferDrawBuffer(m_id, static_cast<GLenum>(buffer)));
}

void Framebuffer::renderbuffer(const Renderbuffer &renderbuffer) const
{
  GLenum attachment;
  switch (renderbuffer.getFormat()) {
  case StorageFormats::DEPTH_COMPONENT32F:
    attachment = GL_DEPTH_COMPONENT;
    break;
  case StorageFormats::DEPTH24_STENCIL8:
    attachment = GL_DEPTH_STENCIL_ATTACHMENT;
    break;
  }

  GLCall(glNamedFramebufferRenderbuffer(m_id, attachment, GL_RENDERBUFFER, renderbuffer.m_id));
}


bool Framebuffer::isComplete() const
{
  return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
