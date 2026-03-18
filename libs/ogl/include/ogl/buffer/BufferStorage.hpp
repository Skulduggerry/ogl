#ifndef OGL_BUFFERSTORAGE_HPP
#define OGL_BUFFERSTORAGE_HPP

#include "BufferMapping.hpp"
#include "Enums.hpp"

#include <glad/glad.h>
#include <span>

class BufferStorage
{
  GLuint m_id;
  GLsizeiptr m_byteSize = 0;

public:
  BufferStorage();

  ~BufferStorage();

  BufferStorage(const BufferStorage &other) = delete;

  BufferStorage(BufferStorage &&other) noexcept;

  BufferStorage &operator=(const BufferStorage &other) = delete;

  BufferStorage &operator=(BufferStorage &&other) noexcept;

  [[nodiscard]] GLuint getId() const { return m_id; }

  [[nodiscard]] GLsizeiptr getByteSize() const { return m_byteSize; }

  void allocateImmutableBytes(GLsizeiptr byteCount, std::span<const StorageFlag> flags);

  void allocateImmutableBytes(std::span<const std::byte> bytes, std::span<const StorageFlag> flags);

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
    mapRangeBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const MappingFlag> flags) const;

  [[nodiscard]] BufferMapping<const std::byte>
    mapRangeReadBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const MappingFlag> flags) const;

  [[nodiscard]] BufferMapping<std::byte>
    mapRangeWriteBytes(GLintptr byteOffset, GLsizeiptr byteCount, std::span<const MappingFlag> flags) const;
};

#endif