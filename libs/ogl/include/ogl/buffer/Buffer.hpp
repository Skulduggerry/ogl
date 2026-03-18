#ifndef OGL_BUFFER_HPP
#define OGL_BUFFER_HPP

#include "BufferMapping.hpp"
#include "Enums.hpp"

#include <algorithm>
#include <glad/glad.h>
#include <ranges>

class Buffer
{
protected:
  GLuint m_id;

  GLsizeiptr m_elementSize = 0;
  GLsizeiptr m_elementCount = 0;

  Buffer();
  Buffer(Buffer &&other) noexcept;
  Buffer &operator=(Buffer &&other) noexcept;

  template<std::ranges::random_access_range B, std::ranges::random_access_range F>
    requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
  void allocateImmutable(const B &buffer, const F &flags);

  template<typename T, std::ranges::random_access_range F>
    requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
  void allocateImmutable(GLsizeiptr count, const F &flags);

  template<std::ranges::random_access_range B> void allocateMutable(const B &buffer, BufferUsage usage);

  template<typename T> void allocateMutable(GLsizeiptr count, BufferUsage usage);

  template<std::ranges::random_access_range B> void subData(GLintptr offset, const B &buffer);

  template<typename T> BufferMapping<T> map(AccessFlag flag);

  template<typename T, std::ranges::random_access_range F>
    requires std::same_as<std::ranges::range_value_t<F>, RangeAccessFlag>
  BufferMapping<T> mapRange(GLintptr offset, GLsizeiptr count, const F &flags);

  template<std::ranges::random_access_range F>
    requires std::is_enum_v<std::ranges::range_value_t<F>>
             && std::is_same_v<std::underlying_type_t<std::ranges::range_value_t<F>>, GLbitfield>
  GLbitfield toBitfield(const F &data);

public:
  virtual ~Buffer();
  Buffer(const Buffer &other) = delete;
  Buffer &operator=(const Buffer &other) = delete;

  void copyFrom(const Buffer &other, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr count) const;

  void invalidateData() const;

  void invalidateSubData(GLintptr offset, GLsizeiptr count) const;
};

// ====================================
// IMPLEMENTATION OF TEMPLATE FUNCTIONS
// ====================================

template<std::ranges::random_access_range B, std::ranges::random_access_range F>
  requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
void Buffer::allocateImmutable(const B &buffer, const F &flags)
{
  using namespace std::ranges;

  m_elementSize = sizeof(range_value_t<B>);
  m_elementCount = size(buffer);

  const GLsizeiptr byteSize = m_elementSize * m_elementCount;
  const GLbitfield bitfield = toBitfield(flags);
  GLCall(glNamedBufferStorage(m_id, byteSize, data(buffer), bitfield));
}

template<typename T, std::ranges::random_access_range F>
  requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
void Buffer::allocateImmutable(const GLsizeiptr count, const F &flags)
{
  m_elementSize = sizeof(T);
  m_elementCount = count;

  const GLsizeiptr byteSize = m_elementSize * m_elementCount;
  const GLbitfield bitfield = toBitfield(flags);
  GLCall(glNamedBufferStorage(m_id, byteSize, nullptr, bitfield));
}

template<std::ranges::random_access_range B> void Buffer::allocateMutable(const B &buffer, BufferUsage usage)
{
  using namespace std::ranges;

  m_elementSize = sizeof(range_value_t<B>);
  m_elementCount = static_cast<GLsizeiptr>(size(buffer));

  const GLsizeiptr byteSize = m_elementSize * m_elementCount;
  GLCall(glNamedBufferData(m_id, byteSize, data(buffer), static_cast<GLenum>(usage)));
}

template<typename T> void Buffer::allocateMutable(const GLsizeiptr count, BufferUsage usage)
{
  m_elementSize = sizeof(T);
  m_elementCount = count;

  const GLsizeiptr byteSize = m_elementSize * m_elementCount;
  GLCall(glNamedBufferData(m_id, byteSize, nullptr, static_cast<GLenum>(usage)));
}

template<std::ranges::random_access_range B> void Buffer::subData(const GLintptr offset, const B &buffer)
{
  using namespace std::ranges;

  // quick and dirty type check
  ASSERT(m_elementSize == sizeof(range_value_t<B>));// type must be same as type that was previously uploaded
  ASSERT(offset + size(buffer) <= m_elementCount);// we don't write out of bounds

  const GLintptr byteOffset = offset * m_elementSize;
  const GLsizeiptr byteSize = size(buffer) * m_elementSize;
  GLCall(glNamedBufferSubData(m_id, byteOffset, byteSize, data(buffer)));
}
template<typename T> BufferMapping<T> Buffer::map(AccessFlag flag)
{
  GLCall(void *ptr = glMapNamedBuffer(m_id, static_cast<GLenum>(flag)));
  return BufferMapping<T>{ m_id, static_cast<T *>(ptr), m_elementCount };
}

template<typename T, std::ranges::random_access_range F>
  requires std::same_as<std::ranges::range_value_t<F>, RangeAccessFlag>
BufferMapping<T> Buffer::mapRange(const GLintptr offset, GLsizeiptr count, const F &flags)
{
  ASSERT(m_elementSize == sizeof(T));// type must be same as type that was previously uploaded
  ASSERT(offset + count <= m_elementCount);// we don't write out of bounds

  const GLintptr byteOffset = offset * m_elementSize;
  const GLsizeiptr byteCount = count * m_elementSize;
  const GLbitfield bitfield = toBitfield(flags);

  void *ptr = glMapNamedBufferRange(m_id, byteOffset, byteCount, bitfield);
  return BufferMapping<T>{ m_id, static_cast<T *>(ptr), count };
}

template<std::ranges::random_access_range F>
  requires std::is_enum_v<std::ranges::range_value_t<F>>
           && std::is_same_v<std::underlying_type_t<std::ranges::range_value_t<F>>, GLbitfield>
GLbitfield Buffer::toBitfield(const F &data)
{
  auto view = data | std::views::transform([](auto f) { return static_cast<GLbitfield>(f); });
  return std::ranges::fold_left(view, 0, std::bit_or<GLbitfield>());
}

#endif
