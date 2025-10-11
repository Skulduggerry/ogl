#include "ogl/VertexArray.hpp"
#include "ogl/Logging.hpp"
#include "ogl/VertexBuffer.hpp"
#include <utility>

GLuint genVertexArrayId()
{
  GLuint id = 0;
  GLCall(glCreateVertexArrays(1, &id));
  return id;
}

VertexArray::VertexArray() : m_id(genVertexArrayId()) {}

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_id)); }

VertexArray::VertexArray(VertexArray &&other) noexcept : m_id(other.m_id) {}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void VertexArray::vertexBuffer(const GLuint bindingIndex,
  const VertexBuffer &buffer,
  const GLintptr offset,
  const GLsizei stride) const
{
  GLCall(glVertexArrayVertexBuffer(m_id, bindingIndex, buffer.m_id, offset, stride));
}

VertexArray &VertexArray::enableAttrib(const GLuint index)
{
  GLCall(glEnableVertexArrayAttrib(m_id, index));
  return *this;
}

VertexArray &VertexArray::attribFormat(const GLuint attribIndex,
  const GLint size,
  const GLenum type,
  const GLboolean normalized,
  const GLuint relativeOffset)
{
  GLCall(glVertexArrayAttribFormat(m_id, attribIndex, size, type, normalized, relativeOffset));
  return *this;
}

VertexArray &
  VertexArray::attribIFormat(const GLuint attribIndex, const GLint size, const GLenum type, const GLuint relativeOffset)
{
  GLCall(glVertexArrayAttribIFormat(m_id, attribIndex, size, type, relativeOffset));
  return *this;
}

VertexArray &
  VertexArray::attribLFormat(const GLuint attribIndex, const GLint size, const GLenum type, const GLuint relativeOffset)
{
  GLCall(glVertexArrayAttribLFormat(m_id, attribIndex, size, type, relativeOffset));
  return *this;
}

void VertexArray::attribBinding(const GLuint attribIndex, const GLuint bindingIndex) const
{
  GLCall(glVertexArrayAttribBinding(m_id, attribIndex, bindingIndex));
}
