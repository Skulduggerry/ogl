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

void Framebuffer::drawBuffer(Attachment buffer) const
{
  GLCall(glNamedFramebufferDrawBuffer(m_id, static_cast<GLenum>(buffer)));
}

void Framebuffer::attach(const Renderbuffer &renderbuffer, Attachment attachment) const
{
  GLCall(glNamedFramebufferRenderbuffer(m_id, static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.m_id));
}

void Framebuffer::detach(Attachment attachment) const
{
  glNamedFramebufferTexture(m_id, static_cast<GLenum>(attachment), 0, 0);
}


bool Framebuffer::isComplete() const
{
  return glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
