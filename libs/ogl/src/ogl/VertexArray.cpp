#include "ogl/VertexArray.hpp"
#include "ogl/Logging.hpp"
#include "ogl/VertexBuffer.hpp"
#include <utility>

GLuint createVertexArrayId()
{
  GLuint id = 0;
  GLCall(glCreateVertexArrays(1, &id));
  return id;
}

VertexArray::VertexArray() : m_id(createVertexArrayId()) {}

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_id)); }

VertexArray::VertexArray(VertexArray &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void VertexArray::bind() const { GLCall(glBindVertexArray(m_id)); }

void VertexArray::unbind() { GLCall(glBindVertexArray(0)); }

void VertexArray::vertexBuffer(const GLuint bindingIndex,
  const VertexBuffer &buffer,
  const VertexBufferLayout &layout,
  const GLintptr offset) const
{
  GLCall(glVertexArrayVertexBuffer(m_id, bindingIndex, buffer.m_id, offset, layout.m_totalOffset));

  for (auto &[attribIndex, count, type, normalized, relativeOffset] : layout.m_floatAttribs) {
    GLCall(glEnableVertexArrayAttrib(m_id, attribIndex));
    GLCall(glVertexArrayAttribFormat(
      m_id, attribIndex, static_cast<GLint>(count), static_cast<GLenum>(type), normalized, relativeOffset));
    GLCall(glVertexArrayAttribBinding(m_id, attribIndex, bindingIndex));
  }

  for (auto &[attribIndex, count, type, relativeOffset] : layout.m_intAttribs) {
    GLCall(glEnableVertexArrayAttrib(m_id, attribIndex));
    GLCall(glVertexArrayAttribIFormat(
      m_id, attribIndex, static_cast<GLint>(count), static_cast<GLenum>(type), relativeOffset));
    GLCall(glVertexArrayAttribBinding(m_id, attribIndex, bindingIndex));
  }

  for (auto &[attribIndex, count, relativeOffset] : layout.m_doubleAttribs) {
    GLCall(glEnableVertexArrayAttrib(m_id, attribIndex));
    GLCall(glVertexArrayAttribLFormat(
      m_id, attribIndex, static_cast<GLint>(count), static_cast<GLenum>(BufferDataType::DOUBLE), relativeOffset));
    GLCall(glVertexArrayAttribBinding(m_id, attribIndex, bindingIndex));
  }
}