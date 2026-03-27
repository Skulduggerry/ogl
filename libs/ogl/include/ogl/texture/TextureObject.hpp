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

template<TextureTarget> class TextureObject
{
  GLuint m_id;

public:
  explicit TextureObject() noexcept;

  explicit TextureObject(NoCreate_t) noexcept;

  ~TextureObject() noexcept;

  TextureObject(const TextureObject &other) = delete;

  TextureObject(TextureObject &&other) noexcept;

  TextureObject &operator=(const TextureObject &other) = delete;

  TextureObject &operator=(TextureObject &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] GLboolean isValid() const noexcept { return m_id != 0; }

  void bindTextureUnit(GLuint unit) const;

  void generateMipmap() const;

  void borderColor(const glm::vec4 &color) const;

  void minFilter(TextureMinFilter filter) const;

  void magFilter(TextureMagFilter filter) const;

  void wrapS(TextureWrap wrap) const;

  void wrapT(TextureWrap wrap) const;

  void wrapR(TextureWrap wrap) const;
};

template class TextureObject<TextureTarget::TEXTURE_2D>;
template class TextureObject<TextureTarget::TEXTURE_2D_ARRAY>;

#endif
