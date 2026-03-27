#include "ogl/texture/Texture2DArray.hpp"

#include "ogl/Logging.hpp"
#include "ogl/texture/Helper.hpp"

Texture2DArray::Texture2DArray(NoCreate_t) noexcept : m_texture(NoCreate) {}

void Texture2DArray::storage(const InternalImageFormat format,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layers)
{ storage(calculateMipLevels(width, height), format, width, height, layers); }

void Texture2DArray::storage(const GLsizei mipLevels,
  const InternalImageFormat format,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layerCount)
{
  m_width = width;
  m_height = height;
  m_layerCount = layerCount;
  GLCall(glTextureStorage3D(getId(), mipLevels, static_cast<GLenum>(format), width, height, layerCount));
}

void Texture2DArray::subImageBytes(const GLint mipLevel,
  const GLint xOffset,
  const GLint yOffset,
  const GLint layerOffset,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layerCount,
  const ImageFormat format,
  const ImageDataType type,
  const std::span<const std::byte> pixels) const
{
  GLCall(glTextureSubImage3D(getId(),
    mipLevel,
    xOffset,
    yOffset,
    layerOffset,
    width,
    height,
    layerCount,
    static_cast<GLenum>(format),
    static_cast<GLenum>(type),
    pixels.data()));
}