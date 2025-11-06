#ifndef OGL_TEXTURE_HPP
#define OGL_TEXTURE_HPP
#include "InternalFormats.hpp"
#include <glad/glad.h>

// TODO: think about cleaner way to implement

class Framebuffer;

enum struct TextureTargets : GLenum {
  TEXTURE_1D = GL_TEXTURE_1D,
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_3D = GL_TEXTURE_3D,
  TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
  TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
  TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
  TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

template<TextureTargets TARGET>
class Texture
{
  GLuint m_id;

  InternalFormat m_format = InternalFormat::DEPTH_COMPONENT32F;
  GLsizei m_width = 0;
  GLsizei m_height = 0;

public:
  Texture();
  Texture(InternalFormat format, GLsizei width, GLsizei height);
  ~Texture();
  Texture(const Texture &other) = delete;
  Texture(Texture &&other) noexcept;

  Texture &operator=(const Texture &other) = delete;
  Texture &operator=(Texture &&other) noexcept;

  void storage(InternalFormat format, GLsizei width, GLsizei height);

  [[nodiscard]] InternalFormat getFormat() const { return m_format; }
  [[nodiscard]] GLsizei getWidth() const { return m_width; }
  [[nodiscard]] GLsizei getHeight() const { return m_height; }

  friend Framebuffer;
};

#endif
