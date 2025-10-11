#ifndef OGL_VERTEXARRAY_HPP
#define OGL_VERTEXARRAY_HPP
#include "VertexBuffer.hpp"
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

  void vertexBuffer(GLuint bindingIndex, const VertexBuffer &buffer, GLintptr offset, GLsizei stride) const;
  VertexArray &enableAttrib(GLuint index);
  VertexArray &attribFormat(GLuint attribIndex, GLint size, GLenum type, GLboolean normalized, GLuint relativeOffset);
  VertexArray &attribIFormat(GLuint attribIndex, GLint size, GLenum type, GLuint relativeOffset);
  VertexArray &attribLFormat(GLuint attribIndex, GLint size, GLenum type, GLuint relativeOffset);
  void attribBinding(GLuint attribIndex, GLuint bindingIndex) const;
};

#endif// OGL_VERTEXARRAY_HPP
