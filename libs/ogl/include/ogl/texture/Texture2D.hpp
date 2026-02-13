#ifndef OGL_TEXTURE2D_HPP
#define OGL_TEXTURE2D_HPP

#include "TextureBase.hpp"
#include "ogl/DataType.hpp"
#include "ogl/ImageFormat.hpp"

class Texture2D final : public TextureBase
{
  InternalFormat m_format = InternalFormat::RGB16F;
  GLsizei m_mipLevelCount = 0;
  GLsizei m_width = 0, m_height = 0;

public:
  Texture2D();
  ~Texture2D() override = default;
  Texture2D(const Texture2D &other) = delete;
  Texture2D(Texture2D &&other) noexcept;

  Texture2D &operator=(const Texture2D &other) = delete;
  Texture2D &operator=(Texture2D &&other) noexcept;

  void storage(InternalFormat format, GLsizei width, GLsizei height);
  void storage(GLsizei mipLevelCount, InternalFormat format, GLsizei width, GLsizei height);
  void subImage(GLint mipLevel,
    GLint xOffset,
    GLint yOffset,
    GLsizei width,
    GLsizei height,
    Format format,
    ImageDataType type,
    const void *pixels) const;
};

#endif
