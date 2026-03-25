#include "ogl/texture/TextureObject.hpp"

#include "ogl/Logging.hpp"

static GLuint createTexture(TextureTarget target)
{
  GLuint id = 0;
  GLCall(glCreateTextures(static_cast<GLenum>(target), 1, &id));
  return id;
}

TextureObject::TextureObject(const TextureTarget target) noexcept : m_id(createTexture(target)), m_target(target) {}

TextureObject::~TextureObject()
{
  if (m_id != 0) { GLCall(glDeleteTextures(1, &m_id)); }
}