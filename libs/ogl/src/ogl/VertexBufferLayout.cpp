#include "ogl/VertexBufferLayout.hpp"

template<BufferDataType T> VertexBufferLayout &VertexBufferLayout::pushF(GLuint count, GLboolean normalized)
{
  m_floatAttribs.emplace_back(m_attribIndex, count, T, normalized, m_totalOffset);
  m_attribIndex++;
  m_totalOffset += count * getSizeOfType<T>();

  return *this;
}

template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::BYTE>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::UNSIGNED_BYTE>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::SHORT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::UNSIGNED_SHORT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::INT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::UNSIGNED_INT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::FIXED>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::FLOAT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::HALF_FLOAT>(GLuint, GLboolean);
template VertexBufferLayout &VertexBufferLayout::pushF<BufferDataType::DOUBLE>(GLuint, GLboolean);

template<BufferDataType T> VertexBufferLayout &VertexBufferLayout::pushI(GLuint count)
{
  m_intAttribs.emplace_back(m_attribIndex, count, T, m_totalOffset);
  m_attribIndex++;
  m_totalOffset += count * getSizeOfType<T>();

  return *this;
}

template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::BYTE>(GLuint);
template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::UNSIGNED_BYTE>(GLuint);
template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::SHORT>(GLuint);
template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::UNSIGNED_SHORT>(GLuint);
template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::INT>(GLuint);
template VertexBufferLayout &VertexBufferLayout::pushI<BufferDataType::UNSIGNED_INT>(GLuint);

VertexBufferLayout &VertexBufferLayout::pushL(GLuint count)
{
  m_doubleAttribs.emplace_back(m_attribIndex, count, m_totalOffset);
  m_attribIndex++;
  m_totalOffset += count * getSizeOfType<BufferDataType::DOUBLE>();

  return *this;
}