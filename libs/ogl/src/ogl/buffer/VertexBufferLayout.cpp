#include "ogl/buffer/VertexBufferLayout.hpp"
#include "ogl/Logging.hpp"

VertexBufferLayout &
  VertexBufferLayout::pushFloat(const AttributeType type, const GLint components, const GLboolean normalized)
{
  ASSERT(1 <= components && components <= 4);
  ASSERT(type != AttributeType::DOUBLE);
  m_attribs.emplace_back(m_nextLocation++, components, type, AttributeKind::FLOAT, normalized, m_stride);
  m_stride += components * getSizeOfType(type);
  return *this;
}

VertexBufferLayout &VertexBufferLayout::pushInt(const AttributeType type, const GLint components)
{
  ASSERT(1 <= components && components <= 4);
  ASSERT(type != AttributeType::FIXED && type != AttributeType::FLOAT && type != AttributeType::HALF_FLOAT
         && type != AttributeType::DOUBLE);
  m_attribs.emplace_back(m_nextLocation++, components, type, AttributeKind::INT, GL_FALSE, m_stride);
  m_stride += components * getSizeOfType(type);
  return *this;
}

VertexBufferLayout &VertexBufferLayout::pushDouble(const GLint components)
{
  ASSERT(1 <= components && components <= 4);
  m_attribs.emplace_back(
    m_nextLocation++, components, AttributeType::DOUBLE, AttributeKind::DOUBLE, GL_FALSE, m_stride);
  m_stride += components * getSizeOfType(AttributeType::DOUBLE);
  return *this;
}