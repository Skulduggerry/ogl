#ifndef OGL_IMAGEFORMATUTILS_HPP
#define OGL_IMAGEFORMATUTILS_HPP

#include "ImageFormat.hpp"
#include "Types.hpp"

#include <cstddef>
#include <optional>

constexpr int channelCount(const ImageFormat format)
{
  switch (format) {
  case ImageFormat::RED:
  case ImageFormat::DEPTH_COMPONENT:
  case ImageFormat::STENCIL_INDEX:
    return 1;
  case ImageFormat::RG:
    return 2;
  case ImageFormat::RGB:
  case ImageFormat::BGR:
    return 3;
  case ImageFormat::RGBA:
  case ImageFormat::BGRA:
    return 4;
  }
  return 0;
}

constexpr bool isColorFormat(const ImageFormat format)
{
  switch (format) {
  case ImageFormat::RED:
  case ImageFormat::RG:
  case ImageFormat::RGB:
  case ImageFormat::BGR:
  case ImageFormat::RGBA:
  case ImageFormat::BGRA:
    return true;
  case ImageFormat::DEPTH_COMPONENT:
  case ImageFormat::STENCIL_INDEX:
    return false;
  }
  return false;
}

constexpr bool isDepthFormat(const ImageFormat format) { return format == ImageFormat::DEPTH_COMPONENT; }

constexpr bool isStencilFormat(const ImageFormat format) { return format == ImageFormat::STENCIL_INDEX; }

constexpr std::size_t bytesPerChannel(const ImageDataType type)
{
  switch (type) {
  case ImageDataType::UNSIGNED_BYTE:
  case ImageDataType::BYTE:
    return sizeof(GLbyte);
  case ImageDataType::UNSIGNED_SHORT:
  case ImageDataType::SHORT:
    return sizeof(GLshort);
  case ImageDataType::UNSIGNED_INT:
  case ImageDataType::INT:
    return sizeof(GLint);
  case ImageDataType::FLOAT:
    return sizeof(GLfloat);
  case ImageDataType::UNSIGNED_BYTE_3_3_2:
  case ImageDataType::UNSIGNED_BYTE_2_3_3_REV:
    return sizeof(GLubyte);
  case ImageDataType::UNSIGNED_SHORT_5_6_5:
  case ImageDataType::UNSIGNED_SHORT_5_6_5_REV:
  case ImageDataType::UNSIGNED_SHORT_4_4_4_4:
  case ImageDataType::UNSIGNED_SHORT_4_4_4_4_REV:
  case ImageDataType::UNSIGNED_SHORT_5_5_5_1:
  case ImageDataType::UNSIGNED_SHORT_1_5_5_5_REV:
    return sizeof(GLushort);
  case ImageDataType::UNSIGNED_INT_8_8_8_8:
  case ImageDataType::UNSIGNED_INT_8_8_8_8_REV:
  case ImageDataType::UNSIGNED_INT_10_10_10_2:
  case ImageDataType::UNSIGNED_INT_2_10_10_10_REV:
    return sizeof(GLuint);
  }
  return 0;
}

#endif
