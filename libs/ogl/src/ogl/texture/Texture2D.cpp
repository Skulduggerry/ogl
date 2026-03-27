#include "ogl/texture/Texture2D.hpp"

#include "ogl/Logging.hpp"
#include "ogl/texture/Helper.hpp"

Texture2D::Texture2D(NoCreate_t) : m_texture(NoCreate) {}

void Texture2D::storage(const InternalImageFormat format, const GLsizei width, const GLsizei height)
{ storage(calculateMipLevels(width, height), format, width, height); }

void Texture2D::storage(const GLsizei mipLevels,
  const InternalImageFormat format,
  const GLsizei width,
  const GLsizei height)
{
  m_width = width;
  m_height = height;
  m_mipLevels = mipLevels;

  GLCall(glTextureStorage2D(getId(), mipLevels, static_cast<GLenum>(format), width, height));
}

void Texture2D::subImageBytes(const GLint mipLevel,
  const GLint xOffset,
  const GLint yOffset,
  const GLsizei width,
  const GLsizei height,
  const ImageFormat format,
  const ImageDataType type,
  const std::span<const std::byte> pixels) const
{
  GLCall(glTextureSubImage2D(getId(),
    mipLevel,
    xOffset,
    yOffset,
    width,
    height,
    static_cast<GLenum>(format),
    static_cast<GLenum>(type),
    pixels.data()));
}