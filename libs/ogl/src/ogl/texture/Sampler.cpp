#include "ogl/texture/Sampler.hpp"
#include "ogl/Logging.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

static GLuint createSampler()
{
  GLuint id = 0;
  GLCall(glCreateSamplers(1, &id));
  return id;
}

Sampler::Sampler() noexcept : m_id(createSampler()) {}

Sampler::~Sampler() noexcept
{
  if (hasName()) { GLCall(glDeleteSamplers(1, &m_id)); }
}

Sampler::Sampler(Sampler &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Sampler &Sampler::operator=(Sampler &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteSamplers(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);

  return *this;
}

void Sampler::bindUnit(const GLuint unit) const { GLCall(glBindSampler(unit, m_id)); }

void Sampler::unbindUnit(const GLuint unit) { GLCall(glBindSampler(unit, 0)); }

void Sampler::minFilter(TextureMinFilter filter) const
{ GLCall(glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter))); }

void Sampler::magFilter(TextureMagFilter filter) const
{ GLCall(glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter))); }

void Sampler::wrap(const TextureWrap wrap) const
{
  wrapS(wrap);
  wrapT(wrap);
  wrapR(wrap);
}

void Sampler::wrapS(TextureWrap wrap) const
{ GLCall(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap))); }

void Sampler::wrapT(TextureWrap wrap) const
{ GLCall(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap))); }

void Sampler::wrapR(TextureWrap wrap) const
{ GLCall(glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap))); }

void Sampler::borderColor(const glm::vec4 &color) const
{ GLCall(glSamplerParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color))); }

void Sampler::minLod(const float v) const { GLCall(glSamplerParameterf(m_id, GL_TEXTURE_MIN_LOD, v)); }

void Sampler::maxLod(const float v) const { GLCall(glSamplerParameterf(m_id, GL_TEXTURE_MAX_LOD, v)); }

void Sampler::lodBias(const float v) const { GLCall(glSamplerParameterf(m_id, GL_TEXTURE_LOD_BIAS, v)); }

void Sampler::debugLabel(const std::string_view name) const
{ GLCall(glObjectLabel(GL_SAMPLER, m_id, static_cast<GLsizei>(name.size()), name.data())); }