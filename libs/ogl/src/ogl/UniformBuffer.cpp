#include "ogl/UniformBuffer.hpp"

GLuint createUniformBufferId()
{
  GLuint id = 0;
  GLCall(glCreateBuffers(1, &id));
  return id;
}

UniformBuffer::UniformBuffer() : m_id(createUniformBufferId()) {}

UniformBuffer::~UniformBuffer() { GLCall(glDeleteBuffers(1, &m_id)); }

UniformBuffer::UniformBuffer(UniformBuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

UniformBuffer &UniformBuffer::operator=(UniformBuffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}

void UniformBuffer::bufferData(const GLsizeiptr size, const BufferUsage usage) const
{ GLCall(glNamedBufferData(m_id, size, nullptr, static_cast<GLenum>(usage))); }

void UniformBuffer::bind(const GLuint bindingPoint) const
{ GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_id)); }

void UniformBuffer::bind(const GLuint bindingPoint, const GLintptr offset, const GLsizeiptr size) const
{ glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_id, offset, size); }