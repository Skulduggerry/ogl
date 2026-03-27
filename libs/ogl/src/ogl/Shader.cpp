#include "ogl/Shader.hpp"
#include "ogl/Logging.hpp"

#include <fmt/base.h>
#include <fstream>
#include <regex>
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

std::string mapReplace(std::string text, const std::map<std::string, std::string> &replacements)
{
  if (replacements.empty()) return text;

  const std::string pattern_str =
    "\\{\\{\\s*("
    + (replacements | std::views::keys | std::views::join_with(std::string{ "|" }) | std::ranges::to<std::string>())
    + ")\\s*\\}\\}";
  const std::regex pattern(pattern_str);

  std::string result{};
  const auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
  const auto words_end = std::sregex_iterator();

  size_t last_pos = 0;
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    const std::smatch &match = *i;

    // Add the text before the match
    result += text.substr(last_pos, static_cast<size_t>(match.position()) - last_pos);

    // Find the replacement value (match[1] is the key inside the brackets)
    result += replacements.at(match[1].str());

    last_pos = static_cast<size_t>(match.position() + match.length());
  }

  // Add the remaining part of the string
  text = result + text.substr(last_pos);
  return text;
}

GLuint createShader(Shader::Type type)
{
  GLCall(const GLuint id = glCreateShader(static_cast<GLenum>(type)));
  return id;
}

Shader::Shader(const std::string &path, const Type type, const std::map<std::string, std::string> &replacements)
  : m_id(createShader(type))
{
  std::string code = readFile(path);
  code = mapReplace(code, replacements);

  const char *src = code.c_str();
  const auto size = static_cast<GLint>(code.size());

  GLCall(glShaderSource(m_id, 1, &src, &size));
  GLCall(glCompileShader(m_id));

  GLint result;
  GLCall(glGetShaderiv(m_id, GL_COMPILE_STATUS, &result));
  if (GL_FALSE == result) {
    GLint length;
    GLCall(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length));

    std::string infoLog(static_cast<size_t>(length), 0);
    GLCall(glGetShaderInfoLog(m_id, length, &length, infoLog.data()));

    fmt::println(stderr, "[Shader Error]: Failed to compile shader! {}", path);
    fmt::println(stderr, "{}", infoLog);
  }
}

Shader::Shader(NoCreate_t) : m_id(0) {}

Shader::~Shader()
{
  if (isValid()) { GLCall(glDeleteShader(m_id)); }
}

Shader::Shader(Shader &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {}

Shader &Shader::operator=(Shader &&other) noexcept
{
  if (this == &other) { return *this; }
  using std::swap;
  swap(m_id, other.m_id);
  return *this;
}
