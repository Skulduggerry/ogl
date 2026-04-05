#ifndef OGL_BUFFERSTORAGE_HPP
#define OGL_BUFFERSTORAGE_HPP

#include "BufferEnums.hpp"
#include "BufferMapping.hpp"

#include <glad/glad.h>
#include <span>
#include <string_view>

class BufferStorage
{
  GLuint m_id;
  GLsizeiptr m_byteSize = 0;

public:
  BufferStorage() noexcept;
  ~BufferStorage() noexcept;
  BufferStorage(const BufferStorage &other) = delete;
  BufferStorage(BufferStorage &&other) noexcept;
  BufferStorage &operator=(const BufferStorage &other) = delete;
  BufferStorage &operator=(BufferStorage &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }
  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }
  [[nodiscard]] GLsizeiptr getByteSize() const noexcept { return m_byteSize; }

  void allocateImmutableBytes(GLsizeiptr byteCount, std::span<const BufferStorageFlag> flags);
  void allocateImmutableBytes(std::span<const std::byte> bytes, std::span<const BufferStorageFlag> flags);
  void allocateMutableBytes(GLsizeiptr byteCount, BufferUsage bufferUsage);
  void allocateMutableBytes(std::span<const std::byte> bytes, BufferUsage bufferUsage);

  void subDataBytes(GLintptr byteOffset, std::span<const std::byte> bytes) const;
  void getSubDataBytes(GLintptr byteOffset, std::span<std::byte> out) const;

  void copyBytesFrom(const BufferStorage &src,
    GLintptr readByteOffset,
    GLintptr writeByteOffset,
    GLsizeiptr byteCount) const;

  void invalidateData() const;
  void invalidateSubDataBytes(GLintptr byteOffset, GLsizeiptr byteCount) const;

  [[nodiscard]] BufferMapping<std::byte> mapBytes() const;
  [[nodiscard]] BufferMapping<const std::byte> mapReadBytes() const;
  [[nodiscard]] BufferMapping<std::byte> mapWriteBytes() const;
  [[nodiscard]] BufferMapping<std::byte>
    mapRangeBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const BufferMappingFlag> flags) const;
  [[nodiscard]] BufferMapping<const std::byte>
    mapRangeReadBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const BufferMappingFlag> flags) const;
  [[nodiscard]] BufferMapping<std::byte>
    mapRangeWriteBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const BufferMappingFlag> flags) const;

  void debugLabel(std::string_view name) const;
};

#endif