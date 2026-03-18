#ifndef OGL_UNIFORMBUFFER_HPP
#define OGL_UNIFORMBUFFER_HPP
#include "BufferStorage.hpp"

class UniformBuffer
{
  BufferStorage m_storage;

public:
  [[nodiscard]] GLuint getId() const { return m_storage.getId(); }

  [[nodiscard]] GLsizeiptr getByteSize() const { return m_storage.getByteSize(); }

  void allocateImmutableBytes(GLsizeiptr byteCount, std::span<const BufferStorageFlag> flags);

  void allocateImmutableBytes(std::span<const std::byte> bytes, std::span<const BufferStorageFlag> flags);

  void allocateMutableBytes(GLsizeiptr byteCount, BufferUsage bufferUsage);

  void allocateMutableBytes(std::span<const std::byte> bytes, BufferUsage bufferUsage);

  template<typename T>
    requires std::is_trivially_copyable_v<T>
  void set(GLintptr byteOffset, const T &value) const
  {
    const auto bytes = std::as_bytes(std::span{ &value, 1 });
    subDataBytes(byteOffset, bytes);
  }

  template<typename T>
    requires std::is_trivially_copyable_v<T>
  void setArray(const GLintptr byteOffset, std::span<const T> data) const
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
};

#endif
