#ifndef OGL_UNIFORMBUFFER_HPP
#define OGL_UNIFORMBUFFER_HPP
#include "Logging.hpp"
#include "buffer/Buffer.hpp"

#include <glad/glad.h>
#include <ranges>

class UniformBuffer
{
  GLuint m_id;

public:
  UniformBuffer();
  ~UniformBuffer();
  UniformBuffer(const UniformBuffer &other) = delete;
  UniformBuffer(UniformBuffer &&other) noexcept;

  UniformBuffer &operator=(const UniformBuffer &other) = delete;
  UniformBuffer &operator=(UniformBuffer &&other) noexcept;

  void bufferData(GLsizeiptr size, BufferUsage usage) const;

  template<std::ranges::random_access_range R> void bufferData(const R &buffer, const BufferUsage usage) const
  {
    using namespace std::ranges;
    GLCall(glNamedBufferData(m_id,
      static_cast<GLsizeiptr>(size(buffer) * sizeof(range_value_t<R>)),
      data(buffer),
      static_cast<GLenum>(usage)));
  }

  template<std::ranges::random_access_range R>
  void subData(const GLintptr offset, const GLsizeiptr size, const R &buffer)
  {
    using namespace std::ranges;
    ASSERT(size <= static_cast<GLsizeiptr>(std::ranges::size(buffer) * sizeof(range_value_t<R>)));
    GLCall(glNamedBufferSubData(m_id, offset, size * static_cast<GLsizeiptr>(sizeof(range_value_t<R>)), data(buffer)));
  }

  void bind(GLuint bindingPoint) const;
  void bind(GLuint bindingPoint, GLintptr offset, GLsizeiptr size) const;
};

#endif
