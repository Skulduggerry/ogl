#ifndef OGL_PROGRAM_HPP
#define OGL_PROGRAM_HPP

#include "Shader.hpp"
#include "raii-gl/detail/StringViewHash.hpp"

#include <glm/matrix.hpp>
#include <optional>
#include <span>
#include <string>

struct ProgramLinkResult
{
  bool ok;
  std::string log;
};

struct ShaderSourceInfo
{
  std::string path;
  ShaderType type;
};

class Program
{
  GLuint m_id;
  bool m_linked = false;
  // used for optimization
  mutable std::unordered_map<std::string, GLint, SvHash, SvEq> m_uniformLocationCache{};
  mutable std::unordered_map<std::string, GLuint, SvHash, SvEq> m_uniformBlockIndexCache{};

public:
  Program();
  ~Program();
  Program(const Program &other) = delete;
  Program(Program &&other) noexcept;
  Program &operator=(const Program &other) = delete;
  Program &operator=(Program &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }
  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }
  [[nodiscard]] bool isLinked() const noexcept { return m_linked; }

  void bind() const;
  static void unbind();

  [[nodiscard]] ProgramLinkResult link(std::span<const Shader *const> shaders);
  [[nodiscard]] ProgramLinkResult link(const Shader &vertex, const Shader &fragment);
  [[nodiscard]] ProgramLinkResult link(const Shader &compute);

  void setBool(std::string_view name, GLboolean value) const;
  void setInt(std::string_view name, GLint value) const;
  void setUnsigned(std::string_view name, GLuint value) const;
  void setFloat(std::string_view name, GLfloat value) const;

  void setVec2(std::string_view name, const glm::vec2 &value) const;
  void setVec3(std::string_view name, const glm::vec3 &value) const;
  void setVec4(std::string_view name, const glm::vec4 &value) const;

  void setIVec2(std::string_view name, const glm::ivec2 &value) const;
  void setIVec3(std::string_view name, const glm::ivec3 &value) const;
  void setIVec4(std::string_view name, const glm::ivec4 &value) const;

  void setUVec2(std::string_view name, const glm::uvec2 &value) const;
  void setUVec3(std::string_view name, const glm::uvec3 &value) const;
  void setUVec4(std::string_view name, const glm::uvec4 &value) const;

  void setMat2(std::string_view name, const glm::mat2 &value) const;
  void setMat3(std::string_view name, const glm::mat3 &value) const;
  void setMat4(std::string_view name, const glm::mat4 &value) const;

  void setMat2x3(std::string_view name, const glm::mat2x3 &value) const;
  void setMat3x2(std::string_view name, const glm::mat3x2 &value) const;
  void setMat2x4(std::string_view name, const glm::mat2x4 &value) const;
  void setMat4x2(std::string_view name, const glm::mat4x2 &value) const;
  void setMat3x4(std::string_view name, const glm::mat3x4 &value) const;
  void setMat4x3(std::string_view name, const glm::mat4x3 &value) const;

  void uniformBlockBinding(std::string_view uniformBlockName, GLuint bindingPoint) const;

  void debugLabel(std::string_view name) const;

  [[nodiscard]] static std::optional<Program> fromFile(const std::string &vertexPath,
    const std::string &fragmentPath,
    std::span<const ShaderSourceInfo> additional = {},
    const Replacements &replacements = {});

  [[nodiscard]] static std::optional<Program> fromFile(const std::string &computePath,
    const Replacements &replacements = {});

private:
  GLuint getUniformBlockIndex(std::string_view uniformBlockName) const;
  GLint getUniformLocation(std::string_view uniformName) const;
};

#endif// OGL_PROGRAM_HPP
