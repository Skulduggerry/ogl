#ifndef OGL_TEXTURE2D_HPP
#define OGL_TEXTURE2D_HPP
#include "TextureObject.hpp"
#include "ogl/ImageFormat.hpp"
#include "ogl/Types.hpp"

#include <span>

class Texture2D
{
  TextureObject<TextureTarget::TEXTURE_2D> m_texture{};
  GLsizei m_width = 0, m_height = 0, m_mipLevels = 0;
  bool m_allocated = false;

public:
  Texture2D() noexcept = default;

  Texture2D(const Texture2D &other) = delete;

  Texture2D(Texture2D &&other) noexcept = default;

  Texture2D &operator=(const Texture2D &other) = delete;

  Texture2D &operator=(Texture2D &&other) noexcept = default;

  [[nodiscard]] GLuint getId() const noexcept { return m_texture.getId(); }

  [[nodiscard]] bool hasName() const noexcept { return m_texture.hasName(); }

  [[nodiscard]] GLsizei getWidth() const noexcept { return m_width; }

  [[nodiscard]] GLsizei getHeight() const noexcept { return m_height; }

  [[nodiscard]] GLsizei getMipLevels() const noexcept { return m_mipLevels; }

  [[nodiscard]] bool isAllocated() const noexcept { return m_allocated; }

  void bindTextureUnit(const GLuint unit) const { m_texture.bindTextureUnit(unit); }

  void generateMipmap() const;

  void storage(InternalImageFormat format, GLsizei width, GLsizei height);

  void storage(GLsizei mipLevels, InternalImageFormat format, GLsizei width, GLsizei height);

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
