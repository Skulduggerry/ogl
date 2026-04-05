#ifndef OGL_VERTEXBUFFERLAYOUT_HPP
#define OGL_VERTEXBUFFERLAYOUT_HPP

#include "raii-gl/Types.hpp"

#include <cstdint>
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
  std::vector<VertexAttribDesc> m_attribs{};

public:
  VertexBufferLayout() = default;
  VertexBufferLayout(const VertexBufferLayout &other) = delete;
  VertexBufferLayout(VertexBufferLayout &&other) noexcept = default;
  VertexBufferLayout &operator=(const VertexBufferLayout &other) = delete;
  VertexBufferLayout &operator=(VertexBufferLayout &&other) noexcept = default;

  [[nodiscard]] GLint stride() const noexcept { return m_stride; }
  [[nodiscard]] std::span<const VertexAttribDesc> attribs() const noexcept { return m_attribs; }

  VertexBufferLayout &addPaddingBytes(const GLint padding);
  VertexBufferLayout &incrementLocation(const GLuint increment);

  VertexBufferLayout &pushFloat(AttributeType type, GLint components, GLboolean normalized = GL_FALSE);
  VertexBufferLayout &pushInt(AttributeType type, GLint components);
  VertexBufferLayout &pushDouble(GLint components);
};

#endif
