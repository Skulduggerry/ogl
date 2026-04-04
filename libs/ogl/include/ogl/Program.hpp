#ifndef OGL_PROGRAM_HPP
#define OGL_PROGRAM_HPP
#include "Shader.hpp"


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <unordered_map>

class Program
{
  // the program id
  GLuint m_id;
  // used for optimization
  mutable std::unordered_map<std::string, GLint> m_uniformLocationCache{};

public:
  struct ShaderSourceInfo
  {
    std::string path;
    Shader::Type type;
  };

  Program(const std::string &vertexPath,
    const std::string &fragmentPath,
    const std::vector<ShaderSourceInfo> &additional = {},
    const std::map<std::string, std::string> &replacements = {});
  explicit Program(const std::string &computePath, const std::map<std::string, std::string> &replacements = {});

  ~Program();
  Program(const Program &other) = delete;
  Program(Program &&other) noexcept;

  Program &operator=(const Program &other) = delete;
  Program &operator=(Program &&other) noexcept;

  [[nodiscard]] GLuint getId() const noexcept { return m_id; }

  [[nodiscard]] bool hasName() const noexcept { return m_id != 0; }

  void bind() const;
  static void unbind();

  void setBool(const std::string &name, GLboolean value) const;
  void setInt(const std::string &name, GLint value) const;
  void setUnsigned(const std::string &name, GLuint value) const;
  void setFloat(const std::string &name, GLfloat value) const;

  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;

  void setIVec2(const std::string &name, const glm::ivec2 &value) const;
  void setIVec3(const std::string &name, const glm::ivec3 &value) const;
  void setIVec4(const std::string &name, const glm::ivec4 &value) const;

  void setUVec2(const std::string &name, const glm::uvec2 &value) const;
  void setUVec3(const std::string &name, const glm::uvec3 &value) const;
  void setUVec4(const std::string &name, const glm::uvec4 &value) const;

  void setMat2(const std::string &name, const glm::mat2 &value) const;
  void setMat3(const std::string &name, const glm::mat3 &value) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;

  void setMat2x3(const std::string &name, const glm::mat2x3 &value) const;
  void setMat3x2(const std::string &name, const glm::mat3x2 &value) const;
  void setMat2x4(const std::string &name, const glm::mat2x4 &value) const;
  void setMat4x2(const std::string &name, const glm::mat4x2 &value) const;
  void setMat3x4(const std::string &name, const glm::mat3x4 &value) const;
  void setMat4x3(const std::string &name, const glm::mat4x3 &value) const;

  void uniformBlockBinding(const std::string &uniformBlockName, GLuint bindingPoint) const;

private:
  GLuint getUniformBlockIndex(const std::string &uniformBlockName) const;
  GLint getUniformLocation(const std::string &name) const;
};

#endif// OGL_PROGRAM_HPP
