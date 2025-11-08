#include "ogl/Framebuffer.hpp"
#include "ogl/VertexBuffer.hpp"

#include <algorithm>
#include <array>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <numeric>
#include <ranges>
#include <vector>

constexpr std::array<glm::vec3, 8> getFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view);
constexpr glm::mat4 getLightViewMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::vec3 &lightDir);
constexpr glm::mat4 getLightProjectionMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::mat4 &lightView);


int main()
{
  const VertexBuffer vbo{};
  const std::vector<GLfloat> vertices{ 0.0, 0.0, 0.5, 1.0, 1.0, 0.0 };
  vbo.bufferData(vertices, GL_STATIC_DRAW);
}

constexpr std::array<glm::vec3, 8> getFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view)
{
  const glm::mat4 inv = glm::inverse(proj * view);
  std::array<glm::vec3, 8> frustumCorners{};
  auto ptr = frustumCorners.begin();

  static constexpr std::array ndcCorners{ -1.0F, 1.0F };
  for (const float x : ndcCorners) {
    for (const float y : ndcCorners) {
      for (const float z : ndcCorners) {
        const glm::vec4 pt = inv * glm::vec4{ x, y, z, 1.0F };
        *ptr = glm::vec3{ pt / pt.w };
        ++ptr;
      }
    }
  }

  return frustumCorners;
}

constexpr glm::mat4 getLightViewMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::vec3 &lightDir)
{
  // get the center of the frustum by averaging the coordinates of the corners
  glm::vec3 center = std::reduce(frustumWorldSpaceCorners.begin(), frustumWorldSpaceCorners.end(), glm::vec3{ 0.0F });
  center /= frustumWorldSpaceCorners.size();

  return glm::lookAt(center + lightDir, center, glm::vec3{ 0.0F, 1.0F, 0.0F });
}

constexpr glm::mat4 getLightProjectionMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::mat4 &lightView)
{
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();
  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::lowest();
  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::lowest();
  for (const auto &corner : frustumWorldSpaceCorners) {
    const glm::vec3 trf = lightView * glm::vec4{ corner, 1.0F };
    minX = std::min(minX, trf.x);
    maxX = std::max(maxX, trf.x);
    minY = std::min(minY, trf.y);
    maxY = std::max(maxY, trf.y);
    minZ = std::min(minZ, trf.z);
    maxZ = std::max(maxZ, trf.z);
  }

  // tune this parameter according to the scene
  static constexpr float zMult = 10.0F;
  minZ = minZ < 0 ? minZ * zMult : minZ / zMult;
  maxZ = maxX < 0 ? maxX / zMult : maxX * zMult;

  return glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
}