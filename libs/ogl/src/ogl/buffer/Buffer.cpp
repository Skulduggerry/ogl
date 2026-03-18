#include "ogl/buffer/Buffer.hpp"

GLuint createBuffer()
{
  GLuint id; //NOLINT
  glCreateBuffers(1, &id);
  return id;
}

Buffer::Buffer() : m_id(createBuffer()) {}

Buffer::Buffer(Buffer &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_elementSize(std::exchange(other.m_elementSize, 0)),
    m_elementCount(std::exchange(other.m_elementCount, 0))
{}

Buffer &Buffer::operator=(Buffer &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  swap(m_elementSize, other.m_elementSize);
  swap(m_elementCount, other.m_elementCount);
  return *this;
}

Buffer::~Buffer()
{
  if (m_id != 0) { glDeleteBuffers(1, &m_id); }
}

void Buffer::copyFrom(const Buffer &other,
  const GLintptr readOffset,
  const GLintptr writeOffset,
  const GLsizeiptr count) const
{
  // quick and dirty sanity checks
  ASSERT(m_elementSize == other.m_elementSize);
  ASSERT(readOffset + count <= other.m_elementCount);
  ASSERT(writeOffset + count < m_elementCount);

  const GLintptr byteReadOffset = readOffset * m_elementSize;
  const GLintptr byteWriteOffset = writeOffset * m_elementSize;
  const GLsizeiptr byteSize = count * m_elementSize;
  GLCall(glCopyNamedBufferSubData(other.m_id, m_id, byteReadOffset, byteWriteOffset, byteSize));
}

void Buffer::invalidateData() const { GLCall(glInvalidateBufferData(m_id)); }

void Buffer::invalidateSubData(const GLintptr offset, const GLsizeiptr count) const
{
  const GLintptr byteOffset = offset * m_elementSize;
  const GLsizeiptr byteSize = count * m_elementSize;
  GLCall(glInvalidateBufferSubData(m_id, byteOffset, byteSize));
}