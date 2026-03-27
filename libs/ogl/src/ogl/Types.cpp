#include "ogl/Types.hpp"
#include "ogl/Logging.hpp"

#include <utility>

GLint getSizeOfType(const AttributeType dataType)
{
  switch (dataType) {
  case AttributeType::BYTE:
    return sizeof(GLbyte);
  case AttributeType::UNSIGNED_BYTE:
    return sizeof(GLubyte);
  case AttributeType::SHORT:
    return sizeof(GLshort);
  case AttributeType::UNSIGNED_SHORT:
    return sizeof(GLushort);
  case AttributeType::INT:
    return sizeof(GLint);
  case AttributeType::UNSIGNED_INT:
    return sizeof(GLuint);
  case AttributeType::FIXED:
    return sizeof(GLfixed);
  case AttributeType::FLOAT:
    return sizeof(GLfloat);
  case AttributeType::HALF_FLOAT:
    return sizeof(GLhalf);
  case AttributeType::DOUBLE:
    return sizeof(GLdouble);
  }
  std::unreachable();
}