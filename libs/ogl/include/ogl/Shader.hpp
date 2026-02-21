#ifndef OGL_SHADER_HPP
#define OGL_SHADER_HPP
#include <format>
#include <glad/glad.h>
#include <map>
#include <string>

class Program;

class Shader
{
public:
  enum struct Types : GLenum {
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

private:
  GLuint m_id;

public:
  Shader(const std::string &path, Types type, const std::map<std::string, std::string> &replacements = {});
  ~Shader();
  Shader(const Shader &other) = delete;
  Shader(Shader &&other) noexcept;

  Shader &operator=(const Shader &other) = delete;
  Shader &operator=(Shader &&other) noexcept;

  friend class Program;
};

#endif// OGL_SHADER_HPP
