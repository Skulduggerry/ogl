#ifndef OGL_VERTEXBUFFER_HPP
#define OGL_VERTEXBUFFER_HPP
#include "Logging.hpp"
#include "ogl/Concepts.hpp"
#include <glad/glad.h>

class VertexBuffer
{
  GLuint m_id;

public:
  VertexBuffer();
  ~VertexBuffer();
  VertexBuffer(const VertexBuffer &other) = delete;
  VertexBuffer(VertexBuffer &&other) noexcept;

  VertexBuffer &operator=(const VertexBuffer &other) = delete;
  VertexBuffer &operator=(VertexBuffer &&other) noexcept;

  template<Container C> void bufferData(const C &data, const GLenum usage) const
  {
    GLCall(glNamedBufferData(
      m_id, static_cast<GLsizeiptr>(data.size() * sizeof(typename C::value_type)), data.data(), usage));
  }

  friend class VertexArray;
};

#endif// OGL_VERTEXBUFFER_HPP
