#ifndef OGL_SHADER_HPP
#define OGL_SHADER_HPP

#include "raii-gl/detail/StringViewHash.hpp"

#include <expected>
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

using Replacements = std::unordered_map<std::string, std::string, SvHash, SvEq>;

class Shader
{
  GLuint m_id;
  ShaderType m_type;
  bool m_compiled = false;

public:
  struct Source
  {
    std::string path;
    std::string code;
  };

  struct LoadError
  {
    std::string path;
    std::string log;
  };

  struct CompileError
  {
    ShaderType type;
    std::string log;
  };

  explicit Shader(ShaderType type);
  ~Shader();
  Shader(const Shader &other) = delete;
  Shader(Shader &&other) noexcept;

  Shader &operator=(const Shader &other) = delete;
  Shader &operator=(Shader &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }
  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }
  [[nodiscard]] bool isCompiled() const noexcept { return m_compiled; }

  [[nodiscard]] static std::expected<Source, LoadError> loadSource(std::string_view path,
    const Replacements &replacements);
  [[nodiscard]] std::expected<void, CompileError> compile(std::string_view code);

  void debugLabel(std::string_view name) const;
};

#endif// OGL_SHADER_HPP
