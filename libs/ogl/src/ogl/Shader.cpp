#include "ogl/Shader.hpp"

#include "ogl/Logging.hpp"

#include <fmt/base.h>
#include <fstream>
#include <sstream>
#include <utility>

std::string readFile(const std::string &path)
{
  std::ifstream shaderFile{ path, std::ios::in };

  if (!shaderFile.is_open()) {
    fmt::println(stderr, "[Shader Error]: Could not read file {}.", path);
    return "";
  }

  std::stringstream buffer{};
  buffer << shaderFile.rdbuf();
  return buffer.str();
}

GLuint createShader(Shader::Types type)
{
  GLCall(const GLuint id = glCreateShader(static_cast<std::underlying_type_t<Shader::Types>>(type)));
  return id;
}

Shader::Shader(const std::string &path, const Types type) : m_id(createShader(type))
{
  const std::string code = readFile(path);
  const char *src = code.c_str();
  const auto size = static_cast<GLint>(code.size());

  GLCall(glShaderSource(m_id, 1, &src, &size));
  GLCall(glCompileShader(m_id));

  GLint result;
  GLCall(glGetShaderiv(m_id, GL_COMPILE_STATUS, &result));
  if (GL_FALSE == result) {
    GLint length;
    GLCall(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length));

    std::string infoLog(length, 0);
    GLCall(glGetShaderInfoLog(m_id, length, &length, infoLog.data()));

    fmt::println(stderr, "[Shader Error]: Failed to compile shader! {}", path);
    fmt::println(stderr, "{}", infoLog);
  }
}

Shader::~Shader() { GLCall(glDeleteShader(m_id)); }

Shader::Shader(Shader &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Shader &Shader::operator=(Shader &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}
