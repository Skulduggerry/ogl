#ifndef OGL_LOGGING_HPP
#define OGL_LOGGING_HPP

#include <fmt/base.h>
#include <glad/glad.h>

#ifdef __GNUG__
#define DEBUG_BREAK() __builtin_trap()
#elifdef WIN32
#define DEBUG_BREAK() __debugbreak()
#endif

#ifdef DEVELOPER_BUILD

#define ASSERT(x) \
if (!(x)) { DEBUG_BREAK(); }

#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#define ASSERT(x) ;
#endif

inline void GLClearError()
{
  while (glGetError() != GL_NO_ERROR) { ; }
}

inline const char *GLErrorToString(const GLenum error)
{
  switch (error) {
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_STACK_OVERFLOW:
    return "GL_STACK_OVERFLOW";
  case GL_STACK_UNDERFLOW:
    return "GL_STACK_UNDERFLOW";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_CONTEXT_LOST:
    return "GL_CONTEXT_LOST";
  default:
    return "unknown";
  }
}

inline bool GLLogCall(const char *function, const char *file, int line)
{
  bool noError = true;
  while (const GLenum error = glGetError()) {
    fmt::println(stderr, "[OpenGL Error] ({}): {} {} {}", GLErrorToString(error), function, file, line);
    noError = false;
  }
  return noError;
}

#endif// OGL_LOGGING_HPP
