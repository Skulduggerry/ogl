#include "raii-gl/render/Shader.hpp"
#include "raii-gl/Logging.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <optional>
#include <utility>

static void trim(std::string_view &view)
{
  while (!view.empty() && std::isspace(static_cast<unsigned char>(view.front()))) view.remove_prefix(1);
  while (!view.empty() && std::isspace(static_cast<unsigned char>(view.back()))) view.remove_suffix(1);
}

static std::optional<std::string> readFile(std::filesystem::path path)
{
  std::ifstream file{ path, std::ios::in | std::ios::binary };
  if (!file.is_open()) { return std::nullopt; }

  file.seekg(0, std::ios::end);
  const std::streamsize size = file.tellg();
  if (size < 0) return std::nullopt;
  file.seekg(0, std::ios::beg);

  std::string contents(static_cast<std::size_t>(size), '\0');
  if (!file.read(contents.data(), size)) return std::nullopt;

  return contents;
}

static std::string mapReplace(std::string_view text, const Replacements &replacements)
{
  std::string out{};
  out.reserve(text.size());

  if (replacements.empty()) {
    out = text;
    return out;
  }

  std::size_t i = 0;
  while (i < text.size()) {
    // find next {{
    const std::size_t open = text.find("{{", i);
    if (open == std::string_view::npos) {
      out.append(text.substr(i));
      break;
    }

    // copy everything before {{
    out.append(text.substr(i, open - i));

    // find closing }}
    const std::size_t close = text.find("}}", open + 2);
    if (close == std::string_view::npos) {
      // no closing braces -> treat as literal
      out.append(text.substr(open));
      break;
    }

    std::string_view key = text.substr(open + 2, close - (open + 2));
    trim(key);

    // replace if found or keep original token
    if (auto it = replacements.find(key); it != replacements.end()) {
      out.append(it->second);
    } else {
      out.append(text.substr(open, (close + 2) - open));
    }

    i = close + 2;
  }

  return out;
}

static GLuint createShader(ShaderType type)
{
  GLCall(const GLuint id = glCreateShader(static_cast<GLenum>(type)));
  return id;
}

Shader::Shader(const ShaderType type) : m_id(createShader(type)), m_type(type) {}

Shader::~Shader()
{
  if (hasName()) { GLCall(glDeleteShader(m_id)); }
}

Shader::Shader(Shader &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_type(other.m_type), m_compiled(std::exchange(other.m_compiled, false))
{}

Shader &Shader::operator=(Shader &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteShader(m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);
  m_type = other.m_type;
  m_compiled = std::exchange(other.m_compiled, false);

  return *this;
}

ShaderSource Shader::loadSource(const std::string_view path, const Replacements &replacements)
{
  const auto absPath = std::filesystem::absolute(std::filesystem::path{ path });

  const std::optional<std::string> file = readFile(absPath);
  if (!file.has_value()) {
    return { false, absPath.string(), {}, fmt::format("Unable to load file at: {}", absPath.c_str()) };
  }
  return { true, absPath.string(), mapReplace(std::string_view{ *file }, replacements), {} };
}

ShaderCompileResult Shader::compile(const std::string_view code)
{
  const char *src = code.data();
  const auto size = static_cast<GLint>(code.size());

  GLCall(glShaderSource(m_id, 1, &src, &size));
  GLCall(glCompileShader(m_id));

  GLint result;
  GLCall(glGetShaderiv(m_id, GL_COMPILE_STATUS, &result));
  if (GL_FALSE == result) {
    GLint length;
    GLCall(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length));

    std::string infoLog(static_cast<std::size_t>(length), '\0');
    GLCall(glGetShaderInfoLog(m_id, length, &length, infoLog.data()));

    m_compiled = false;
    return { false, infoLog };
  }

  m_compiled = true;
  return { true, std::string{} };
}

void Shader::debugLabel(const std::string_view name) const
{ GLCall(glObjectLabel(GL_SHADER, m_id, static_cast<GLsizei>(name.size()), name.data())); }
