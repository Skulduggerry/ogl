#ifndef OGL_VERTEXBUFFERLAYOUT_HPP
#define OGL_VERTEXBUFFERLAYOUT_HPP

#include "DataType.hpp"

#include <glad/glad.h>
#include <vector>

struct AttribFormatI
{
  GLuint attribIndex;
  GLuint count;
  BufferDataType type;
  GLuint relativeOffset;
};

struct AttribFormatL
{
  GLuint attribIndex;
  GLuint count;
  GLuint relativeOffset;
};

struct AttribFormatF
{
  GLuint attribIndex;
  GLuint count;
  BufferDataType type;
  GLboolean normalized;
  GLuint relativeOffset;
};

class VertexBufferLayout
{
  GLuint m_attribIndex = 0;
  GLuint m_totalOffset = 0;

  std::vector<AttribFormatF> m_floatAttribs{};
  std::vector<AttribFormatI> m_intAttribs{};
  std::vector<AttribFormatL> m_doubleAttribs{};

public:
  void increaseAttribIndex(const GLuint amount) { m_attribIndex += amount; }
  void increaseOffset(const GLuint amount) { m_totalOffset += amount; }

  template<BufferDataType T> VertexBufferLayout &pushF(GLuint count, GLboolean normalized = GL_FALSE);
  template<BufferDataType T> VertexBufferLayout &pushI(GLuint count);
  VertexBufferLayout &pushL(GLuint count);

private:
  friend class VertexArray;
};

#endif
