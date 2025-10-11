#include "ogl/IndexBuffer.hpp"
#include "ogl/Logging.hpp"
#include <utility>

GLuint genIndexBuffer()
{
  GLuint id = 0;
  GLCall(glCreateBuffers(1, &id));
  return id;
}

IndexBuffer::IndexBuffer() : m_id(genIndexBuffer()) {}

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_id)); }

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id)); }
