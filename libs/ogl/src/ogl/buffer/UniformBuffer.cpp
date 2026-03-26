#include "ogl/buffer/UniformBuffer.hpp"

#include "ogl/GlLimits.hpp"

UniformBuffer::UniformBuffer(NoCreate_t) noexcept : m_storage(NoCreate) {}

void UniformBuffer::allocateImmutableBytes(const GLsizeiptr byteCount, const std::span<const BufferStorageFlag> flags)
{
  ASSERT(byteCount <= GlLimits::getInstance().maxUniformBlockSize);
  m_storage.allocateImmutableBytes(byteCount, flags);
}

void UniformBuffer::allocateImmutableBytes(const std::span<const std::byte> bytes,
  const std::span<const BufferStorageFlag> flags)
{
  ASSERT(bytes.size_bytes() <= GlLimits::getInstance().maxUniformBlockSize);
  m_storage.allocateImmutableBytes(bytes, flags);
}

void UniformBuffer::allocateMutableBytes(const GLsizeiptr byteCount, const BufferUsage bufferUsage)
{
  ASSERT(byteCount <= GlLimits::getInstance().maxUniformBlockSize);
  m_storage.allocateMutableBytes(byteCount, bufferUsage);
}

void UniformBuffer::allocateMutableBytes(const std::span<const std::byte> bytes, const BufferUsage bufferUsage)
{
  ASSERT(bytes.size_bytes() <= GlLimits::getInstance().maxUniformBlockSize);
  m_storage.allocateMutableBytes(bytes, bufferUsage);
}

void UniformBuffer::subDataBytes(const GLintptr byteOffset, const std::span<const std::byte> bytes) const
{ m_storage.subDataBytes(byteOffset, bytes); }

void UniformBuffer::copyBytesFrom(const UniformBuffer &src,
  const GLintptr readByteOffset,
  const GLintptr writeByteOffset,
  const GLsizeiptr byteCount) const
{ m_storage.copyBytesFrom(src.m_storage, readByteOffset, writeByteOffset, byteCount); }

void UniformBuffer::invalidateData() const { m_storage.invalidateData(); }

void UniformBuffer::invalidateSubDataBytes(const GLintptr byteOffset, const GLsizeiptr byteCount) const
{ m_storage.invalidateSubDataBytes(byteOffset, byteCount); }

BufferMapping<std::byte> UniformBuffer::mapWriteBytes() const { return m_storage.mapWriteBytes(); }

BufferMapping<std::byte> UniformBuffer::mapRangeWriteBytes(const GLintptr byteOffset,
  const GLsizeiptr byteCount,
  const std::span<const BufferMappingFlag> flags) const
{ return m_storage.mapRangeWriteBytes(byteOffset, byteCount, flags); }

void UniformBuffer::bindBase(const GLuint bindingIndex) const
{ GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, m_storage.getId())); }

void UniformBuffer::bindRange(const GLuint bindingIndex, const GLintptr byteOffset, const GLsizeiptr byteCount) const
{
  ASSERT(byteOffset + byteCount <= m_storage.getByteSize());
  ASSERT(byteOffset % GlLimits::getInstance().uboOffsetAlignment == 0);

  GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, m_storage.getId(), byteOffset, byteCount));
}