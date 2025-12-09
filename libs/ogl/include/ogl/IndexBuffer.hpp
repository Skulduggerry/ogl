#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "BufferUsage.hpp"
#include "DataType.hpp"
#include "Logging.hpp"

#include <glad/glad.h>
#include <ranges>

class IndexBuffer
{
  GLuint m_id;
  IndexType m_type = IndexType::UNSIGNED_INT;

public:
  IndexBuffer();
  ~IndexBuffer();
  IndexBuffer(const IndexBuffer &other) = delete;
  IndexBuffer(IndexBuffer &&other) noexcept;

  IndexBuffer &operator=(const IndexBuffer &other) = delete;
  IndexBuffer &operator=(IndexBuffer &&other) noexcept;

  void bind() const;

  template<std::ranges::random_access_range R> void bufferData(const R &buffer, const BufferUsage usage)
  {
    using namespace std::ranges;
    GLCall(glNamedBufferData(m_id,
      static_cast<GLsizeiptr>(size(buffer) * sizeof(range_value_t<R>)),
      data(buffer),
      static_cast<GLenum>(usage)));

    if constexpr (std::same_as<GLubyte, range_value_t<R>>) {
      m_type = IndexType::UNSIGNED_BYTE;
    } else if constexpr (std::same_as<GLushort, range_value_t<R>>) {
      m_type = IndexType::UNSIGNED_SHORT;
    } else if constexpr (std::same_as<GLuint, range_value_t<R>>) {
      m_type = IndexType::UNSIGNED_INT;
    } else {
      static_assert(false, "Unsupported type: Please make sure your container stores GLubyte, GLushort or GLuint!");
    }
  }
};

#endif// OGL_INDEXBUFFER_HPP