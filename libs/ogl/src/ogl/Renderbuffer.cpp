#include "ogl/Renderbuffer.hpp"
#include "ogl/Logging.hpp"

#include <utility>

GLuint createRenderbuffer()
{
  GLuint id = 0;
  GLCall(glCreateRenderbuffers(1, &id));
  return id;
}

Renderbuffer::Renderbuffer() : m_id(createRenderbuffer()) {}

Renderbuffer::Renderbuffer(const InternalFormat format, const GLsizei width, const GLsizei height) : Renderbuffer{}
{
  storage(format, width, height);
}

Renderbuffer::~Renderbuffer() { GLCall(glDeleteRenderbuffers(1, &m_id)); }

Renderbuffer::Renderbuffer(Renderbuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Renderbuffer &Renderbuffer::operator=(Renderbuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void Renderbuffer::storage(const InternalFormat format, const GLsizei width, const GLsizei height)
{
  m_format = format;
  m_width = width;
  m_height = height;

  GLCall(glNamedRenderbufferStorage(m_id, static_cast<GLenum>(m_format), m_width, m_height));
}
