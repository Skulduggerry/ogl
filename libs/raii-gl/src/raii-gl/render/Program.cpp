#include "raii-gl/render/Program.hpp"
#include "raii-gl/Logging.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

static GLuint createProgram()
{
  GLCall(const GLuint id = glCreateProgram());
  return id;
}

Program::Program() : m_id(createProgram()) {}

Program::~Program()
{
  if (hasName()) { GLCall(glDeleteProgram(m_id)); }
}

Program::Program(Program &&other) noexcept
  : m_id(std::exchange(other.m_id, 0)), m_linked(std::exchange(other.m_linked, false)),
    m_uniformLocationCache(std::move(other.m_uniformLocationCache)),
    m_uniformBlockIndexCache(std::move(other.m_uniformBlockIndexCache))
{}

Program &Program::operator=(Program &&other) noexcept
{
  if (this == &other) { return *this; }

  // release currently owned resource
  if (hasName()) { GLCall(glDeleteProgram(m_id)); }

  // steal
  m_id = std::exchange(other.m_id, 0);
  m_linked = std::exchange(other.m_linked, false);
  m_uniformLocationCache = std::move(other.m_uniformLocationCache);
  m_uniformBlockIndexCache = std::move(other.m_uniformBlockIndexCache);

  return *this;
}

void Program::bind() const { GLCall(glUseProgram(m_id)); }

void Program::unbind() { GLCall(glUseProgram(0)); }

static std::string getProgramInfoLog(const GLuint program)
{
  std::string log{};
  GLint length = 0;
  GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
  if (length <= 1) return log;

  log.resize(static_cast<std::size_t>(length), '\0');
  GLCall(glGetProgramInfoLog(program, length, &length, log.data()));
  return log;
}

ProgramLinkResult Program::link(const std::span<const Shader *const> shaders)
{
  m_uniformLocationCache.clear();
  m_uniformBlockIndexCache.clear();

  if (shaders.empty()) {
    m_linked = false;
    return { false, "No shaders provided to Program::link()!" };
  }

  for (const Shader *shader : shaders) {
    if (!shader || !shader->hasName()) {
      m_linked = false;
      return { false, "Null/invalid shader passed to Program::link()!" };
    }

    if (!shader->isCompiled()) {
      m_linked = false;
      return { false, "Attempt to link program with an uncompiled shader." };
    }
  }

  for (const Shader *shader : shaders) { GLCall(glAttachShader(m_id, shader->getId())); }

  GLCall(glLinkProgram(m_id));

  for (const Shader *shader : shaders) { GLCall(glDetachShader(m_id, shader->getId())); }

  GLint ok = GL_FALSE;
  GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &ok));

  if (ok != GL_TRUE) {
    m_linked = false;
    return { false, getProgramInfoLog(m_id) };
  }

  m_linked = true;
  return { true, {} };
}

ProgramLinkResult Program::link(const Shader &vertex, const Shader &fragment)
{
  std::array arr = { &vertex, &fragment };
  return link(arr);
}

ProgramLinkResult Program::link(const Shader &compute)
{
  std::array arr = { &compute };
  return link(arr);
}

void Program::setBool(const std::string_view name, const GLboolean value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform1i(m_id, location, value));
  }
}

void Program::setInt(const std::string_view name, const GLint value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform1i(m_id, location, value));
  }
}

void Program::setUnsigned(const std::string_view name, const GLuint value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform1ui(m_id, location, value));
  }
}

void Program::setFloat(const std::string_view name, const GLfloat value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform1f(m_id, location, value));
  }
}

void Program::setVec2(const std::string_view name, const glm::vec2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform2fv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setVec3(const std::string_view name, const glm::vec3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform3fv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setVec4(const std::string_view name, const glm::vec4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform4fv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setIVec2(const std::string_view name, const glm::ivec2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform2iv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setIVec3(const std::string_view name, const glm::ivec3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform3iv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setIVec4(const std::string_view name, const glm::ivec4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform4iv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setUVec2(const std::string_view name, const glm::uvec2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform2uiv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setUVec3(const std::string_view name, const glm::uvec3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform3uiv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setUVec4(const std::string_view name, const glm::uvec4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniform4uiv(m_id, location, 1, glm::value_ptr(value)));
  }
}

void Program::setMat2(const std::string_view name, const glm::mat2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix2fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat3(const std::string_view name, const glm::mat3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix3fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat4(const std::string_view name, const glm::mat4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix4fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat2x3(const std::string_view name, const glm::mat2x3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix2x3fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat3x2(const std::string_view name, const glm::mat3x2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix3x2fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat2x4(const std::string_view name, const glm::mat2x4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix2x4fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat4x2(const std::string_view name, const glm::mat4x2 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix4x2fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat3x4(const std::string_view name, const glm::mat3x4 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix3x4fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::setMat4x3(const std::string_view name, const glm::mat4x3 &value) const
{
  if (const GLint location = getUniformLocation(name); location != -1) {
    GLCall(glProgramUniformMatrix4x3fv(m_id, location, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void Program::uniformBlockBinding(const std::string_view uniformBlockName, const GLuint bindingPoint) const
{
  if (const GLuint index = getUniformBlockIndex(uniformBlockName); index != GL_INVALID_INDEX) {
    GLCall(glUniformBlockBinding(m_id, index, bindingPoint));
  }
}

void Program::debugLabel(std::string_view name) const
{ GLCall(glObjectLabel(GL_PROGRAM, m_id, static_cast<GLsizei>(name.size()), name.data())); }

static std::optional<Shader>
  loadShader(const std::string_view path, const ShaderType type, const Replacements &replacements)
{
  ShaderSource source = Shader::loadSource(path, replacements);
  if (!source.ok) {
    fmt::println(stderr, "[Program Error] {}", source.log);
    return std::nullopt;
  }

  Shader shader{ type };
  if (auto [ok, log] = shader.compile(source.code); !ok) {
    fmt::println(stderr, "[Program Error] {}", log);
    return std::nullopt;
  }

  return shader;
}

std::optional<Program> Program::fromFile(const std::string &vertexPath,
  const std::string &fragmentPath,
  const std::span<const ShaderSourceInfo> additional,
  const Replacements &replacements)
{
  std::vector<Shader> shaders{};
  shaders.reserve(2 + additional.size());

  std::optional<Shader> vertexShader = loadShader(vertexPath, ShaderType::VERTEX, replacements);
  if (!vertexShader.has_value()) return std::nullopt;
  shaders.emplace_back(std::move(*vertexShader));

  std::optional<Shader> fragmentShader = loadShader(fragmentPath, ShaderType::FRAGMENT, replacements);
  if (!fragmentShader.has_value()) return std::nullopt;
  shaders.emplace_back(std::move(*fragmentShader));

  for (const auto &[path, type] : additional) {
    std::optional<Shader> shader = loadShader(path, type, replacements);
    if (!shader.has_value()) return std::nullopt;
    shaders.emplace_back(std::move(*shader));
  }

  std::vector<Shader const *> ptrs(shaders.size());
  for (std::size_t i = 0; i < shaders.size(); ++i) { ptrs[i] = &shaders[i]; }

  Program program{};
  if (auto [ok, log] = program.link(ptrs); !ok) {
    fmt::println(stderr, "[Program Error] {}", log);
    return std::nullopt;
  }
  return program;
}

std::optional<Program> Program::fromFile(const std::string &computePath, const Replacements &replacements)
{
  const std::optional<Shader> vertexShader = loadShader(computePath, ShaderType::COMPUTE, replacements);
  if (!vertexShader.has_value()) return std::nullopt;

  Program program{};
  if (auto [ok, log] = program.link(vertexShader.value()); !ok) {
    fmt::println(stderr, "[Program Error] {}", log);
    return std::nullopt;
  }
  return program;
}

GLuint Program::getUniformBlockIndex(const std::string_view uniformBlockName) const
{
  if (!m_linked) return GL_INVALID_INDEX;

  if (const auto it = m_uniformBlockIndexCache.find(uniformBlockName); m_uniformBlockIndexCache.end() != it) {
    return it->second;
  }

  const std::string name{ uniformBlockName };
  GLCall(const GLuint index = glGetUniformBlockIndex(m_id, name.c_str()));

  if (index == GL_INVALID_INDEX) {
    fmt::println(stderr, "Warning: uniform block index '{}' does not exist!", uniformBlockName);
  }

  m_uniformBlockIndexCache.insert({ name, index });
  return index;
}

GLint Program::getUniformLocation(const std::string_view uniformName) const
{
  if (!m_linked) return -1;

  if (const auto it = m_uniformLocationCache.find(uniformName); m_uniformLocationCache.end() != it) {
    return it->second;
  }

  const std::string name{ uniformName };
  GLCall(GLint location = glGetUniformLocation(m_id, name.c_str()));

  if (-1 == location) { fmt::println(stderr, "Warning: uniform '{}' does not exist!", uniformName); }

  m_uniformLocationCache.insert({ name, location });
  return location;
}
