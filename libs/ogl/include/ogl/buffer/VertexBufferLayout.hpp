#ifndef OGL_VERTEXBUFFERLAYOUT_HPP
#define OGL_VERTEXBUFFERLAYOUT_HPP

#include "ogl/Types.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <span>
#include <vector>

enum struct AttributeKind : uint8_t { FLOAT, INT, DOUBLE };

struct VertexAttribDesc
{
  GLuint location = 0;
  GLint components = 0;
  AttributeType type = AttributeType::FLOAT;
  AttributeKind kind = AttributeKind::FLOAT;
  GLboolean normalized = GL_FALSE;
  GLuint relativeOffset = 0;
};

class VertexBufferLayout
{
  GLint m_stride = 0;
  GLuint m_nextLocation = 0;
  std::vector<VertexAttribDesc> m_attribs;

public:
  [[nodiscard]] GLint stride() const noexcept { return m_stride; }

  VertexBufferLayout &addPaddingBytes(const GLint padding)
  {
    m_stride += padding;
    return *this;
  }

  VertexBufferLayout &incrementLocation(const GLuint increment)
  {
    m_nextLocation += increment;
    return *this;
  }

  [[nodiscard]] std::span<const VertexAttribDesc> attribs() const noexcept { return m_attribs; }

  VertexBufferLayout &pushFloat(AttributeType type, GLint components, GLboolean normalized = GL_FALSE);

  VertexBufferLayout &pushInt(AttributeType type, GLint components);

  VertexBufferLayout &pushDouble(GLint components);
};

#endif
