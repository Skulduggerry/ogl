#include "ogl/texture/TextureBase.hpp"

#include "ogl/Logging.hpp"

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/gtc/integer.hpp>

TextureBase::TextureBase(const GLuint id) : m_id(id) {}

TextureBase::~TextureBase() { GLCall(glDeleteTextures(1, &m_id)); }

void TextureBase::bindTextureUnit(const GLuint unit) const { GLCall(glBindTextureUnit(unit, m_id)); }

void TextureBase::borderColor(std::array<GLfloat, 4> color) const
{
  GLCall(glTextureParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, color.data()));
}

void TextureBase::minFilter(TextureMinFilter filter) const
{
  GLCall(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter)));
}

void TextureBase::magFilter(TextureMagFilter filter) const
{
  GLCall(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter)));
}

void TextureBase::textureWrapS(TextureWrap s) const
{
  GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(s)));
}

void TextureBase::textureWrapT(TextureWrap t) const
{
  GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(t)));
}

void TextureBase::textureWrapR(TextureWrap r) const
{
  GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, static_cast<GLint>(r)));
}

GLsizei TextureBase::calculateMipLevels(const GLsizei width, const GLsizei height)
{
  return static_cast<GLsizei>(floor(log2(std::max(width, height)))) + 1;
}
