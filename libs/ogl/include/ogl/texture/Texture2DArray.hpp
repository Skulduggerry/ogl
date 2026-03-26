#ifndef OGL_TEXTURE2DARRAY_HPP
#define OGL_TEXTURE2DARRAY_HPP

#include "TextureObject.hpp"
#include "ogl/ImageFormat.hpp"
#include "ogl/Types.hpp"

#include <span>

class Texture2DArray
{
  TextureObject m_texture{ TextureTarget::TEXTURE_2D_ARRAY };
  GLsizei m_width = 0, m_height = 0, m_layerCount = 0;

public:
  Texture2DArray() noexcept = default;

  explicit Texture2DArray(NoCreate_t) noexcept;

  Texture2DArray(const Texture2DArray &other) = delete;

  Texture2DArray(Texture2DArray &&other) noexcept = default;

  Texture2DArray &operator=(const Texture2DArray &other) = delete;

  Texture2DArray &operator=(Texture2DArray &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_texture.getId(); }

  [[nodiscard]] GLuint getWidth() const noexcept { return m_width; }

  [[nodiscard]] GLuint getHeight() const noexcept { return m_height; }

  [[nodiscard]] GLuint getLayerCount() const noexcept { return m_layerCount; }

  void bindTextureUnit(const GLuint unit) const { m_texture.bindTextureUnit(unit); }

  void minFilter(const TextureMinFilter filter) const { m_texture.minFilter(filter); }

  void magFilter(const TextureMagFilter filter) const { m_texture.magFilter(filter); }

  void wrap(const TextureWrap wrap) const
  {
    wrapS(wrap);
    wrapT(wrap);
    wrapR(wrap);
  }

  void wrapS(const TextureWrap wrap) const { m_texture.wrapS(wrap); }

  void wrapT(const TextureWrap wrap) const { m_texture.wrapT(wrap); }

  void wrapR(const TextureWrap wrap) const { m_texture.wrapR(wrap); }

  void storage(InternalImageFormat format, GLsizei width, GLsizei height, GLsizei layers);

  void storage(GLsizei mipLevels, InternalImageFormat format, GLsizei width, GLsizei height, GLsizei layerCount);

  void subImageBytes(GLint mipLevel,
    GLint xOffset,
    GLint yOffset,
    GLint layerOffset,
    GLsizei width,
    GLsizei height,
    GLsizei layerCount,
    ImageFormat format,
    ImageDataType type,
    std::span<const std::byte> pixels) const;
};

#endif
