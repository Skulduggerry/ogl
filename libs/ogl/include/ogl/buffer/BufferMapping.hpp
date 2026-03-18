#ifndef OGL_BUFFERMAPPING_HPP
#define OGL_BUFFERMAPPING_HPP

#include "ogl/Logging.hpp"


#include <glad/glad.h>
#include <span>
#include <type_traits>
#include <utility>

template<typename T>
  requires std::is_trivially_copyable_v<std::remove_const_t<T>>
class BufferMapping
{
  GLuint m_bufferId = 0;
  std::span<T> m_view{};

public:
  using element_type = T;

  BufferMapping() = default;

  BufferMapping(const GLuint bufferId, T *ptr, const size_t count) noexcept : m_bufferId(bufferId), m_view(ptr, count)
  {}

  BufferMapping(const BufferMapping &other) = delete;

  BufferMapping(BufferMapping &&other) noexcept
    : m_bufferId(std::exchange(other.m_bufferId, 0)), m_view(std::exchange(other.m_view, {}))
  {}

  BufferMapping &operator=(const BufferMapping &other) = delete;

  BufferMapping &operator=(BufferMapping &&other) noexcept
  {
    if (this == &other) { return *this; }
    using std::swap;
    swap(m_bufferId, other.m_bufferId);
    swap(m_view, other.m_view);
    return *this;
  }

  ~BufferMapping() { reset(); }

  void reset() noexcept
  {
    if (valid()) { GLCall(glUnmapNamedBuffer(m_bufferId)); }
    m_bufferId = 0;
    m_view = {};
  }

  [[nodiscard]] bool valid() const noexcept { return m_bufferId != 0 && !m_view.empty(); }

  [[nodiscard]] std::span<T> span() const noexcept { return m_view; }

  [[nodiscard]] T *data() const noexcept { return m_view.data(); }

  [[nodiscard]] auto size() const { return m_view.size(); }

  [[nodiscard]] auto sizeGL() const { return static_cast<GLsizeiptr>(m_view.size()); }

  T &operator[](size_t index) const noexcept { return m_view[index]; }

  [[nodiscard]] auto begin() const noexcept { return m_view.begin(); }

  [[nodiscard]] auto end() const noexcept { return m_view.end(); }

  void flushElements(const GLintptr elementOffset, const GLsizeiptr elementCount) const
    requires(!std::is_const_v<T>)
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    glFlushMappedNamedBufferRange(m_bufferId, byteOffset, byteCount);
  }

  void flushAll() const
    requires(!std::is_const_v<T>)
  { flushElements(0, sizeGL()); }
};

#endif