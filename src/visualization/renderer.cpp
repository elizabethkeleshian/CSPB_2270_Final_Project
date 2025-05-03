#include "visualization/renderer.h"
#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include "scene_graph/types.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
namespace visualization {

struct Renderer::Impl {
  int viewportWidth = 800;
  int viewportHeight = 600;
  bool initialized = false;

  unsigned int rectangleVAO = 0;
  unsigned int circleVAO = 0;
  unsigned int shaderProgram = 0;
};

Renderer::Renderer() : impl_(std::make_unique<Impl>()) {};

Renderer::~Renderer() { cleanup(); }

bool Renderer::initialize() {
  // initialize GLEW
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    return false;
  }

  // Create a simple shader program
  const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        
        uniform mat4 model;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        }
    )";

  const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        uniform vec4 color;
        
        void main() {
            FragColor = color;
        }
    )";

  // compile shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "Vertex shader compilation failed: " << infoLog << '\n';
    return false;
  }

  // compile fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  // check for compilation errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "Fragment shader compilation failed: " << infoLog << '\n';
    return false;
  }

  // link shaders
  impl_->shaderProgram = glCreateProgram();
  glAttachShader(impl_->shaderProgram, vertexShader);
  glAttachShader(impl_->shaderProgram, fragmentShader);
  glLinkProgram(impl_->shaderProgram);

  // check for linking errors
  glGetProgramiv(impl_->shaderProgram, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(impl_->shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed: " << infoLog << '\n';
    return false;
  }

  // delete shaders as they are already linked to the program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // rectangle geometry
  float rectangleVertices[] = {
      // positions
      -0.5F, -0.5F, // bottom left
      0.5F,  -0.5F, // bottom right
      0.5F,  0.5F,  // top right
      -0.5F, 0.5F   // top left
  };

  // rectangle indices
  unsigned int rectangleIndices[] = {
      0, 1, 2, // first triangle
      2, 3, 0  // second triangle
  };

  unsigned int VBO, EBO;
  glGenVertexArrays(1, &impl_->rectangleVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices),
               rectangleIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Set up circle geometry (approximated as a polygon)
  const int circleSegments = 32;
  float circleVertices[circleSegments * 2 + 2];
  circleVertices[0] = 0.0f; // center x
  circleVertices[1] = 0.0f; // center y

  for (int i = 0; i <= circleSegments; i++) {
    float angle = 2.0f * M_PI * i / circleSegments;
    circleVertices[2 + i * 2] = 0.5f * cos(angle);     // x
    circleVertices[2 + i * 2 + 1] = 0.5f * sin(angle); // y
  }

  unsigned int circleIndices[circleSegments * 3];
  for (int i = 0; i < circleSegments; i++) {
    circleIndices[i * 3] = 0; // center
    circleIndices[i * 3 + 1] = i + 1;
    circleIndices[i * 3 + 2] = (i + 1) % circleSegments + 1;
  }

  unsigned int circleVBO, circleEBO;
  glGenVertexArrays(1, &impl_->circleVAO);
  glGenBuffers(1, &circleVBO);
  glGenBuffers(1, &circleEBO);

  glBindVertexArray(impl_->circleVAO);

  glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(circleIndices), circleIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Set up OpenGL state
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  impl_->initialized = true;
  return true;
}

void Renderer::cleanup() {
  if (impl_->initialized) {
    glDeleteVertexArrays(1, &impl_->rectangleVAO);
    glDeleteVertexArrays(1, &impl_->circleVAO);
    glDeleteProgram(impl_->shaderProgram);
    impl_->initialized = false;
  }
}

void Renderer::beginFrame() {
  glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
  // Finish any pending operations for the frame
  glFlush();
}

void Renderer::setViewport(int width, int height) {
  impl_->viewportWidth = width;
  impl_->viewportHeight = height;
  glViewport(0, 0, width, height);
}

void Renderer::renderShape(const scene_graph::Shape &shape) {
  if (!impl_->initialized) {
    std::cerr << "Renderer not initialized!\n";
    return;
  }

  glUseProgram(impl_->shaderProgram);

  unsigned int modelLoc = glGetUniformLocation(impl_->shaderProgram, "model");
  unsigned int projLoc =
      glGetUniformLocation(impl_->shaderProgram, "projection");
  unsigned int colorLoc = glGetUniformLocation(impl_->shaderProgram, "color");

  // Create projection matrix (simple orthographic)
  scene_graph::Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set uniforms
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                     glm::value_ptr(shape.getGlobalTransform().getMatrix()));

  const scene_graph::Vector4 &color = shape.getColor();
  glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

  // Draw shape based on type
  if (const auto *rect = dynamic_cast<const scene_graph::Rectangle *>(&shape)) {
    const scene_graph::Vector2 &size = rect->getSize();

    // Create model matrix that scales to the correct size
    glm::mat4 sizeScale =
        glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE,
        glm::value_ptr(shape.getGlobalTransform().getMatrix() * sizeScale));

    // Draw rectangle
    glBindVertexArray(impl_->rectangleVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  } else if (const auto *circle =
                 dynamic_cast<const scene_graph::Circle *>(&shape)) {
    float radius = circle->getRadius();

    // Create model matrix that scales to the correct size
    scene_graph::Matrix4 sizeScale =
        glm::scale(scene_graph::Matrix4(1.0f),
                   glm::vec3(radius * 2.0f, radius * 2.0f, 1.0f));
    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE,
        glm::value_ptr(shape.getGlobalTransform().getMatrix() * sizeScale));

    // Draw circle
    glBindVertexArray(impl_->circleVAO);
    glDrawElements(GL_TRIANGLES, 32 * 3, GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);
  }
}

} // namespace visualization