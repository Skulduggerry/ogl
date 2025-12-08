#include "ogl/texture/Texture2D.hpp"
#include "ogl/Logging.hpp"

#include <utility>

GLuint createTexture()
{
  GLuint id = 0;
  glCreateTextures(GL_TEXTURE_2D, 1, &id);
  return id;
}

Texture2D::Texture2D() : TextureBase(createTexture()) {}

Texture2D::Texture2D(Texture2D &&other) noexcept
  : TextureBase(std::exchange(other.m_id, 0)), m_format(other.m_format), m_levels(other.m_levels),
    m_width(other.m_width), m_height(other.m_height)
{}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  swap(m_format, other.m_format);
  swap(m_levels, other.m_levels);
  swap(m_width, other.m_width);
  swap(m_height, other.m_height);

  return *this;
}

void Texture2D::storage(const GLsizei levels, InternalFormat format, const GLsizei width, const GLsizei height)
{
  m_levels = levels;
  m_format = format;
  m_width = width;
  m_height = height;
  GLCall(glTextureStorage2D(m_id, levels, static_cast<GLenum>(format), width, height));
}

void Texture2D::subImage(const GLint level,
  const GLint xOffset,
  const GLint yOffset,
  const GLsizei width,
  const GLsizei height,
  Format format,
  DataType type,
  const void *pixels) const
{
  GLCall(glTextureSubImage2D(
    m_id, level, xOffset, yOffset, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), pixels));
}
