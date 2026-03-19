#include "ogl/Camera.hpp"
#include "ogl/Framebuffer.hpp"
#include "ogl/Logging.hpp"
#include "ogl/Program.hpp"
#include "ogl/Shader.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/texture_old/Texture2D.hpp"
#include "ogl/texture_old/Texture2DArray.hpp"

#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <memory>
#include <random>
#define STB_IMAGE_IMPLEMENTATION
#include "ogl/buffer/UniformBuffer.hpp"


#include <glm/ext/matrix_clip_space.hpp>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cursor_pos_callback(GLFWwindow *window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void process_input(GLFWwindow *window);

Texture2D loadTexture(const std::string &path);

void renderScene(const Program &program);
void renderCube();
void renderPlane();
void renderQuad();

// global variables
Camera camera{ { 5.0f, 5.0f, 5.0f } };
Camera debugCamera{ { -80, 80, 80 }, { 0.0, 1.0, 0.0 }, -45, -35 };

int windowWidth = 800, windowHeight = 600;
float lastX = 0;
float lastY = 0;
bool firstMouse = true;
bool isMouseCaptured = false;
bool fullScreen = false;
int currentMonitor = 0;
float cameraNearPlane = 0.1f, cameraFarPlane = 500.0f;

// timing
float deltaTime = 0.0;
float lastFrame = 0.0;

std::array shadowCascadeLevels{ cameraFarPlane / 50.0f,
  cameraFarPlane / 25.0f,
  cameraFarPlane / 10.0f,
  cameraFarPlane / 2.0f };
int debugLayer = 0;

// lighting info
const glm::vec3 lightDir = glm::normalize(glm::vec3(20.0f, 50, 20.0f));
constexpr unsigned int depthMapResolution = 4096;

bool showQuad = false, showDebugCam = false;

std::random_device device;
std::mt19937 generator{ device() };

std::array<glm::mat4, shadowCascadeLevels.size() + 1> getLightSpaceMatrices();
void drawCascadeVolumeVisualizers(const Program &program);

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

  // build and compile shaders
  const Program shader{ RESOURCE_PATH "shaders/shader.vert", RESOURCE_PATH "shaders/shader.frag" };
  const Program simpleDepthShader{ RESOURCE_PATH "shaders/shadow_mapping_depth.vert",
    RESOURCE_PATH "shaders/shadow_mapping_depth.frag",
    { Program::ShaderSourceInfo{ RESOURCE_PATH "shaders/shadow_mapping_depth.geom", Shader::Type::GEOMETRY } } };
  const Program debugDepthQuad(RESOURCE_PATH "shaders/debug_quad.vert", RESOURCE_PATH "shaders/debug_quad.frag");
  const Program debugCascadeShader(
    RESOURCE_PATH "shaders/debug_cascade.vert", RESOURCE_PATH "shaders/debug_cascade.frag");

  // load textures
  const Texture2D woodTexture = loadTexture("textures/wood.png");

  // configure light FBO
  Texture2DArray lightDepthMaps{};
  lightDepthMaps.storage(
    InternalFormat::DEPTH_COMPONENT32F, depthMapResolution, depthMapResolution, shadowCascadeLevels.size() + 1);
  lightDepthMaps.minFilter(TextureMinFilter::NEAREST);
  lightDepthMaps.magFilter(TextureMagFilter::NEAREST);
  lightDepthMaps.textureWrapS(TextureWrap::CLAMP_TO_BORDER);
  lightDepthMaps.textureWrapT(TextureWrap::CLAMP_TO_BORDER);
  lightDepthMaps.borderColor({ 1.0f, 1.0f, 1.0f, 1.0f });

  const Framebuffer lightFBO{};
  lightFBO.attach(lightDepthMaps, Attachment::DEPTH, 0);
  lightFBO.drawBuffer(Attachment::NONE);
  lightFBO.readBuffer(Attachment::NONE);
  if (!lightFBO.isComplete()) {
    fmt::println(stderr, "[OpenGL Error] Framebuffer is not complete!");
    return -1;
  }

  // configure UBO
  UniformBuffer matricesUBO{};
  matricesUBO.allocateMutableBytes(sizeof(glm::mat4) * 16, BufferUsage::STATIC_DRAW);
  matricesUBO.bindBase(0);

  // shader configuration
  shader.setInt("diffuseTexture", 0);
  shader.setInt("shadowMap", 1);
  debugDepthQuad.setInt("depthMap", 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    [[maybe_unused]] Camera c = camera;

    // per-frame time logic
    // --------------------
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    process_input(window);

    // change light position over time
    // lightPos.x = sin(glfwGetTime()) * 3.0f;
    // lightPos.z = cos(glfwGetTime()) * 2.0f;
    // lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

    // render
    // ------
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 0. UBO setup
    const auto lightMatrices = getLightSpaceMatrices();
    matricesUBO.subDataBytes(0, std::as_bytes(std::span{ lightMatrices }));

    // 1. render the depth of a scene to texture (from light's perspective)
    simpleDepthShader.bind();
    lightFBO.bind();
    GLCall(glViewport(0, 0, depthMapResolution, depthMapResolution));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    GLCall(glCullFace(GL_FRONT));
    renderScene(simpleDepthShader);
    GLCall(glCullFace(GL_BACK));
    DefaultFramebuffer::getInstance().bind();

    // 2. render the scene as normal using the generated depth/shadow map
    if (showQuad) {

      // reset viewport
      GLCall(glViewport(0, 0, windowWidth, windowHeight));
      GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

      debugDepthQuad.bind();
      debugDepthQuad.setInt("layer", debugLayer);
      lightDepthMaps.bindTextureUnit(0);
      renderQuad();

    } else {
      // reset viewport
      GLsizei width = showDebugCam ? windowWidth / 2 : windowWidth;

      GLCall(glViewport(0, 0, width, windowHeight));
      GLCall(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

      shader.bind();

      const glm::mat4 proj = glm::perspective(glm::radians(camera.getZoom()),
        static_cast<float>(width) / static_cast<float>(windowHeight),
        cameraNearPlane,
        cameraFarPlane);
      const glm::mat4 view = camera.getViewMatrix();
      shader.setMat4("projection", proj);
      shader.setMat4("view", view);

      shader.setVec3("viewPos", camera.getPosition());
      shader.setVec3("lightDir", lightDir);
      shader.setFloat("farPlane", cameraFarPlane);
      shader.setInt("cascadeCount", shadowCascadeLevels.size());
      for (size_t i = 0; i < shadowCascadeLevels.size(); ++i) {
        shader.setFloat(fmt::format("cascadePlaneDistances[{}]", i), shadowCascadeLevels[i]);
      }

      woodTexture.bindTextureUnit(0);
      lightDepthMaps.bindTextureUnit(1);
      renderScene(shader);

      if (showDebugCam) {
        // reset viewport
        GLCall(glViewport(width + 1, 0, windowWidth - width, windowHeight));

        const glm::mat4 debugProj = glm::perspective(glm::radians(debugCamera.getZoom()),
          static_cast<float>(width) / static_cast<float>(windowHeight),
          cameraNearPlane,
          cameraFarPlane);
        const glm::mat4 debugView = debugCamera.getViewMatrix();
        shader.setMat4("projection", debugProj);
        shader.setMat4("view", debugView);
        shader.setVec3("viewPos", debugCamera.getPosition());
        renderScene(shader);

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        debugCascadeShader.bind();
        debugCascadeShader.setMat4("projection", debugProj);
        debugCascadeShader.setMat4("view", debugView);
        drawCascadeVolumeVisualizers(debugCascadeShader);
        GLCall(glDisable(GL_BLEND));
      }
    }

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

  static bool fullScreenButtonPressed = false;
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

  static int fPress = GLFW_RELEASE;
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && fPress == GLFW_PRESS) { showQuad = !showQuad; }
  fPress = glfwGetKey(window, GLFW_KEY_F);

  static int nPress = GLFW_RELEASE;
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE && nPress == GLFW_PRESS) {
    debugLayer++;
    if (debugLayer > static_cast<int>(shadowCascadeLevels.size())) { debugLayer = 0; }
  }
  nPress = glfwGetKey(window, GLFW_KEY_N);

  static int cPress = GLFW_RELEASE;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && cPress == GLFW_PRESS) { showDebugCam = !showDebugCam; }
  cPress = glfwGetKey(window, GLFW_KEY_C);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::FORWARD, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::BACKWARD, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::LEFT, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::RIGHT, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::UP, deltaTime); }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { camera.processKeyboard(Camera::Directions::DOWN, deltaTime); }
}

Texture2D loadTexture(const std::string &path)
{
  Texture2D texture{};

  int width, height, channels;
  uint8_t *data = stbi_load((RESOURCE_PATH + path).c_str(), &width, &height, &channels, 0);
  if (!data) {
    fmt::println(stderr, "Failed to load texture at path: {}", RESOURCE_PATH + path);
    stbi_image_free(data);
    return texture;
  }

  Format format;
  InternalFormat internal_format;
  switch (channels) {
  case 1:
    format = Format::RED;
    internal_format = InternalFormat::R8;
    break;
  case 2:
    format = Format::RG;
    internal_format = InternalFormat::RG8;
    break;
  case 3:
    format = Format::RGB;
    internal_format = InternalFormat::RGB8;
    break;
  case 4:
    format = Format::RGBA;
    internal_format = InternalFormat::RGBA8;
    break;
  default:
    format = Format::RGB;
    internal_format = InternalFormat::RGB8;
  }

  texture.storage(internal_format, width, height);
  texture.subImage(0, 0, 0, width, height, format, ImageDataType::UNSIGNED_BYTE, data);
  texture.generateMipmap();

  texture.textureWrapS(TextureWrap::REPEAT);
  texture.textureWrapT(TextureWrap::REPEAT);
  texture.minFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
  texture.magFilter(TextureMagFilter::LINEAR);

  stbi_image_free(data);
  return texture;
}

void renderCube()
{
  static bool first = true;
  static VertexArray cubeVAO{};
  static VertexBuffer<GLfloat> cubeVBO{};

  if (first) {
    first = false;

    // clang-format off
    static constexpr std::array vertices = {
      // back face
      -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
       1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
       1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
       1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
      -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
      -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
      // front face
      -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
       1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
       1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
       1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
      -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
      -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
      // left face
      -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
      -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
      -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
      -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
      -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
      -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
      // right face
       1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
       1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
       1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
       1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
       1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
       1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
      // bottom face
      -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
       1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
       1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
       1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
      -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
      -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
      // top face
      -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
       1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
       1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
       1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
      -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
      -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // bottom-left
    };
    // clang-format on
    cubeVBO.allocateImmutable(vertices);

    VertexBufferLayout layout{};
    layout.pushFloat(AttributeType::FLOAT, 3).pushFloat(AttributeType::FLOAT, 3).pushFloat(AttributeType::FLOAT, 2);

    cubeVAO.vertexBuffer(0, cubeVBO, layout, 0);
  }

  cubeVAO.bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  VertexArray::unbind();
}

void renderPlane()
{
  static bool first = true;
  static VertexArray planeVAO{};
  static VertexBuffer<GLfloat> planeVBO{};

  if (first) {
    first = false;

    // clang-format off
    static constexpr std::array vertices = {
      // positions            // normals         // texcoords
      25.0f, -2.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
     -25.0f, -2.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
     -25.0f, -2.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
      25.0f, -2.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
     -25.0f, -2.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
      25.0f, -2.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f,
    };
    // clang-format on
    planeVBO.allocateImmutable(vertices);

    VertexBufferLayout layout{};
    layout.pushFloat(AttributeType::FLOAT, 3).pushFloat(AttributeType::FLOAT, 3).pushFloat(AttributeType::FLOAT, 2);

    planeVAO.vertexBuffer(0, planeVBO, layout, 0);
  }

  planeVAO.bind();
  GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
  VertexArray::unbind();
}

void renderQuad()
{
  static bool first = true;
  static VertexArray quadVAO{};
  static VertexBuffer<GLfloat> quadVBO{};

  if (first) {
    first = false;

    // clang-format off
    static constexpr std::array vertices = {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
       1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // clang-format on
    quadVBO.allocateImmutable(vertices);

    VertexBufferLayout layout{};
    layout.pushFloat(AttributeType::FLOAT, 3).pushFloat(AttributeType::FLOAT, 2);

    quadVAO.vertexBuffer(0, quadVBO, layout, 0);
  }

  quadVAO.bind();
  GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 6));
  VertexArray::unbind();
}

void renderScene(const Program &program)
{
  // floor
  program.setMat4("model", glm::mat4{ 1.0f });
  renderPlane();

  static std::array<glm::mat4, 10> modelMatrices{};
  static bool first = true;
  if (first) {
    first = false;
    for (unsigned int i = 0; i < 10; ++i) {
      static std::uniform_real_distribution<float> offsetDistribution{ -10, 10 };
      static std::uniform_real_distribution<float> scaleDistribution{ 1.0, 2.0 };
      static std::uniform_real_distribution<float> rotationDistribution{ 0, 180 };

      glm::mat4 model{ 1.0f };
      model = glm::translate(model,
        glm::vec3{
          offsetDistribution(generator), offsetDistribution(generator) + 10.0f, offsetDistribution(generator) });
      model = glm::rotate(
        model, glm::radians(rotationDistribution(generator)), glm::normalize(glm::vec3{ 1.0f, 0.0f, 1.0f }));
      model = glm::scale(model, glm::vec3{ scaleDistribution(generator) });
      modelMatrices[i] = model;
    }
  }

  for (const auto &model : modelMatrices) {
    program.setMat4("model", model);
    renderCube();
  }
}


constexpr std::array<glm::vec3, 8> getFrustumCornersWorldSpace(const glm::mat4 &projView)
{
  const glm::mat4 inv = glm::inverse(projView);
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


std::array<glm::vec3, 8> getFrustumCornersWorldSpace(const glm::mat4 &proj, const glm::mat4 &view)
{ return getFrustumCornersWorldSpace(proj * view); }

glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane)
{
  const auto proj = glm::perspective(glm::radians(camera.getZoom()),
    static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
    nearPlane,
    farPlane);
  const auto corners = getFrustumCornersWorldSpace(proj, camera.getViewMatrix());

  // get the center of the frustum by averaging the coordinates of the corners
  glm::vec3 center = std::reduce(corners.begin(), corners.end(), glm::vec3{ 0.0F });
  center /= corners.size();

  const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();
  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::lowest();
  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::lowest();
  for (const auto &v : corners) {
    const auto trf = lightView * glm::vec4{ v, 1.0f };
    minX = std::min(minX, trf.x);
    maxX = std::max(maxX, trf.x);
    minY = std::min(minY, trf.y);
    maxY = std::max(maxY, trf.y);
    minZ = std::min(minZ, trf.z);
    maxZ = std::max(maxZ, trf.z);
  }

  // Tune this parameter according to the scene
  constexpr float zMult = 10.0f;
  if (minZ < 0) {
    minZ *= zMult;
  } else {
    minZ /= zMult;
  }
  if (maxZ < 0) {
    maxZ /= zMult;
  } else {
    maxZ *= zMult;
  }

  const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
  return lightProjection * lightView;
}

std::array<glm::mat4, shadowCascadeLevels.size() + 1> getLightSpaceMatrices()
{
  std::array<glm::mat4, shadowCascadeLevels.size() + 1> ret{};

  for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i) {
    if (i == 0) {
      ret[i] = getLightSpaceMatrix(cameraNearPlane, shadowCascadeLevels[i]);
    } else if (i < shadowCascadeLevels.size()) {
      ret[i] = getLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]);
    } else {
      ret[i] = getLightSpaceMatrix(shadowCascadeLevels[i - 1], cameraFarPlane);
    }
  }

  return ret;
}


void drawCascadeVolumeVisualizers(const Program &program)
{
  static VertexArray vao{};
  static VertexBuffer<glm::vec3> vbo{};
  static IndexBuffer<GLuint> ebo{};

  // clang-format off
  static constexpr std::array<GLuint, 36> indices = {
    0, 2, 3,
    0, 3, 1,
    4, 6, 2,
    4, 2, 0,
    5, 7, 6,
    5, 6, 4,
    1, 3, 7,
    1, 7, 5,
    6, 7, 3,
    6, 3, 2,
    1, 5, 4,
    0, 1, 4,
  };
  // clang-format on

  // clang-format off
  static constexpr std::array colors = {
    glm::vec4{1.0, 0.0, 0.0, 0.5f},
    glm::vec4{0.0, 1.0, 0.0, 0.5f},
    glm::vec4{0.0, 0.0, 1.0, 0.5f},
  };
  // clang-format on

  ebo.allocateMutable(indices);

  for (unsigned int i = 0; i < shadowCascadeLevels.size() + 1; ++i) {
    float near, far;
    if (i == 0) {
      near = cameraNearPlane;
      far = shadowCascadeLevels[0];
    } else if (i == shadowCascadeLevels.size()) {
      near = shadowCascadeLevels[i - 1];
      far = cameraFarPlane;
    } else {
      near = shadowCascadeLevels[i - 1];
      far = shadowCascadeLevels[i];
    }

    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),
      static_cast<float>(windowWidth) / 2 / static_cast<float>(windowHeight),
      near,
      far);

    const auto corners = getFrustumCornersWorldSpace(projection * view);
    vbo.allocateMutable(corners);

    VertexBufferLayout layout{};
    layout.pushFloat(AttributeType::FLOAT, 3);

    vao.vertexBuffer(0, vbo, layout, 0);
    vao.indexBuffer(ebo);

    vao.bind();
    program.setVec4("color", colors[i % 3]);
    GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
  }

  VertexArray::unbind();
}