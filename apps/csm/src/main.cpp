#include "glad/glad.h"
#include "ogl/Program.hpp"
#include "ogl/VertexBuffer.hpp"
#include <vector>

int main()
{
  const VertexBuffer vbo{};
  const std::vector<GLfloat> vertices{ 0.0, 0.0, 0.5, 1.0, 1.0, 0.0 };
  vbo.bufferData(vertices, GL_STATIC_DRAW);
}