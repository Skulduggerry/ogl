#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "BufferUsage.hpp"
#include "Logging.hpp"

#include <glad/glad.h>
#include <ranges>

enum struct IndexFormat : GLenum {
  // all index data types supported by OpenGL
  // ========================================
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
};

class IndexBuffer
{
  GLuint m_id;
  IndexFormat m_type = IndexFormat::UNSIGNED_INT;

public:
  IndexBuffer();
  ~IndexBuffer();
  IndexBuffer(const IndexBuffer &other) = delete;
  IndexBuffer(IndexBuffer &&other) noexcept;

  IndexBuffer &operator=(const IndexBuffer &other) = delete;
  IndexBuffer &operator=(IndexBuffer &&other) noexcept;

  void bind() const;

  [[nodiscard]] IndexFormat getType() const { return m_type; }

  template<std::ranges::random_access_range R> void bufferData(const R &buffer, const BufferUsage usage)
  {
    using namespace std::ranges;
    // TODO: maybe switch to glNamedBufferStorage for performance reasons
    GLCall(glNamedBufferData(m_id,
      static_cast<GLsizeiptr>(size(buffer) * sizeof(range_value_t<R>)),
      data(buffer),
      static_cast<GLenum>(usage)));

    if constexpr (std::same_as<GLubyte, range_value_t<R>>) {
      m_type = IndexFormat::UNSIGNED_BYTE;
    } else if constexpr (std::same_as<GLushort, range_value_t<R>>) {
      m_type = IndexFormat::UNSIGNED_SHORT;
    } else if constexpr (std::same_as<GLuint, range_value_t<R>>) {
      m_type = IndexFormat::UNSIGNED_INT;
    } else {
      static_assert(false, "Unsupported type: Please make sure your container stores GLubyte, GLushort or GLuint!");
    }
  }
};

#endif// OGL_INDEXBUFFER_HPP