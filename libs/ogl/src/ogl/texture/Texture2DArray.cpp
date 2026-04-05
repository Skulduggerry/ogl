#include "ogl/texture/Texture2DArray.hpp"

#include "ogl/Logging.hpp"
#include "ogl/texture/Helper.hpp"

TextureLayerAttachment Texture2DArray::asLayerAttachment(const GLint level, const GLint layer) const
{ return { getId(), level, layer }; }

TextureLevelAttachment Texture2DArray::asLayeredAttachment(const GLint level) const { return { getId(), level }; }

void Texture2DArray::generateMipmap() const
{
  ASSERT(isAllocated());
  m_texture.generateMipmap();
}

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
  ASSERT(width > 0 && height > 0);
  ASSERT(mipLevels > 0);
  ASSERT(mipLevels <= calculateMipLevels(width, height));

  m_width = width;
  m_height = height;
  m_layerCount = layerCount;
  m_mipLevels = mipLevels;
  m_allocated = true;

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
  ASSERT(isAllocated());

  ASSERT(xOffset >= 0 && width > 0);
  ASSERT(yOffset >= 0 && height > 0);
  ASSERT(layerOffset >= 0 && layerCount > 0);
  ASSERT(0 <= mipLevel && mipLevel < m_mipLevels);

  const GLsizei levelWidth = std::max(1, m_width >> mipLevel);
  const GLsizei levelHeight = std::max(1, m_height >> mipLevel);
  ASSERT(xOffset + width <= levelWidth);
  ASSERT(yOffset + height <= levelHeight);
  ASSERT(layerOffset + layerCount <= m_layerCount);

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