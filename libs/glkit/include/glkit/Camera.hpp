#ifndef OGL_CAMERA_HPP
#define OGL_CAMERA_HPP

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
public:
  enum struct Directions : std::uint8_t { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

  // clang-format off
  static constexpr float YAW         = -90.0F;
  static constexpr float PITCH       =   0.0F;
  static constexpr float ZOOM        =  45.0F;
  static constexpr float SPEED       =   2.5F;
  static constexpr float SENSITIVITY =   0.1F;
  // clang-format on

private:
  // camera vectors
  glm::vec3 m_position;
  glm::vec3 m_front{};
  glm::vec3 m_right{};
  glm::vec3 m_worldUp;

  // euler angles
  float m_yaw;
  float m_pitch;

  // camera options
  float m_zoom = ZOOM;

public:
  // public camera options
  float m_movementSpeed = SPEED;
  float m_mouseSensitivity = SENSITIVITY;

  // constructor with vectors
  explicit Camera(glm::vec3 position = {},
    glm::vec3 worldUp = { 0.0F, 1.0F, 0.0F },
    float yaw = YAW,
    float pitch = PITCH);

  Camera(float posX,
    float posY,
    float posZ,
    float worldUpX,
    float worldUpY,
    float worldUpZ,
    float yaw = YAW,
    float pitch = PITCH);

  [[nodiscard]] glm::vec3 getPosition() const { return m_position; }

  [[nodiscard]] glm::vec3 getViewDirection() const { return m_front; }

  [[nodiscard]] glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }

  [[nodiscard]] float getZoom() const { return m_zoom; }

  void processKeyboard(Directions direction, float deltaTime);

  void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

  void processMouseScroll(float yOffset);

private:
  void updateCameraVectors();
};

#endif// OGL_CAMERA_HPP
