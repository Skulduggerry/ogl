#ifndef OGL_VERTEXBUFFER_HPP
#define OGL_VERTEXBUFFER_HPP
#include "BufferUsage.hpp"
#include "Logging.hpp"

#include <glad/glad.h>
#include <ranges>

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

  template<std::ranges::random_access_range R> void bufferData(const R &buffer, const BufferUsage usage) const
  {
    using namespace std::ranges;
    // TODO: maybe switch to glNamedBufferStorage for performance reasons
    GLCall(glNamedBufferData(m_id,
      static_cast<GLsizeiptr>(size(buffer) * sizeof(range_value_t<R>)),
      data(buffer),
      static_cast<GLenum>(usage)));
  }

  friend class VertexArray;
};

#endif// OGL_VERTEXBUFFER_HPP
