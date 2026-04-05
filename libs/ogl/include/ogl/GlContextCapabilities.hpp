#ifndef OGL_GLCONTEXTCAPABILITIES_HPP
#define OGL_GLCONTEXTCAPABILITIES_HPP
#include <array>
#include <glad/glad.h>
#include <string>
#include <vector>

struct GlContextCapabilities
{
  GLint maxCombinedUniformBlocks = 0;
  GLint maxUniformBufferBindings = 0;
  GLint uboOffsetAlignment = 0;
  GLint maxUniformBlockSize = 0;

  GLint maxTextureSize = 0;
  GLint max3DTextureSize = 0;
  GLint maxArrayTextureLayers = 0;
  GLint maxCubeMapTextureSize = 0;
  GLint maxCombinedTextureImageUnits = 0;

  GLint maxColorAttachments = 0;
  GLint maxDrawBuffers = 0;
  GLint maxRenderbufferSize = 0;
  std::array<GLint, 2> maxFramebufferDims{};
  std::array<GLint, 2> maxViewportDims{};

  GLint maxVertexAttribs = 0;
  GLint maxVertexAttribBindings = 0;
  GLint maxVertexAttribStride = 0;

  std::string vendor{};
  std::string renderer{};
  std::string version{};
  std::string glslVersion{};
  std::vector<std::string> extensions{};

  static const GlContextCapabilities &getInstance();
};

#endif
