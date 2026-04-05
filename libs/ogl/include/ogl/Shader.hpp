#ifndef OGL_SHADER_HPP
#define OGL_SHADER_HPP

#include "ogl/StringViewHash.hpp"

#include <glad/glad.h>
#include <string>
#include <unordered_map>

enum struct ShaderType : GLenum {
  // the two basic types of shaders we always need
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER,

  // additional types of shaders
  TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
  EVALUATION_CONTROL = GL_TESS_EVALUATION_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,

  // a different type of shader
  COMPUTE = GL_COMPUTE_SHADER,
};

struct ShaderSource
{
  bool ok;
  std::string path;
  std::string code;
  std::string log;
};

struct ShaderCompileResult
{
  bool ok;
  std::string log;
};

using Replacements = std::unordered_map<std::string, std::string, SvHash, SvEq>;

class Shader
{
  GLuint m_id;
  ShaderType m_type;
  bool m_compiled = false;

public:
  explicit Shader(ShaderType type);
  ~Shader();
  Shader(const Shader &other) = delete;
  Shader(Shader &&other) noexcept;

  Shader &operator=(const Shader &other) = delete;
  Shader &operator=(Shader &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }

  [[nodiscard]] bool isCompiled() const noexcept { return m_compiled; }

  [[nodiscard]] static ShaderSource loadSource(std::string_view path, const Replacements &replacements);

  [[nodiscard]] ShaderCompileResult compile(std::string_view code);

  void debugLabel(std::string_view name) const;
};

#endif// OGL_SHADER_HPP
