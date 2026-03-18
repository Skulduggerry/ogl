#ifndef OGL_BUFFERMAPPING_HPP
#define OGL_BUFFERMAPPING_HPP

#include "ogl/Logging.hpp"

#include <cstddef>
#include <cstdint>
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

  [[nodiscard]] bool valid() const noexcept { return m_bufferId != 0; }

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
    const GLintptr byteOffset = elementOffset * static_cast<GLintptr>(sizeof(T));
    const GLsizeiptr byteCount = elementCount * static_cast<GLsizeiptr>(sizeof(T));
    GLCall(glFlushMappedNamedBufferRange(m_bufferId, byteOffset, byteCount));
  }

  void flushAll() const
    requires(!std::is_const_v<T>)
  { flushElements(0, sizeGL()); }

  template<typename U> friend auto toBytes(BufferMapping<U> &&oldMapping);

  template<typename U, typename ByteT> friend BufferMapping<U> fromBytes(BufferMapping<ByteT> &&oldMapping);
};

template<class T> auto toBytes(BufferMapping<T> &&oldMapping)
{
  using ByteT = std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>;

  auto *ptr = reinterpret_cast<ByteT *>(oldMapping.data());
  const size_t byteCount = oldMapping.m_view.size_bytes();

  // reinterpret pointer with correct constness
  BufferMapping<ByteT> newMapping{ oldMapping.m_bufferId, ptr, byteCount };

  // steal ownership so oldMapping won't unmap
  oldMapping.m_bufferId = 0;
  oldMapping.m_view = {};

  return newMapping;
}

template<typename U, typename ByteT>
  requires std::same_as<std::remove_const_t<ByteT>, std::byte> && std::is_trivially_copyable_v<std::remove_const_t<U>>
BufferMapping<U> fromBytes(BufferMapping<ByteT> &&oldMapping)
{
  if constexpr (std::is_const_v<ByteT>) {
    static_assert(std::is_const_v<U>, "Cannot create BufferMapping<U> from BufferMapping<const std::byte>");
  }

  using BaseU = std::remove_const_t<U>;

  const size_t totalBytes = oldMapping.m_view.size_bytes();
  ASSERT(totalBytes % sizeof(BaseU) == 0);

  const auto addr = reinterpret_cast<std::uintptr_t>(oldMapping.data());
  ASSERT(addr % alignof(BaseU) == 0);

  const size_t count = totalBytes / sizeof(BaseU);
  auto *ptr = reinterpret_cast<U *>(oldMapping.data());

  BufferMapping<U> newMapping{ oldMapping.m_bufferId, ptr, count };

  // steal ownership so oldMapping won't unmap
  oldMapping.m_bufferId = 0;
  oldMapping.m_view = {};

  return newMapping;
}

#endif