#include "ogl/texture/Texture2D.hpp"
#include "ogl/Logging.hpp"

#include <utility>

static GLuint createTexture()
{
  GLuint id = 0;
  GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &id));
  return id;
}

Texture2D::Texture2D() : TextureBase(createTexture()) {}

Texture2D::Texture2D(Texture2D &&other) noexcept
  : TextureBase(std::move(other)), m_format(other.m_format), m_mipLevelCount(other.m_mipLevelCount), m_width(other.m_width),
    m_height(other.m_height)
{}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  m_format = other.m_format;
  m_mipLevelCount = other.m_mipLevelCount;
  m_width = other.m_width;
  m_height = other.m_height;
  TextureBase::operator=(std::move(other));
  return *this;
}

void Texture2D::storage(const InternalFormat format, const GLsizei width, const GLsizei height)
{ storage(calculateMipLevels(width, height), format, width, height); }


void Texture2D::storage(const GLsizei mipLevelCount,
  const InternalFormat format,
  const GLsizei width,
  const GLsizei height)
{
  m_mipLevelCount = mipLevelCount;
  m_format = format;
  m_width = width;
  m_height = height;
  GLCall(glTextureStorage2D(m_id, mipLevelCount, static_cast<GLenum>(format), width, height));
}

void Texture2D::subImage(const GLint mipLevel,
  const GLint xOffset,
  const GLint yOffset,
  const GLsizei width,
  const GLsizei height,
  Format format,
  ImageDataType type,
  const void *pixels) const
{
  GLCall(glTextureSubImage2D(
    m_id, mipLevel, xOffset, yOffset, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), pixels));
}
