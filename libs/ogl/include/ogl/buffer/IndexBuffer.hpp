#ifndef OGL_INDEXBUFFER_HPP
#define OGL_INDEXBUFFER_HPP
#include "Buffer.hpp"

#include <vector>

// template<typename T>
// concept IndexType = std::same_as<T, GLubyte> || std::same_as<T, GLushort> || std::same_as<T, GLuint>;
//
// class IndexBuffer : public Buffer
// {
//   IndexFormat m_format = IndexFormat::UNSIGNED_INT;
//
// public:
//   IndexBuffer();
//   IndexBuffer(IndexBuffer &&other) noexcept;
//   IndexBuffer(const IndexBuffer &other) = delete;
//   IndexBuffer &operator=(IndexBuffer &&other) noexcept;
//   IndexBuffer &operator=(const IndexBuffer &other) = delete;
//
//   template<std::ranges::random_access_range B, std::ranges::random_access_range F>
//     requires IndexType<std::ranges::range_value_t<B>> && std::same_as<std::ranges::range_value_t<F>, StorageFlag>
//   void allocateImmutable(const B &buffer, const F &flags);
//
//   template<IndexType T, std::ranges::random_access_range F>
//     requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
//   void allocateImmutable(GLsizeiptr count, const F &flags);
//
//   template<std::ranges::random_access_range B>
//     requires IndexType<std::ranges::range_value_t<B>>
//   void allocateMutable(const B &buffer, BufferUsage usage);
//
//   template<IndexType T> void allocateMutable(GLsizeiptr count, BufferUsage usage);
//
//   template<std::ranges::random_access_range B>
//     requires IndexType<std::ranges::range_value_t<B>>
//   void subData(GLintptr offset, const B &buffer);
//
//   template<IndexType T> BufferMapping<T> map(AccessFlag flag);
//
//   template<IndexType T, std::ranges::random_access_range F>
//     requires std::same_as<std::ranges::range_value_t<F>, RangeAccessFlag>
//   BufferMapping<T> mapRange(GLintptr offset, GLsizeiptr count, const F &flags);
//
// private:
//   template<IndexType T> static IndexFormat fromType();
// };
//
// // ====================================
// // IMPLEMENTATION OF TEMPLATE FUNCTIONS
// // ====================================
//
// template<std::ranges::random_access_range B, std::ranges::random_access_range F>
//   requires IndexType<std::ranges::range_value_t<B>> && std::same_as<std::ranges::range_value_t<F>, StorageFlag>
// void IndexBuffer::allocateImmutable(const B &buffer, const F &flags)
// {
//   Buffer::allocateImmutable(buffer, flags);
//   m_format = fromType<std::ranges::range_value_t<B>>();
// }
//
// template<IndexType T, std::ranges::random_access_range F>
//   requires std::same_as<std::ranges::range_value_t<F>, StorageFlag>
// void IndexBuffer::allocateImmutable(const GLsizeiptr count, const F &flags)
// {
//   Buffer::allocateImmutable<T, F>(count, flags);
//   m_format = fromType<T>();
// }
//
// template<std::ranges::random_access_range B>
//   requires IndexType<std::ranges::range_value_t<B>>
// void IndexBuffer::allocateMutable(const B &buffer, BufferUsage usage)
// {
//   Buffer::allocateMutable(buffer, usage);
//   m_format = fromType<std::ranges::range_value_t<B>>();
// }
//
// template<IndexType T> void IndexBuffer::allocateMutable(const GLsizeiptr count, const BufferUsage usage)
// {
//   Buffer::allocateMutable<T>(count, usage);
//   m_format = fromType<T>();
// }
//
// template<IndexType T> IndexFormat IndexBuffer::fromType()
// {
//   if constexpr (std::same_as<T, GLubyte>) {
//     return IndexFormat::UNSIGNED_BYTE;
//   } else if constexpr (std::same_as<T, GLushort>) {
//     return IndexFormat::UNSIGNED_SHORT;
//   } else {
//     return IndexFormat::UNSIGNED_INT;
//   }
// }

#endif