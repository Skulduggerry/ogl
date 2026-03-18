#ifndef OGL_BUFFER_HPP
#define OGL_BUFFER_HPP

#include "BufferMapping.hpp"
#include "Enums.hpp"

#include <glad/glad.h>

enum class BufferType { Vertex, Index, Uniform };

template<typename T>
concept IndexType = std::same_as<T, GLubyte> || std::same_as<T, GLushort> || std::same_as<T, GLuint>;

template<BufferType BT, typename T>
concept BufferElementType = std::is_trivially_copyable_v<T> && (BT != BufferType::Index || IndexType<T>);

template<BufferType BT> class Buffer
{

  GLuint m_id;

  GLsizeiptr m_byteSize = 0;

public:
  Buffer();

  ~Buffer();

  Buffer(Buffer &&other) noexcept;

  Buffer &operator=(Buffer &&other) noexcept;

  Buffer(const Buffer &other) = delete;

  Buffer &operator=(const Buffer &other) = delete;

  template<typename T>
    requires BufferElementType<BT, T>
  void allocateImmutable(std::span<const T> buffer, std::span<const StorageFlag> flags);

  template<typename T>
    requires BufferElementType<BT, T>
  void allocateImmutable(GLsizeiptr count, std::span<const StorageFlag> flags);

  void allocateImmutableBytes(GLsizeiptr byteCount, std::span<const StorageFlag> flags);

  template<typename T>
    requires BufferElementType<BT, T>
  void allocateMutable(std::span<const T> buffer, BufferUsage usage);

  template<typename T>
    requires BufferElementType<BT, T>
  void allocateMutable(GLsizeiptr count, BufferUsage usage);

  void allocateMutableBytes(GLsizeiptr byteCount, BufferUsage usage);

  template<typename T>
    requires BufferElementType<BT, T>
  void subDataElements(GLintptr offset, std::span<const T> buffer);

  void subDataBytes(GLintptr byteOffset, std::span<const std::byte> bytes);

  template<typename T>
    requires BufferElementType<BT, T>
  BufferMapping<T> map(AccessFlag flag);

  BufferMapping<std::byte> mapBytes(AccessFlag flag);

  template<typename T>
    requires BufferElementType<BT, T>
  BufferMapping<T> mapRange(GLintptr offset, GLsizeiptr count, std::span<const RangeAccessFlag> flags);

  BufferMapping<std::byte>
    mapRangeBytes(GLintptr byteOffset, GLsizeiptr byteSize, std::span<const RangeAccessFlag> flags);

  template<BufferType OtherBT>
  void copyBytesFrom(const Buffer<OtherBT> &other,
    GLintptr readByteOffset,
    GLintptr writeByteOffset,
    GLsizeiptr byteCount) const;

  void invalidateData() const;

  template<typename T>
    requires BufferElementType<BT, T>
  void invalidateSubDataElements(GLintptr offset, GLsizeiptr count) const;

  void invalidateSubDataBytes(GLintptr byteOffset, GLsizeiptr byteCount) const;
};

template<> class Buffer<BufferType::Index>
{
public:
  void setIndexFormat(const IndexFormat fmt) { m_indexFormat = fmt; }
  [[nodiscard]] IndexFormat indexFormat() const { return m_indexFormat; }

private:
  IndexFormat m_indexFormat = IndexFormat::UNSIGNED_INT;
};

using VertexBuffer = Buffer<BufferType::Vertex>;
using IndexBuffer = Buffer<BufferType::Index>;
using UniformBuffer = Buffer<BufferType::Uniform>;

// ====================================
// IMPLEMENTATION OF TEMPLATE FUNCTIONS
// ====================================

#endif
