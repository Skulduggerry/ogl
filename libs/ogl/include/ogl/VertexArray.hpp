#ifndef OGL_VERTEXARRAY_HPP
#define OGL_VERTEXARRAY_HPP
#include "DataType.hpp"
#include "VertexBuffer.hpp"
#include <glad/glad.h>

enum struct AttributeSize : GLint {
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  BGRA = GL_BGRA,
};

class VertexArray
{
  GLuint m_id;

public:
  VertexArray();
  ~VertexArray();
  VertexArray(const VertexArray &other) = delete;
  VertexArray(VertexArray &&other) noexcept;

  VertexArray &operator=(const VertexArray &other) = delete;
  VertexArray &operator=(VertexArray &&other) noexcept;

  // TODO add class VertexBufferLayout to store the layout of a buffer
  void vertexBuffer(GLuint bindingIndex, const VertexBuffer &buffer, GLintptr offset, GLsizei stride) const;
  VertexArray &enableAttrib(GLuint index);
  VertexArray &attribFormat(GLuint attribIndex,
    AttributeSize size,
    BufferDataType type,
    GLboolean normalized,
    GLuint relativeOffset);
  VertexArray &attribIFormat(GLuint attribIndex, AttributeSize size, BufferDataType type, GLuint relativeOffset);
  VertexArray &attribLFormat(GLuint attribIndex, AttributeSize size, BufferDataType type, GLuint relativeOffset);
  void attribBinding(GLuint attribIndex, GLuint bindingIndex) const;
};

#endif// OGL_VERTEXARRAY_HPP
