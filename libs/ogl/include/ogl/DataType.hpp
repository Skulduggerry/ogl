#ifndef OGL_DATATYPE_HPP
#define OGL_DATATYPE_HPP

#include <concepts>
#include <glad/glad.h>

enum struct ImageDataType : GLenum {
  // all image data types supported by OpenGL
  // ========================================
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  BYTE = GL_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  SHORT = GL_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  INT = GL_INT,
  FLOAT = GL_FLOAT,
  UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
  UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
  UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
  UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
  UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
  UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
  UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
  UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
  UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
  UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
  UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
  UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
};

enum struct IndexType : GLenum {
  // all index data types supported by OpenGL
  // ========================================
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
};

enum struct BufferDataType : GLenum {
  // all buffer data types supported by OpenGL
  // =========================================
  BYTE = GL_BYTE,
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  SHORT = GL_SHORT,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  INT = GL_INT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  FIXED = GL_FIXED,
  FLOAT = GL_FLOAT,
  HALF_FLOAT = GL_HALF_FLOAT,
  DOUBLE = GL_DOUBLE,
};

template<BufferDataType T> consteval GLuint getSizeOfType()
{
  if constexpr (T == BufferDataType::BYTE) {
    return sizeof(GLbyte);
  } else if constexpr (T == BufferDataType::UNSIGNED_BYTE) {
    return sizeof(GLubyte);
  } else if constexpr (T == BufferDataType::SHORT) {
    return sizeof(GLshort);
  } else if constexpr (T == BufferDataType::UNSIGNED_SHORT) {
    return sizeof(GLushort);
  } else if constexpr (T == BufferDataType::INT) {
    return sizeof(GLint);
  } else if constexpr (T == BufferDataType::UNSIGNED_INT) {
    return sizeof(GLuint);
  } else if constexpr (T == BufferDataType::FIXED) {
    return sizeof(GLfixed);
  } else if constexpr (T == BufferDataType::FLOAT) {
    return sizeof(GLfloat);
  } else if constexpr (T == BufferDataType::HALF_FLOAT) {
    return sizeof(GLhalf);
  } else if constexpr (T == BufferDataType::DOUBLE) {
    return sizeof(GLdouble);
  } else {
    static_assert(false, "This type is not yet supported");
    return 0;
  }
}

#endif
