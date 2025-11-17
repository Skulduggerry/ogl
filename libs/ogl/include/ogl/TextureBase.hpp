#ifndef OGL_TEXTURE_HPP
#define OGL_TEXTURE_HPP
#include "InternalFormats.hpp"
#include <glad/glad.h>

// TODO: think about cleaner way to implement

class Framebuffer;

enum struct TextureTarget : GLenum {
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

  // TODO add more genereal methods

};

#endif
