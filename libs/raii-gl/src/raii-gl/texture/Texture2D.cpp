#include "raii-gl/texture/Texture2D.hpp"
#include "raii-gl/Logging.hpp"
#include "raii-gl/texture/TextureHelper.hpp"

void Texture2D::generateMipmap() const
{
  ASSERT(isAllocated());
  m_texture.generateMipmap();
}

void Texture2D::storage(const InternalImageFormat internalFormat, const GLsizei width, const GLsizei height)
{ storage(calculateMipLevels(width, height), internalFormat, width, height); }

void Texture2D::storage(const GLsizei mipLevels,
  const InternalImageFormat internalFormat,
  const GLsizei width,
  const GLsizei height)
{
  ASSERT(width > 0 && height > 0);
  ASSERT(mipLevels > 0);
  ASSERT(mipLevels <= calculateMipLevels(width, height));

  m_width = width;
  m_height = height;
  m_mipLevels = mipLevels;
  m_allocated = true;

  GLCall(glTextureStorage2D(getId(), mipLevels, static_cast<GLenum>(internalFormat), width, height));
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
  ASSERT(isAllocated());

  ASSERT(xOffset >= 0 && width > 0);
  ASSERT(yOffset >= 0 && height > 0);
  ASSERT(0 <= mipLevel && mipLevel < m_mipLevels);

  [[maybe_unused]] const GLsizei levelWidth = std::max(1, m_width >> mipLevel);
  [[maybe_unused]] const GLsizei levelHeight = std::max(1, m_height >> mipLevel);
  ASSERT(xOffset + width <= levelWidth);
  ASSERT(yOffset + height <= levelHeight);

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