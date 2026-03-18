#include "ogl/buffer/BufferStorage.hpp"
#include "ogl/Logging.hpp"
#include "ogl/UniformBuffer.hpp"

#include <algorithm>
#include <numeric>

static GLuint createBuffer()
{
  GLuint id;
  GLCall(glCreateBuffers(1, &id));
  return id;
}

BufferStorage::BufferStorage() : m_id(createBuffer()) {}

BufferStorage::~BufferStorage()
{
  if (m_id != 0) { GLCall(glDeleteBuffers(1, &m_id)); }
}

BufferStorage::BufferStorage(BufferStorage &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_byteSize(std::exchange(other.m_byteSize, 0))
{}

BufferStorage &BufferStorage::operator=(BufferStorage &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  swap(m_byteSize, other.m_byteSize);
  return *this;
}

// ---- helpers to combine flags ----
template<typename T>
  requires std::is_enum_v<T>
static GLbitfield toBitfield(const std::span<const T> flags)
{
  GLbitfield bits = 0;
  for (const T flag : flags) { bits |= static_cast<GLbitfield>(flag); }
  return bits;
}

void BufferStorage::allocateImmutableBytes(const GLsizeiptr byteCount, const std::span<const StorageFlag> flags)
{
  m_byteSize = byteCount;
  GLCall(glNamedBufferStorage(m_id, byteCount, nullptr, toBitfield(flags)));
}

void BufferStorage::allocateImmutableBytes(const std::span<const std::byte> bytes,
  const std::span<const StorageFlag> flags)
{
  m_byteSize = static_cast<GLsizeiptr>(bytes.size_bytes());
  GLCall(glNamedBufferStorage(m_id, m_byteSize, bytes.data(), toBitfield(flags)));
}

void BufferStorage::allocateMutableBytes(const GLsizeiptr byteCount, const BufferUsage bufferUsage)
{
  m_byteSize = byteCount;
  GLCall(glNamedBufferData(m_id, byteCount, nullptr, static_cast<GLenum>(bufferUsage)));
}

void BufferStorage::allocateMutableBytes(const std::span<const std::byte> bytes, const BufferUsage bufferUsage)
{
  m_byteSize = static_cast<GLsizeiptr>(bytes.size_bytes());
  glNamedBufferData(m_id, m_byteSize, bytes.data(), static_cast<GLenum>(bufferUsage));
}

void BufferStorage::subDataBytes(const GLintptr byteOffset, const std::span<const std::byte> bytes) const
{
  const auto byteSize = static_cast<GLsizeiptr>(bytes.size_bytes());
  ASSERT(byteOffset + byteSize <= m_byteSize);
  GLCall(glNamedBufferSubData(m_id, byteOffset, byteSize, bytes.data()));
}

void BufferStorage::getSubDataBytes(const GLintptr byteOffset, std::span<std::byte> out) const
{
  const auto byteSize = static_cast<GLsizeiptr>(out.size_bytes());
  ASSERT(byteOffset + byteSize <= m_byteSize);
  GLCall(glGetNamedBufferSubData(m_id, byteOffset, byteSize, out.data()));
}

void BufferStorage::copyBytesFrom(const BufferStorage &src,
  const GLintptr readByteOffset,
  const GLintptr writeByteOffset,
  const GLsizeiptr byteCount) const
{
  ASSERT(readByteOffset + byteCount <= src.m_byteSize);
  ASSERT(writeByteOffset + byteCount <= m_byteSize);
  GLCall(glCopyNamedBufferSubData(src.m_id, m_id, readByteOffset, writeByteOffset, byteCount));
}

void BufferStorage::invalidateData() const { GLCall(glInvalidateBufferData(m_id)); }

void BufferStorage::invalidateSubDataBytes(const GLintptr byteOffset, const GLsizeiptr byteCount) const
{
  ASSERT(byteOffset + byteCount <= m_byteSize);
  GLCall(glInvalidateBufferSubData(m_id, byteOffset, byteCount));
}

BufferMapping<std::byte> BufferStorage::mapBytes() const
{
  GLCall(void *ptr = glMapNamedBuffer(m_id, GL_READ_WRITE));
  return BufferMapping{ m_id, static_cast<std::byte *>(ptr), static_cast<size_t>(m_byteSize) };
}

BufferMapping<const std::byte> BufferStorage::mapReadBytes() const
{
  GLCall(const void *ptr = glMapNamedBuffer(m_id, GL_READ_ONLY));
  return BufferMapping{ m_id, static_cast<const std::byte *>(ptr), static_cast<size_t>(m_byteSize) };
}

BufferMapping<std::byte> BufferStorage::mapWriteBytes() const
{
  GLCall(void *ptr = glMapNamedBuffer(m_id, GL_WRITE_ONLY));
  return BufferMapping{ m_id, static_cast<std::byte *>(ptr), static_cast<size_t>(m_byteSize) };
}

BufferMapping<std::byte> BufferStorage::mapRangeBytes(const GLintptr byteOffset,
  const GLsizeiptr byteCount,
  const std::span<const MappingFlag> flags) const
{
  ASSERT(byteOffset + byteCount <= m_byteSize);

  GLbitfield const bitfield = toBitfield(flags) | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
  GLCall(void *ptr = glMapNamedBufferRange(m_id, byteOffset, byteCount, bitfield));
  return BufferMapping{ m_id, static_cast<std::byte *>(ptr), static_cast<size_t>(byteCount) };
}

BufferMapping<const std::byte> BufferStorage::mapRangeReadBytes(const GLintptr byteOffset,
  const GLsizeiptr byteCount,
  const std::span<const MappingFlag> flags) const
{
  ASSERT(byteOffset + byteCount <= m_byteSize);

  GLbitfield const bitfield = toBitfield(flags) | GL_MAP_READ_BIT;
  GLCall(const void *ptr = glMapNamedBufferRange(m_id, byteOffset, byteCount, bitfield));
  return BufferMapping{ m_id, static_cast<const std::byte *>(ptr), static_cast<size_t>(byteCount) };
}

BufferMapping<std::byte> BufferStorage::mapRangeWriteBytes(const GLintptr byteOffset,
  const GLsizeiptr byteCount,
  const std::span<const MappingFlag> flags) const
{
  ASSERT(byteOffset + byteCount <= m_byteSize);

  GLbitfield const bitfield = toBitfield(flags) | GL_MAP_WRITE_BIT;
  GLCall(void *ptr = glMapNamedBufferRange(m_id, byteOffset, byteCount, bitfield));
  return BufferMapping{ m_id, static_cast<std::byte *>(ptr), static_cast<size_t>(byteCount) };
}
