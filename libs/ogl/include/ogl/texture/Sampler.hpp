#ifndef OGL_SAMPLER_HPP
#define OGL_SAMPLER_HPP

#include "TextureEnums.hpp"

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <string_view>

class Sampler
{
  GLuint m_id;

public:
  Sampler() noexcept;
  ~Sampler() noexcept;

  Sampler(const Sampler &other) = delete;
  Sampler &operator=(const Sampler &other) = delete;

  Sampler(Sampler &&other) noexcept;
  Sampler &operator=(Sampler &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }

  void bindUnit(GLuint unit) const;
  static void unbindUnit(GLuint unit);

  void minFilter(TextureMinFilter filter) const;
  void magFilter(TextureMagFilter filter) const;

  void wrap(TextureWrap wrap) const;
  void wrapS(TextureWrap wrap) const;
  void wrapT(TextureWrap wrap) const;
  void wrapR(TextureWrap wrap) const;

  void borderColor(const glm::vec4 &color) const;

  void minLod(float v) const;
  void maxLod(float v) const;
  void lodBias(float v) const;

  void debugLabel(std::string_view name) const;
};

#endif
