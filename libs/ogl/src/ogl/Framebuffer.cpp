#include "ogl/Framebuffer.hpp"
#include "ogl/Logging.hpp"

#include <utility>

GLuint createFramebuffer()
{
  GLuint id = 0;
  GLCall(glCreateFramebuffers(1, &id));
  return id;
}

Framebuffer::Framebuffer() : m_id(createFramebuffer()) {}

Framebuffer::~Framebuffer()
{
  if (hasName()) { GLCall(glDeleteFramebuffers(1, &m_id)); }
}

Framebuffer::Framebuffer(Framebuffer &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteFramebuffers(1, &m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);
  return *this;
}

FramebufferStatus Framebuffer::getStatus(const FramebufferTarget target) const
{
  GLCall(GLenum result = glCheckNamedFramebufferStatus(m_id, static_cast<GLenum>(target)));
  return static_cast<FramebufferStatus>(result);
}

bool Framebuffer::isComplete(const FramebufferTarget target) const
{ return getStatus(target) == FramebufferStatus::FRAMEBUFFER_COMPLETE; }

void Framebuffer::drawBuffer(Attachment buffer) const
{ GLCall(glNamedFramebufferDrawBuffer(m_id, static_cast<GLenum>(buffer))); }

void Framebuffer::drawBuffers(const std::span<const Attachment> buffers) const
{
  if (constexpr size_t StackN = 8; buffers.size() <= StackN) {
    std::array<GLenum, StackN> tmp{};
    for (size_t i = 0; i < buffers.size(); ++i) tmp[i] = static_cast<GLenum>(buffers[i]);
    GLCall(glNamedFramebufferDrawBuffers(m_id, static_cast<GLsizei>(buffers.size()), tmp.data()));
    return;
  }

  std::vector<GLenum> tmp(buffers.size());
  for (size_t i = 0; i < buffers.size(); ++i) tmp[i] = static_cast<GLenum>(buffers[i]);
  GLCall(glNamedFramebufferDrawBuffers(m_id, static_cast<GLsizei>(tmp.size()), tmp.data()));
}

void Framebuffer::readBuffer(Attachment buffer) const
{ GLCall(glNamedFramebufferReadBuffer(m_id, static_cast<GLenum>(buffer))); }

void Framebuffer::attach(const Renderbuffer &renderbuffer, Attachment attachment) const
{
  GLCall(glNamedFramebufferRenderbuffer(m_id, static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.getId()));
}

void Framebuffer::attach(const TextureLevelAttachment &info, Attachment attachment) const
{
  ASSERT(info.m_textureId != 0);
  ASSERT(info.m_mipLevel >= 0);

  GLCall(glNamedFramebufferTexture(m_id, static_cast<GLenum>(attachment), info.m_textureId, info.m_mipLevel));
}

void Framebuffer::attach(const TextureLayerAttachment &info, Attachment attachment) const
{
  ASSERT(info.m_textureId != 0);
  ASSERT(info.m_mipLevel >= 0);
  ASSERT(info.m_layer >= 0);

  GLCall(glNamedFramebufferTextureLayer(
    m_id, static_cast<GLenum>(attachment), info.m_textureId, info.m_mipLevel, info.m_layer));
}

void Framebuffer::detach(Attachment attachment) const
{ GLCall(glNamedFramebufferTexture(m_id, static_cast<GLenum>(attachment), 0, 0)); }

void Framebuffer::bind(const FramebufferTarget target) const
{ GLCall(glBindFramebuffer(static_cast<GLenum>(target), m_id)); }

void Framebuffer::bindDefault(const FramebufferTarget target)
{ GLCall(glBindFramebuffer(static_cast<GLenum>(target), 0)); }

void Framebuffer::debugLabel(const std::string_view name) const
{ GLCall(glObjectLabel(GL_FRAMEBUFFER, m_id, name.size(), name.data())); }
