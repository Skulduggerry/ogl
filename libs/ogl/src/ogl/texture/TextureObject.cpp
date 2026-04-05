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

template<TextureTarget TARGET> TextureObject<TARGET>::~TextureObject() noexcept
{
  if (hasName()) { GLCall(glDeleteTextures(1, &m_id)); }
}

template<TextureTarget TARGET>
TextureObject<TARGET>::TextureObject(TextureObject &&other) noexcept : m_id(std::exchange(other.m_id, 0))
{}

template<TextureTarget TARGET> TextureObject<TARGET> &TextureObject<TARGET>::operator=(TextureObject &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteTextures(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);

  return *this;
}

template<TextureTarget TARGET> void TextureObject<TARGET>::bindTextureUnit(const GLuint unit) const
{ GLCall(glBindTextureUnit(unit, m_id)); }

template<TextureTarget TARGET> void TextureObject<TARGET>::generateMipmap() const
{ GLCall(glGenerateTextureMipmap(m_id)); }

template<TextureTarget TARGET> void TextureObject<TARGET>::debugLabel(const std::string_view name) const
{ GLCall(glObjectLabel(GL_TEXTURE, m_id, static_cast<GLsizei>(name.size()), name.data())); }