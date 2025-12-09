#include "ogl/IndexBuffer.hpp"
#include "ogl/Logging.hpp"
#include <utility>

GLuint createIndexBuffer()
{
  GLuint id = 0;
  GLCall(glCreateBuffers(1, &id));
  return id;
}

IndexBuffer::IndexBuffer() : m_id(createIndexBuffer()) {}

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_id)); }

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)), m_type(other.m_type) {}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  m_type = other.m_type;
  return *this;
}

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id)); }
