#ifndef OGL_TEXTURE_HPP
#define OGL_TEXTURE_HPP
#include <array>
#include <glad/glad.h>

class Framebuffer;

enum struct TextureMinFilter : GLint {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum struct TextureMagFilter : GLint {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
};

enum struct TextureWrap : GLint {
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  REPEAT = GL_REPEAT,
  MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
};

class TextureBase
{
protected:
  GLuint m_id;

  // don't allow construction of this base class
  explicit TextureBase(GLuint id);

public:
  // Base class needs virtual destructor
  virtual ~TextureBase();

  // prevent slicing and direct instantiation
  TextureBase(const TextureBase &other) = delete;
  TextureBase(TextureBase &&other) = delete;
  TextureBase &operator=(const TextureBase &other) = delete;
  TextureBase &operator=(TextureBase &&other) = delete;

  void bindTextureUnit(GLuint unit) const;

  // texture parameters
  void borderColor(std::array<GLfloat, 4> color) const;
  void minFilter(TextureMinFilter filter) const;
  void magFilter(TextureMagFilter filter) const;
  void textureWrapS(TextureWrap s) const;
  void textureWrapT(TextureWrap t) const;
  void textureWrapR(TextureWrap r) const;

  friend class Framebuffer;
};

#endif
