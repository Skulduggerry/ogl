#ifndef OGL_TEXTURE2DARRAY_HPP
#define OGL_TEXTURE2DARRAY_HPP

#include "TextureBase.hpp"
#include "ogl/ImageFormat.hpp"


enum struct ImageDataType : GLenum;
class Texture2DArray final : public TextureBase
{
  InternalFormat m_format = InternalFormat::RGB16F;
  GLsizei m_mipLevelCount = 0;
  GLsizei m_width = 0, m_height = 0, m_layerCount = 0;

public:
  Texture2DArray();
  ~Texture2DArray() override = default;
  Texture2DArray(const Texture2DArray &other) = delete;
  Texture2DArray(Texture2DArray &&other) noexcept;

  Texture2DArray &operator=(const Texture2DArray &other) = delete;
  Texture2DArray &operator=(Texture2DArray &&other) noexcept;

  void storage(InternalFormat format, GLsizei width, GLsizei height, GLsizei layerCount);
  void storage(GLsizei mipLevelCount, InternalFormat format, GLsizei width, GLsizei height, GLsizei layerCount);
  void subImage(GLint mipLevel,
    GLint xOffset,
    GLint yOffset,
    GLint layerIndexOffset,
    GLsizei width,
    GLsizei height,
    GLsizei layerCount,
    Format format,
    ImageDataType type,
    const void *pixels) const;
};

#endif
