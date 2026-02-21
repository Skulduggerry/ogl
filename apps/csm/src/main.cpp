#include "ogl/Camera.hpp"
#include "ogl/Framebuffer.hpp"
#include "ogl/Logging.hpp"
#include "ogl/Program.hpp"
#include "ogl/Shader.hpp"
#include "ogl/VertexArray.hpp"

#include <GLFW/glfw3.h>
#include <fmt/base.h>
#include <glm/ext/matrix_clip_space.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cursor_pos_callback(GLFWwindow *window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void process_input(GLFWwindow *window);

// global variables
Camera camera{ { 0.0f, 0.0f, 5.0f } };

int windowWidth = 800, windowHeight = 600;
float lastX = 0;
float lastY = 0;
bool firstMouse = true;
bool isMouseCaptured = false;
bool fullScreen = false, fullScreenButtonPressed = false;
int currentMonitor = 0;

float deltaTime = 0.0;
float lastFrame = 0.0;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a window
  GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL - CSM", nullptr, nullptr);
  if (!window) {
    fmt::println(stderr, "[GLFW Error] Unable to create window");
    glfwTerminate();
    return -1;
  }

  // set callbacks for the newly created window
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, cursor_pos_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSwapInterval(1);

  // initialize GLAD
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    fmt::println(stderr, "[GLAD Error] Failed to initialize GLAD");
    glfwTerminate();
    return -1;
  }

  // configure the OpenGL state
  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
  GLCall(glViewport(0, 0, windowWidth, windowHeight));
  GLCall(glEnable(GL_DEPTH_TEST));

  // load the shader program
  const Program program{ RESOURCE_PATH "shader.vert", RESOURCE_PATH "shader.frag" };

  // create a vertex buffer
  const VertexBuffer vbo{};
  vbo.bufferData(
    std::array{
      // clang-format off
      -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.0f,  0.5f, 0.0f, 0.0f, 1.0f,
      // clang-format on
    },
    BufferUsage::STATIC_DRAW);

  // set the layout for the buffer
  VertexBufferLayout layout{};
  layout.pushF<BufferDataType::FLOAT>(2, GL_FALSE);
  layout.pushF<BufferDataType::FLOAT>(3, GL_FALSE);

  // create a vertex array
  VertexArray vao{};
  vao.vertexBuffer(0, vbo, layout, 0);

  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    process_input(window);

    // render
    // ------
    GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::mat4 projectionMat = glm::perspective(
      glm::radians(camera.m_zoom), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
    glm::mat4 viewMat = camera.getViewMatrix();
    glm::mat4 modelMat{ 1.0f };

    // set the uniform variable
    program.bind();
    program.setMat4("mvp", projectionMat * viewMat * modelMat);

    // bind the VAO and render the frame
    vao.bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, const int width, const int height)
{
  windowWidth = width;
  windowHeight = height;
  GLCall(glViewport(0, 0, width, height));
}

void cursor_pos_callback([[maybe_unused]] GLFWwindow *window, const double xPosIn, const double yPosIn)
{
  if (!isMouseCaptured) return;

  const auto xPos = static_cast<float>(xPosIn);
  const auto yPos = static_cast<float>(yPosIn);

  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  const float xOffset = xPos - lastX;
  const float yOffset = -(yPos - lastY);
  lastX = xPos;
  lastY = yPos;

  camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback([[maybe_unused]] GLFWwindow *window, [[maybe_unused]] double xOffset, const double yOffset)
{
  if (isMouseCaptured) camera.processMouseScroll(static_cast<float>(yOffset));
}

void process_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && isMouseCaptured) {
    isMouseCaptured = false;
    firstMouse = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !isMouseCaptured) {
    isMouseCaptured = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  // return early when the mouse is not captured
  // -------------------------------------------
  if (!isMouseCaptured) { return; }

  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && !fullScreenButtonPressed) {
    fullScreenButtonPressed = true;
    fullScreen = !fullScreen;

    static int saved_x;
    static int saved_y;
    static int saved_w;
    static int saved_h;

    if (fullScreen) {
      // save the current window state
      glfwGetWindowPos(window, &saved_x, &saved_y);
      glfwGetWindowSize(window, &saved_w, &saved_h);

      int monitorCount;
      GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
      GLFWmonitor *monitor = monitors[currentMonitor % monitorCount];
      currentMonitor++;

      const GLFWvidmode *mode = glfwGetVideoMode(monitor);
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
      glfwSetWindowMonitor(window, nullptr, saved_x, saved_y, saved_w, saved_h, GLFW_DONT_CARE);
    }
  } else if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE) {
    fullScreenButtonPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::FORWARD, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::BACKWARD, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::LEFT, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::RIGHT, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::UP, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::DOWN, deltaTime); }
}