#ifndef OGL_TEXTURE2DARRAY_HPP
#define OGL_TEXTURE2DARRAY_HPP

#include "TextureAttachment.hpp"
#include "TextureObject.hpp"
#include "raii-gl/ImageFormat.hpp"
#include "raii-gl/Types.hpp"

#include <span>

class Texture2DArray
{
  TextureObject<TextureTarget::TEXTURE_2D_ARRAY> m_texture{};
  GLsizei m_width = 0, m_height = 0, m_layerCount = 0, m_mipLevels = 0;
  bool m_allocated = false;

public:
  Texture2DArray() noexcept = default;
  Texture2DArray(const Texture2DArray &other) = delete;
  Texture2DArray(Texture2DArray &&other) noexcept = default;
  Texture2DArray &operator=(const Texture2DArray &other) = delete;
  Texture2DArray &operator=(Texture2DArray &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_texture.getId(); }
  [[nodiscard]] bool hasName() const noexcept { return m_texture.hasName(); }
  [[nodiscard]] GLsizei getWidth() const noexcept { return m_width; }
  [[nodiscard]] GLsizei getHeight() const noexcept { return m_height; }
  [[nodiscard]] GLsizei getLayerCount() const noexcept { return m_layerCount; }
  [[nodiscard]] GLsizei getMipLevels() const noexcept { return m_mipLevels; }
  [[nodiscard]] bool isAllocated() const noexcept { return m_allocated; }

  [[nodiscard]] TextureLayerAttachment asLayerAttachment(GLint level, GLint layer) const;
  [[nodiscard]] TextureLevelAttachment asLayeredAttachment(GLint level = 0) const;

  void bindTextureUnit(const GLuint unit) const { m_texture.bindTextureUnit(unit); }

  void generateMipmap() const;

  void storage(InternalImageFormat internalFormat, GLsizei width, GLsizei height, GLsizei layers);
  void
    storage(GLsizei mipLevels, InternalImageFormat internalFormat, GLsizei width, GLsizei height, GLsizei layerCount);

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

  void debugLabel(const std::string_view name) const { m_texture.debugLabel(name); }
};

#endif
