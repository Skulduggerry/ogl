#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "Logging.hpp"
#include "ogl/Concepts.hpp"
#include <glad/glad.h>

class IndexBuffer
{
  GLuint m_id;

public:
  IndexBuffer();
  ~IndexBuffer();
  IndexBuffer(const IndexBuffer &other) = delete;
  IndexBuffer(IndexBuffer &&other) noexcept;

  IndexBuffer &operator=(const IndexBuffer &other) = delete;
  IndexBuffer &operator=(IndexBuffer &&other) noexcept;

  void bind() const;

  template<Container C> void bufferData(const C &data, const GLenum usage) const
  {
    GLCall(glNamedBufferData(
      m_id, static_cast<GLsizeiptr>(data.size() * sizeof(typename C::value_type)), data.data(), usage));
  }
};

#endif// OGL_INDEXBUFFER_HPP
