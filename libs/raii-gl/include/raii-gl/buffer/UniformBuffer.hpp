#ifndef OGL_UNIFORMBUFFER_HPP
#define OGL_UNIFORMBUFFER_HPP

#include "BufferStorage.hpp"
#include "raii-gl/detail/TypeTraits.hpp"

class UniformBuffer
{
  BufferStorage m_storage;

public:
  UniformBuffer() noexcept = default;
  UniformBuffer(const UniformBuffer &other) = delete;
  UniformBuffer(UniformBuffer &&other) noexcept = default;
  UniformBuffer &operator=(const UniformBuffer &other) = delete;
  UniformBuffer &operator=(UniformBuffer &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_storage.getId(); }
  [[nodiscard]] bool hasName() const noexcept { return m_storage.hasName(); }
  [[nodiscard]] GLsizeiptr getByteSize() const noexcept { return m_storage.getByteSize(); }

  void allocateImmutableBytes(GLsizeiptr byteCount, std::span<const BufferStorageFlag> flags);
  void allocateImmutableBytes(std::span<const std::byte> bytes, std::span<const BufferStorageFlag> flags);
  void allocateMutableBytes(GLsizeiptr byteCount, BufferUsage bufferUsage);
  void allocateMutableBytes(std::span<const std::byte> bytes, BufferUsage bufferUsage);

  template<GpuValue T> void setArray(const GLintptr byteOffset, std::span<const T> data) const
  { subDataBytes(byteOffset, std::as_bytes(data)); }

  void subDataBytes(GLintptr byteOffset, std::span<const std::byte> bytes) const;

  void copyBytesFrom(const UniformBuffer &src,
    GLintptr readByteOffset,
    GLintptr writeByteOffset,
    GLsizeiptr byteCount) const;

  void invalidateData() const;
  void invalidateSubDataBytes(GLintptr byteOffset, GLsizeiptr byteCount) const;

  [[nodiscard]] BufferMapping<std::byte> mapWriteBytes() const;
  [[nodiscard]] BufferMapping<std::byte>
    mapRangeWriteBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const BufferMappingFlag> flags) const;

  void bindBase(GLuint bindingIndex) const;
  void bindRange(GLuint bindingIndex, GLintptr byteOffset, GLsizeiptr byteCount) const;

  void debugLabel(const std::string_view name) const { m_storage.debugLabel(name); }
};

#endif
