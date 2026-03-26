#ifndef OGL_GLLIMITS_HPP
#define OGL_GLLIMITS_HPP
#include <glad/glad.h>

struct GlLimits
{
  GLint uboOffsetAlignment = 0;
  GLint maxUniformBlockSize = 0;

  static const GlLimits &getInstance();
};

#endif
