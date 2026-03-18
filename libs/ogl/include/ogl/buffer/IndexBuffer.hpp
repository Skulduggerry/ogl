#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "BufferStorage.hpp"


#include <concepts>
#include <glad/glad.h>

template<typename T>
concept IndexType = std::same_as<std::remove_const_t<T>, GLubyte> || std::same_as<std::remove_const_t<T>, GLushort>
                    || std::same_as<std::remove_const_t<T>, GLuint>;

template<IndexType T> class IndexBuffer
{
  BufferStorage m_storage;

public:
  [[nodiscard]] GLuint getId() const { return m_storage.getId(); }

  [[nodiscard]] GLsizeiptr getByteSize() const { return m_storage.getByteSize(); }

  [[nodiscard]] GLsizeiptr getElementCount() const { return m_storage.getByteSize() / sizeof(T); }

  [[nodiscard]] static consteval IndexFormat getFormat()
  {
    using BaseT = std::remove_const_t<T>;
    if constexpr (std::same_as<BaseT, GLubyte>) {
      return IndexFormat::UNSIGNED_BYTE;
    } else if constexpr (std::same_as<BaseT, GLushort>) {
      return IndexFormat::UNSIGNED_SHORT;
    } else {
      return IndexFormat::UNSIGNED_INT;
    }
  }

  void allocateImmutable(std::span<const T> data, const std::span<const StorageFlag> flags = {})
  { m_storage.allocateImmutableBytes(std::as_bytes(data), flags); }

  void allocateImmutable(const GLsizeiptr elementCount, const std::span<const StorageFlag> flags = {})
  {
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    m_storage.allocateImmutableBytes(byteCount, flags);
  }

  void allocateMutable(std::span<const T> data, const BufferUsage usage = BufferUsage::STATIC_DRAW)
  { m_storage.allocateMutableBytes(std::as_bytes(data), usage); }

  void allocateMutable(const GLsizeiptr elementCount, const BufferUsage usage = BufferUsage::STATIC_DRAW)
  {
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    m_storage.allocateMutableBytes(byteCount, usage);
  }

  void subData(const GLintptr elementOffset, std::span<const T> data) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    m_storage.subDataBytes(byteOffset, std::as_bytes(data));
  }

  void copyFrom(const IndexBuffer &src,
    const GLintptr readElementOffset,
    const GLintptr writeElementOffset,
    const GLsizeiptr elementCount) const
  {
    const GLintptr readByteOffset = readElementOffset * sizeof(T);
    const GLintptr writeByteOffset = writeElementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    m_storage.copyBytesFrom(src.m_storage, readByteOffset, writeByteOffset, byteCount);
  }

  void invalidateData() const { m_storage.invalidateData(); }

  void invalidateSubData(const GLintptr elementOffset, const GLsizeiptr elementCount) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    m_storage.invalidateSubDataBytes(byteOffset, byteCount);
  }

  [[nodiscard]] BufferMapping<T> map() const { return fromBytes<T>(m_storage.mapBytes()); }

  [[nodiscard]] BufferMapping<T> mapWrite() const { return fromBytes<T>(m_storage.mapWriteBytes()); }

  [[nodiscard]] BufferMapping<T> mapRange(const GLintptr elementOffset,
    const GLsizeiptr elementCount,
    const std::span<const MappingFlag> flags) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    return fromBytes<T>(m_storage.mapRangeBytes(byteOffset, byteCount, flags));
  }

  [[nodiscard]] BufferMapping<T> mapRangeWrite(const GLintptr elementOffset,
    const GLsizeiptr elementCount,
    const std::span<const MappingFlag> flags) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    return fromBytes<T>(m_storage.mapRangeWriteBytes(byteOffset, byteCount, flags));
  }
};

#endif
