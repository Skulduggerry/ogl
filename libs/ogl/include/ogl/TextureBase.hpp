#ifndef OGL_TEXTURE_HPP
#define OGL_TEXTURE_HPP
#include <glad/glad.h>

class Framebuffer;

enum struct TextureTarget : GLenum {
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
};

enum struct TextureParameter: GLenum
{
  DEPTH_STENCIL_TEXTURE_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE,

};

enum struct TextureParameterValues: GLenum
{

};

class TextureBase
{
protected:
  GLuint m_id;
  TextureTarget m_target;

  // don't allow construction of this base class
  TextureBase(GLuint id, TextureTarget target);

public:
  // Base class needs virtual destructor
  virtual ~TextureBase();

  // prevent slicing and direct instantiation
  TextureBase(const TextureBase &other) = delete;
  TextureBase(TextureBase &&other) = delete;
  TextureBase &operator=(const TextureBase &other) = delete;
  TextureBase &operator=(TextureBase &&other) = delete;

  // TODO add more general methods
  void bindTextureUnit(GLuint unit);
  void generateTextureMipmap();

  friend class Framebuffer;
};

class Texture2D final : TextureBase
{
public:
  Texture2D();
  ~Texture2D() override;
  Texture2D(const Texture2D &other) = delete;
  Texture2D(Texture2D &&other) noexcept;

  Texture2D &operator=(const Texture2D &other) = delete;
  Texture2D &operator=(Texture2D &&other) noexcept;
};

#endif
