#ifndef OGL_SHADER_HPP
#define OGL_SHADER_HPP
#include <format>
#include <glad/glad.h>
#include <string>

class Program;

class Shader
{
public:
  enum struct Types : GLenum { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

private:
  GLuint m_id;

public:
  Shader(const std::string &path, Types type);
  ~Shader();
  Shader(const Shader &other) = delete;
  Shader(Shader &&other) noexcept;

  Shader &operator=(const Shader &other) = delete;
  Shader &operator=(Shader &&other) noexcept;

  friend class Program;
};

#endif// OGL_SHADER_HPP
