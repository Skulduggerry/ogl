#ifndef OGL_INTERNALFORMATS_HPP
#define OGL_INTERNALFORMATS_HPP
#include <glad/glad.h>

enum struct InternalFormat : GLenum {
  // sized internal formats
  // ======================
  R8 = GL_R8,
  R8_SNORM = GL_R8_SNORM,


  // sized depth-component formats
  // =============================
  DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
  DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
  DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,

  // sized depth-stencil formats
  // ===========================
  DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
  DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,

  // sized stencil-only format
  // =========================
  STENCIL_INDEX8 = GL_STENCIL_INDEX8,
};

#endif
