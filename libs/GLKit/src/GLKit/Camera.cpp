#include "../../include/GLKit/Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
  : m_position(position), m_worldUp(worldUp), m_yaw(yaw), m_pitch(pitch)
{ updateCameraVectors(); }

Camera::Camera(float posX,
  float posY,
  float posZ,
  float worldUpX,
  float worldUpY,
  float worldUpZ,
  const float yaw,
  const float pitch)
  : Camera({ posX, posY, posZ }, { worldUpX, worldUpY, worldUpZ }, yaw, pitch)
{}

void Camera::processKeyboard(const Directions direction, float deltaTime)
{
  const float velocity = m_movementSpeed * deltaTime;

  switch (direction) {
  case Directions::FORWARD:
    m_position += m_front * velocity;
    break;
  case Directions::BACKWARD:
    m_position -= m_front * velocity;
    break;
  case Directions::LEFT:
    m_position -= m_right * velocity;
    break;
  case Directions::RIGHT:
    m_position += m_right * velocity;
    break;
  case Directions::UP:
    m_position += m_worldUp * velocity;
    break;
  case Directions::DOWN:
    m_position -= m_worldUp * velocity;
    break;
  }
}

void Camera::processMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
{
  xOffset *= m_mouseSensitivity;
  yOffset *= m_mouseSensitivity;

  m_yaw += xOffset;
  m_pitch += yOffset;

  if (constrainPitch) {
    static constexpr float MAX_PITCH = 89.0F;
    m_pitch = glm::clamp(m_pitch, -MAX_PITCH, MAX_PITCH);
  }

  updateCameraVectors();
}

void Camera::processMouseScroll(const float yOffset)
{
  static constexpr float MIN_FOV = 1.0F;
  static constexpr float MAX_FOV = 45.0F;
  m_zoom = glm::clamp(m_zoom - yOffset, MIN_FOV, MAX_FOV);
}

void Camera::updateCameraVectors()
{
  glm::vec3 front{};
  front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  front.y = glm::sin(glm::radians(m_pitch));
  front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(front, m_worldUp));
}
