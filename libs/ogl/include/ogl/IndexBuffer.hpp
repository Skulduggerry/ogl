#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "Logging.hpp"

#include <glad/glad.h>
#include <ranges>

enum struct IndexType: GLenum
{
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
};

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

  template<std::ranges::random_access_range R> void bufferData(const R &buffer, const GLenum usage) const
  {
    using namespace std::ranges;
    GLCall(
      glNamedBufferData(m_id, static_cast<GLsizeiptr>(size(buffer) * sizeof(range_value_t<R>)), data(buffer), usage));
    // TODO: store the type of indices
  }
};

#endif// OGL_INDEXBUFFER_HPP