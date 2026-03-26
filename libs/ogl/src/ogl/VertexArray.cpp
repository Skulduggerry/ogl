#include "ogl/VertexArray.hpp"
#include "ogl/Logging.hpp"
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