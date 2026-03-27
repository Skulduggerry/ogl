#ifndef OGL_VERTEXBUFFER_HPP
#define OGL_VERTEXBUFFER_HPP
#include "BufferStorage.hpp"

template<typename T>
  requires std::is_trivially_copyable_v<T>
class VertexBuffer
{
  BufferStorage m_storage;

public:
  VertexBuffer() noexcept = default;

  explicit VertexBuffer(NoCreate_t) noexcept : m_storage(NoCreate) {}

  VertexBuffer(const VertexBuffer &other) = delete;

  VertexBuffer(VertexBuffer &&other) noexcept = default;

  VertexBuffer &operator=(const VertexBuffer &other) = delete;

  VertexBuffer &operator=(VertexBuffer &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_storage.getId(); }

  [[nodiscard]] GLboolean isValid() const noexcept { return m_storage.isValid(); }

  [[nodiscard]] GLsizeiptr getByteSize() const noexcept { return m_storage.getByteSize(); }

  [[nodiscard]] GLsizeiptr getElementCount() const noexcept { return m_storage.getByteSize() / sizeof(T); }

  void allocateImmutable(std::span<const T> data, const std::span<const BufferStorageFlag> flags = {})
  { m_storage.allocateImmutableBytes(std::as_bytes(data), flags); }

  void allocateImmutable(const GLsizeiptr elementCount, const std::span<const BufferStorageFlag> flags = {})
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

  void copyFrom(const VertexBuffer &src,
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
    const std::span<const BufferMappingFlag> flags) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    return fromBytes<T>(m_storage.mapRangeBytes(byteOffset, byteCount, flags));
  }

  [[nodiscard]] BufferMapping<T> mapRangeWrite(const GLintptr elementOffset,
    const GLsizeiptr elementCount,
    const std::span<const BufferMappingFlag> flags) const
  {
    const GLintptr byteOffset = elementOffset * sizeof(T);
    const GLsizeiptr byteCount = elementCount * sizeof(T);
    return fromBytes<T>(m_storage.mapRangeWriteBytes(byteOffset, byteCount, flags));
  }
};

#endif
