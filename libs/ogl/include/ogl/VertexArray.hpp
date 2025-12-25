#ifndef OGL_VERTEXARRAY_HPP
#define OGL_VERTEXARRAY_HPP
#include "DataType.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include <glad/glad.h>

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

  void bind() const;
  static void unbind();

  void vertexBuffer(GLuint bindingIndex,
    const VertexBuffer &buffer,
    const VertexBufferLayout &layout,
    GLintptr offset) const;
};

#endif// OGL_VERTEXARRAY_HPP
