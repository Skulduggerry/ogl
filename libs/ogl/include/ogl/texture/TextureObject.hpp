#ifndef OGL_TEXTUREOBJECT_HPP
#define OGL_TEXTUREOBJECT_HPP

#include "TextureEnums.hpp"
#include "ogl/NoCreate.hpp"

#include <glad/glad.h>
#include <glm/vec4.hpp>

enum struct TextureTarget : GLenum {
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
};

class TextureObject
{
  GLuint m_id;
  TextureTarget m_target;

public:
  explicit TextureObject(TextureTarget target) noexcept;

  explicit TextureObject(TextureTarget target, NoCreate_t) noexcept;

  ~TextureObject() noexcept;

  TextureObject(const TextureObject &other) = delete;

  TextureObject(TextureObject &&other) noexcept;

  TextureObject &operator=(const TextureObject &other) = delete;

  TextureObject &operator=(TextureObject &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] TextureTarget getTarget() const noexcept { return m_target; }

  void bindTextureUnit(GLuint unit) const;

  void generateMipmap() const;

  void borderColor(glm::vec4 color) const;

  void minFilter(TextureMinFilter filter) const;

  void magFilter(TextureMagFilter filter) const;

  void wrapS(TextureWrap wrap) const;

  void wrapT(TextureWrap wrap) const;

  void wrapR(TextureWrap wrap) const;
};

#endif
