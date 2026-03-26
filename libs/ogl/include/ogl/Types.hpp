#ifndef OGL_TYPES_HPP
#define OGL_TYPES_HPP

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

enum struct AttributeType : GLenum {
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

GLuint getSizeOfType(AttributeType dataType);

enum struct IndexType : GLenum {
  // all index data types supported by OpenGL
  // ========================================
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
};

#endif
