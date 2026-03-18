#include "ogl/GlLimits.hpp"

#include "ogl/Logging.hpp"

const GlLimits &GlLimits::getInstance()
{
  static const GlLimits instance = [] {
    GlLimits tmp{};
    GLCall(glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp.maxUniformBlockSize));
    GLCall(glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &tmp.uboOffsetAlignment));
    return tmp;
  }();

  return instance;
}