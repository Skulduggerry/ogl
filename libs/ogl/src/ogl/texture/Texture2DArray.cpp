#include "ogl/texture/Texture2DArray.hpp"

#include "ogl/Logging.hpp"

#include <chrono>
#include <utility>

static GLuint createTexture()
{
  GLuint id = 0;
  GLCall(glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id));
  return id;
}

Texture2DArray::Texture2DArray() : TextureBase(createTexture()) {}

Texture2DArray::Texture2DArray(Texture2DArray &&other) noexcept
  : m_format(other.m_format), m_mipLevelCount(other.m_mipLevelCount), m_width(other.m_width), m_height(other.m_height),
    m_layerCount(other.m_layerCount), TextureBase(std::move(other))
{}

Texture2DArray &Texture2DArray::operator=(Texture2DArray &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  m_format = other.m_format;
  m_mipLevelCount = other.m_mipLevelCount;
  m_width = other.m_width;
  m_height = other.m_height;
  m_layerCount = other.m_layerCount;
  TextureBase::operator=(std::move(other));
  return *this;
}

void Texture2DArray::storage(const InternalFormat format,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layerCount)
{ storage(calculateMipLevels(width, height), format, width, height, layerCount); }

void Texture2DArray::storage(const GLsizei mipLevelCount,
  const InternalFormat format,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layerCount)
{
  m_format = format;
  m_mipLevelCount = mipLevelCount;
  m_width = width;
  m_height = height;
  m_layerCount = layerCount;

  GLCall(glTextureStorage3D(m_id, mipLevelCount, static_cast<GLenum>(format), width, height, layerCount));
}

void Texture2DArray::subImage(const GLint mipLevel,
  const GLint xOffset,
  const GLint yOffset,
  const GLint layerIndexOffset,
  const GLsizei width,
  const GLsizei height,
  const GLsizei layerCount,
  Format format,
  ImageDataType type,
  const void *pixels) const
{
  GLCall(glTextureSubImage3D(m_id,
    mipLevel,
    xOffset,
    yOffset,
    layerIndexOffset,
    width,
    height,
    layerCount,
    static_cast<GLenum>(format),
    static_cast<GLenum>(type),
    pixels));
}