#ifndef OGL_TEXTUREATTACHMENT_HPP
#define OGL_TEXTUREATTACHMENT_HPP

#include "TextureEnums.hpp"


#include <glad/glad.h>

struct TextureLevelAttachment
{
  GLuint m_textureId;
  GLint m_mipLevel;
};

struct TextureLayerAttachment
{
  GLuint m_textureId;
  GLint m_mipLevel;
  GLint m_layer;
};

struct TextureCubeFaceAttachment
{
  GLuint m_textureId;
  GLint m_mipLevel;
  CubeMapFaceIndex m_face;
};

#endif
