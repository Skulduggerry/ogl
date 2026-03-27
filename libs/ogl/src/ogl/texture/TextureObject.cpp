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

template<TextureTarget TARGET> TextureObject<TARGET>::TextureObject() noexcept : m_id(createTexture(TARGET)) {}

template<TextureTarget TARGET> TextureObject<TARGET>::TextureObject(NoCreate_t) noexcept : m_id(0) {}

template<TextureTarget TARGET> TextureObject<TARGET>::~TextureObject() noexcept
{
  if (isValid()) { GLCall(glDeleteTextures(1, &m_id)); }
}

template<TextureTarget TARGET>
TextureObject<TARGET>::TextureObject(TextureObject &&other) noexcept : m_id(std::exchange(other.m_id, 0))
{}

template<TextureTarget TARGET> TextureObject<TARGET> &TextureObject<TARGET>::operator=(TextureObject &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (isValid()) { GLCall(glDeleteTextures(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);

  return *this;
}

template<TextureTarget TARGET> void TextureObject<TARGET>::bindTextureUnit(const GLuint unit) const
{ GLCall(glBindTextureUnit(unit, m_id)); }

template<TextureTarget TARGET> void TextureObject<TARGET>::generateMipmap() const
{ GLCall(glGenerateTextureMipmap(m_id)); }

template<TextureTarget TARGET> void TextureObject<TARGET>::borderColor(const glm::vec4 &color) const
{ GLCall(glTextureParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color))); }

template<TextureTarget TARGET> void TextureObject<TARGET>::minFilter(const TextureMinFilter filter) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter))); }

template<TextureTarget TARGET> void TextureObject<TARGET>::magFilter(const TextureMagFilter filter) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter))); }

template<TextureTarget TARGET> void TextureObject<TARGET>::wrapS(const TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap))); }

template<TextureTarget TARGET> void TextureObject<TARGET>::wrapT(TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap))); }

template<TextureTarget TARGET> void TextureObject<TARGET>::wrapR(TextureWrap wrap) const
{ GLCall(glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap))); }