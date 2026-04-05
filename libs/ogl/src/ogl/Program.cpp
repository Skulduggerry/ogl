#include "ogl/Program.hpp"
#include "ogl/Logging.hpp"
#include "ogl/Shader.hpp"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>
#include <utility>

GLuint createProgram()
{
  GLCall(const GLuint id = glCreateProgram());
  return id;
}

Program::Program(const std::string &vertexPath,
  const std::string &fragmentPath,
  const std::vector<ShaderSourceInfo> &additional,
  const std::map<std::string, std::string> &replacements)
  : m_id(createProgram())
{
  // create shaders and attach to the program
  const Shader vertexShader{ vertexPath, Shader::Type::VERTEX, replacements };
  const Shader fragmentShader{ fragmentPath, Shader::Type::FRAGMENT, replacements };
  const std::vector<Shader> additionalShaders = additional | std::views::transform([](const auto &information) {
    return Shader{ information.path, information.type };
  }) | std::ranges::to<std::vector<Shader>>();

  GLCall(glAttachShader(m_id, vertexShader.getId()));
  GLCall(glAttachShader(m_id, fragmentShader.getId()));
  for (const auto &shader : additionalShaders) { GLCall(glAttachShader(m_id, shader.getId())); }

  // link the program, check for errors
  GLCall(glLinkProgram(m_id));
  GLint result;
  GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &result));
  if (GL_FALSE == result) {
    GLint length;
    GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));

    std::string infoLog(static_cast<size_t>(length), 0);
    GLCall(glGetProgramInfoLog(m_id, length, &length, infoLog.data()));

    fmt::println(stderr, "[Program Error]: Failed to link program!");
    fmt::println(stderr, "{}", infoLog);
  }

  GLCall(glDetachShader(m_id, vertexShader.getId()));
  GLCall(glDetachShader(m_id, fragmentShader.getId()));
  for (const auto &shader : additionalShaders) { GLCall(glDetachShader(m_id, shader.getId())); }
}

Program::Program(const std::string &computePath, const std::map<std::string, std::string> &replacements)
  : m_id(createProgram())
{
  // create shaders and attach to the program
  const Shader computeShader{ computePath, Shader::Type::COMPUTE, replacements };

  GLCall(glAttachShader(m_id, computeShader.getId()));

  // link the program, check for errors
  GLCall(glLinkProgram(m_id));
  GLint result;
  GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &result));
  if (GL_FALSE == result) {
    GLint length;
    GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));

    std::string infoLog(static_cast<size_t>(length), 0);
    GLCall(glGetProgramInfoLog(m_id, length, &length, infoLog.data()));

    fmt::println(stderr, "[Program Error]: Failed to link program!");
    fmt::println(stderr, "{}", infoLog);
  }

  GLCall(glDetachShader(m_id, computeShader.getId()));
}

Program::~Program()
{
  if (hasName()) { GLCall(glDeleteProgram(m_id)); }
}

Program::Program(Program &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_uniformLocationCache(std::move(other.m_uniformLocationCache))
{}

Program &Program::operator=(Program &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteProgram(m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);

  return *this;
}

void Program::bind() const { GLCall(glUseProgram(m_id)); }

void Program::unbind() { GLCall(glUseProgram(0)); }

void Program::setBool(const std::string &name, const GLboolean value) const
{ GLCall(glProgramUniform1i(m_id, getUniformLocation(name), value)); }

void Program::setInt(const std::string &name, const GLint value) const
{ GLCall(glProgramUniform1i(m_id, getUniformLocation(name), value)); }

void Program::setUnsigned(const std::string &name, const GLuint value) const
{ GLCall(glProgramUniform1ui(m_id, getUniformLocation(name), value)); }

void Program::setFloat(const std::string &name, const GLfloat value) const
{ GLCall(glProgramUniform1f(m_id, getUniformLocation(name), value)); }

void Program::setVec2(const std::string &name, const glm::vec2 &value) const
{ GLCall(glProgramUniform2fv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setVec3(const std::string &name, const glm::vec3 &value) const
{ GLCall(glProgramUniform3fv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setVec4(const std::string &name, const glm::vec4 &value) const
{ GLCall(glProgramUniform4fv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setIVec2(const std::string &name, const glm::ivec2 &value) const
{ GLCall(glProgramUniform2iv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setIVec3(const std::string &name, const glm::ivec3 &value) const
{ GLCall(glProgramUniform3iv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setIVec4(const std::string &name, const glm::ivec4 &value) const
{ GLCall(glProgramUniform4iv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setUVec2(const std::string &name, const glm::uvec2 &value) const
{ GLCall(glProgramUniform2uiv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setUVec3(const std::string &name, const glm::uvec3 &value) const
{ GLCall(glProgramUniform3uiv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setUVec4(const std::string &name, const glm::uvec4 &value) const
{ GLCall(glProgramUniform4uiv(m_id, getUniformLocation(name), 1, glm::value_ptr(value))); }

void Program::setMat2(const std::string &name, const glm::mat2 &value) const
{ GLCall(glProgramUniformMatrix2fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat3(const std::string &name, const glm::mat3 &value) const
{ GLCall(glProgramUniformMatrix3fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat4(const std::string &name, const glm::mat4 &value) const
{ GLCall(glProgramUniformMatrix4fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat2x3(const std::string &name, const glm::mat2x3 &value) const
{ GLCall(glProgramUniformMatrix2x3fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat3x2(const std::string &name, const glm::mat3x2 &value) const
{ GLCall(glProgramUniformMatrix3x2fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat2x4(const std::string &name, const glm::mat2x4 &value) const
{ GLCall(glProgramUniformMatrix2x4fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat4x2(const std::string &name, const glm::mat4x2 &value) const
{ GLCall(glProgramUniformMatrix4x2fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat3x4(const std::string &name, const glm::mat3x4 &value) const
{ GLCall(glProgramUniformMatrix3x4fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::setMat4x3(const std::string &name, const glm::mat4x3 &value) const
{ GLCall(glProgramUniformMatrix4x3fv(m_id, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value))); }

void Program::uniformBlockBinding(const std::string &uniformBlockName, const GLuint bindingPoint) const
{ GLCall(glUniformBlockBinding(m_id, getUniformBlockIndex(uniformBlockName), bindingPoint)); }

GLuint Program::getUniformBlockIndex(const std::string &uniformBlockName) const
{
  GLCall(const GLuint index = glGetUniformBlockIndex(m_id, uniformBlockName.c_str()));
  return index;
}

GLint Program::getUniformLocation(const std::string &name) const
{
  if (const auto location = m_uniformLocationCache.find(name); m_uniformLocationCache.end() != location) {
    return location->second;
  }

  GLCall(GLint location = glGetUniformLocation(m_id, name.c_str()));

  if (-1 == location) { fmt::println(stderr, "Warning: uniform '{}' does not exist!", name); }

  m_uniformLocationCache.insert({ name, location });
  return location;
}
