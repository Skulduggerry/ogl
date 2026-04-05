#include "ogl/VertexArray.hpp"
#include "ogl/Logging.hpp"
#include <utility>

GLuint createVertexArray()
{
  GLuint id = 0;
  GLCall(glCreateVertexArrays(1, &id));
  return id;
}

VertexArray::VertexArray() : m_id(createVertexArray()) {}

VertexArray::~VertexArray()
{
  if (hasName()) { GLCall(glDeleteVertexArrays(1, &m_id)); }
}

VertexArray::VertexArray(VertexArray &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteVertexArrays(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);

  return *this;
}

void VertexArray::bind() const { GLCall(glBindVertexArray(m_id)); }

void VertexArray::unbind() { GLCall(glBindVertexArray(0)); }