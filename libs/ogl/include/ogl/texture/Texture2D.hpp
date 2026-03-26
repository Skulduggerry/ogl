#ifndef OGL_TEXTURE2D_HPP
#define OGL_TEXTURE2D_HPP
#include "TextureObject.hpp"
#include "ogl/ImageFormat.hpp"
#include "ogl/Types.hpp"

#include <span>

class Texture2D
{
  TextureObject m_texture{ TextureTarget::TEXTURE_2D };
  GLsizei m_width = 0, m_height = 0;

public:
  Texture2D() noexcept = default;

  explicit Texture2D(NoCreate_t);

  Texture2D(const Texture2D &other) = delete;

  Texture2D(Texture2D &&other) noexcept = default;

  Texture2D &operator=(const Texture2D &other) = delete;

  Texture2D &operator=(Texture2D &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_texture.getId(); }

  [[nodiscard]] GLsizei getWidth() const noexcept { return m_width; }

  [[nodiscard]] GLsizei getHeight() const noexcept { return m_height; }

  void bindTextureUnit(const GLuint unit) const { m_texture.bindTextureUnit(unit); }

  void generateMipmap() const { m_texture.generateMipmap(); }

  void borderColor(const glm::vec4 &color) const { m_texture.borderColor(color); }

  void minFilter(const TextureMinFilter filter) const { m_texture.minFilter(filter); }

  void magFilter(const TextureMagFilter filter) const { m_texture.magFilter(filter); }

  void wrap(const TextureWrap wrap) const
  {
    wrapS(wrap);
    wrapT(wrap);
  }

  void wrapS(const TextureWrap wrap) const { m_texture.wrapS(wrap); }

  void wrapT(const TextureWrap wrap) const { m_texture.wrapT(wrap); }

  void storage(InternalImageFormat format, GLsizei width, GLsizei height);

  void storage(GLsizei levels, InternalImageFormat format, GLsizei width, GLsizei height);

  void subImageBytes(GLint mipLevel,
    GLint xOffset,
    GLint yOffset,
    GLsizei width,
    GLsizei height,
    ImageFormat format,
    ImageDataType type,
    std::span<const std::byte> pixels) const;
};

#endif
