#ifndef OGL_BUFFERMAPPING_HPP
#define OGL_BUFFERMAPPING_HPP
#include "ogl/Logging.hpp"


#include <glad/glad.h>
#include <span>
#include <utility>

template<typename T> class BufferMapping
{
  GLuint m_bufferId;
  std::span<T> m_view;

public:
  BufferMapping(GLuint bufferId, T *mappedPtr, GLsizeiptr count);
  ~BufferMapping();
  BufferMapping(const BufferMapping &other) = delete;
  BufferMapping(BufferMapping &&other) noexcept;

  BufferMapping &operator=(const BufferMapping &other) = delete;
  BufferMapping &operator=(BufferMapping &&other) noexcept;

  std::span<T> span() const;

  T *data();

  T &operator[](size_t index);

  auto size() const;

  auto begin();

  auto end();

  void flush(GLintptr offset, GLsizeiptr count) const;

  void flushAll() const;
};

// ====================================
// IMPLEMENTATION OF TEMPLATE FUNCTIONS
// ====================================

template<typename T>
BufferMapping<T>::BufferMapping(const GLuint bufferId, T *mappedPtr, const GLsizeiptr count)
  : m_bufferId(bufferId), m_view{ mappedPtr, static_cast<size_t>(count) }
{}

template<typename T>
BufferMapping<T>::BufferMapping(BufferMapping &&other) noexcept
  : m_bufferId(std::exchange(other.m_bufferId, 0)), m_view(std::exchange(other.m_view, {}))
{}

template<typename T> BufferMapping<T>::~BufferMapping()
{
  if (m_bufferId != 0 && !m_view.empty()) { GLCall(glUnmapNamedBuffer(m_bufferId)); }
}

template<typename T> BufferMapping<T> &BufferMapping<T>::operator=(BufferMapping &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_bufferId, other.m_bufferId);
  swap(m_view, other.m_view);
  return *this;
}

template<typename T> std::span<T> BufferMapping<T>::span() const { return m_view; }

template<typename T> T *BufferMapping<T>::data() { return m_view.data(); }

template<typename T> T &BufferMapping<T>::operator[](size_t index) { return m_view[index]; }

template<typename T> auto BufferMapping<T>::size() const { return m_view.size(); }

template<typename T> auto BufferMapping<T>::begin() { return m_view.begin(); }

template<typename T> auto BufferMapping<T>::end() { return m_view.end(); }

template<typename T> void BufferMapping<T>::flush(const GLintptr offset, const GLsizeiptr count) const
{
  const GLintptr byteOffset = sizeof(T) * offset;
  const GLsizeiptr byteCount = sizeof(T) * count;

  GLCall(glFlushMappedNamedBufferRange(m_bufferId, byteOffset, byteCount));
}

template<typename T> void BufferMapping<T>::flushAll() const { flush(0, size()); }

#endif