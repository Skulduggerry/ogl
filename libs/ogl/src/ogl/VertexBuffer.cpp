#include "ogl/VertexBuffer.hpp"
#include "ogl/Logging.hpp"
#include <utility>

GLuint createVertexBufferId()
{
  GLuint id = 0;
  GLCall(glCreateBuffers(1, &id));
  return id;
}

VertexBuffer::VertexBuffer() : m_id(createVertexBufferId()) {}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_id)); }

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}