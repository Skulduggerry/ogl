#include "ogl/GlContextCapabilities.hpp"
#include "ogl/Logging.hpp"

static std::string getString(const GLenum name)
{
  GLCall(const GLubyte *s = glGetString(name));
  return s ? reinterpret_cast<const char *>(s) : std::string{};
}

static std::vector<std::string> getExtensions()
{
  GLint n = 0;
  GLCall(glGetIntegerv(GL_NUM_EXTENSIONS, &n));

  std::vector<std::string> extensions{};
  extensions.reserve(static_cast<std::size_t>(n));

  for (GLint i = 0; i < n; ++i) {
    GLCall(const GLubyte *s = glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
    if (s) extensions.emplace_back(reinterpret_cast<const char *>(s));
  }

  return extensions;
}

const GlContextCapabilities &GlContextCapabilities::getInstance()
{
  static const GlContextCapabilities instance = [] {
    GlContextCapabilities tmp{};

    GLCall(glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &tmp.maxCombinedUniformBlocks));
    GLCall(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &tmp.maxUniformBufferBindings));
    GLCall(glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp.maxUniformBlockSize));
    GLCall(glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &tmp.uboOffsetAlignment));

    GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tmp.maxTextureSize));
    GLCall(glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &tmp.max3DTextureSize));
    GLCall(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &tmp.maxArrayTextureLayers));
    GLCall(glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &tmp.maxCubeMapTextureSize));
    GLCall(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &tmp.maxCombinedTextureImageUnits));

    GLCall(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &tmp.maxColorAttachments));
    GLCall(glGetIntegerv(GL_MAX_DRAW_BUFFERS, &tmp.maxDrawBuffers));
    GLCall(glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &tmp.maxRenderbufferSize));
    GLCall(glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &tmp.maxFramebufferDims[0]));
    GLCall(glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &tmp.maxFramebufferDims[1]));
    GLCall(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &tmp.maxViewportDims[0]));

    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tmp.maxVertexAttribs));
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &tmp.maxVertexAttribBindings));
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &tmp.maxVertexAttribStride));

    tmp.vendor = getString(GL_VENDOR);
    tmp.renderer = getString(GL_RENDERER);
    tmp.version = getString(GL_VERSION);
    tmp.glslVersion = getString(GL_SHADING_LANGUAGE_VERSION);
    tmp.extensions = getExtensions();

    return tmp;
  }();

  return instance;
}