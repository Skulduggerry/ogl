#include "ogl/texture/TextureObject.hpp"

#include "ogl/Logging.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

static GLuint createTexture(TextureTarget target)
{
  GLuint id = 0;
  GLCall(glCreateTextures(static_cast<GLenum>(target), 1, &id));
  return id;
}

TextureObject::TextureObject(const TextureTarget target) noexcept : m_id(createTexture(target)), m_target(target) {}

TextureObject::TextureObject(const TextureTarget target, NoCreate_t) noexcept : m_id(0), m_target(target) {}

TextureObject::~TextureObject() noexcept
{
  if (m_id != 0) { GLCall(glDeleteTextures(1, &m_id)); }
}

TextureObject::TextureObject(TextureObject &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_target(other.m_target)
{}

TextureObject &TextureObject::operator=(TextureObject &&other) noexcept
{
  if (this == &other) { return *this; }

  if (this == &other) { return *this; }

  // release currently owned resource
  if (m_id != 0) { GLCall(glDeleteBuffers(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);
  m_target = other.m_target;

  return *this;

  return *this;
}

void TextureObject::bindTextureUnit(const GLuint unit) const { GLCall(glBindTextureUnit(unit, m_id)); }

void TextureObject::generateMipmap() const { GLCall(glGenerateTextureMipmap(m_id)); }

void TextureObject::borderColor(const glm::vec4 &color) const
{ GLCall(glTextureParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color))); }

void TextureObject::minFilter(const TextureMinFilter filter) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter))); }

void TextureObject::magFilter(const TextureMagFilter filter) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter))); }

void TextureObject::wrapS(const TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap))); }

void TextureObject::wrapT(TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap))); }

void TextureObject::wrapR(TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap))); }