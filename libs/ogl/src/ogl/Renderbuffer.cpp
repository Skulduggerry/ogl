#include "ogl/Renderbuffer.hpp"
#include "ogl/Logging.hpp"

#include <utility>

static GLuint createRenderbuffer()
{
  GLuint id = 0;
  GLCall(glCreateRenderbuffers(1, &id));
  return id;
}

Renderbuffer::Renderbuffer() : m_id(createRenderbuffer()) {}

Renderbuffer::Renderbuffer(const InternalImageFormat internalFormat, const GLsizei width, const GLsizei height)
  : Renderbuffer{}
{ storage(internalFormat, width, height); }

Renderbuffer::~Renderbuffer()
{
  if (hasName()) { GLCall(glDeleteRenderbuffers(1, &m_id)); }
}

Renderbuffer::Renderbuffer(Renderbuffer &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_internalFormat(other.m_internalFormat), m_width(other.m_width),
    m_height(other.m_height)
{}

Renderbuffer &Renderbuffer::operator=(Renderbuffer &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteRenderbuffers(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);
  m_internalFormat = other.m_internalFormat;
  m_width = std::exchange(other.m_width, 0);
  m_height = std::exchange(other.m_height, 0);

  return *this;
}

void Renderbuffer::storage(const InternalImageFormat internalFormat, const GLsizei width, const GLsizei height)
{
  ASSERT(width > 0 && height > 0);

  m_internalFormat = internalFormat;
  m_width = width;
  m_height = height;
  m_allocated = true;

  GLCall(glNamedRenderbufferStorage(m_id, static_cast<GLenum>(m_internalFormat), m_width, m_height));
}

void Renderbuffer::debugLabel(const std::string_view name) const
{
  GLCall(glObjectLabel(GL_RENDERER, m_id, static_cast<GLsizei>(name.size()), name.data()));
}
