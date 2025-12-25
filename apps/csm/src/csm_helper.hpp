#ifndef OGL_CSM_HELPER_HPP
#define OGL_CSM_HELPER_HPP

#include <array>
#include <glm/fwd.hpp>

constexpr std::array<glm::vec3, 8> getFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view);
constexpr glm::mat4 getLightViewMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::vec3 &lightDir);
constexpr glm::mat4 getLightProjectionMatrix(const std::array<glm::vec3, 8> &frustumWorldSpaceCorners,
  const glm::mat4 &lightView);

#endif// OGL_CSM_HELPER_HPP
