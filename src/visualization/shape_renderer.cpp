#include "visualization/shape_renderer.h"
#include <GL/glew.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace visualization {

struct ShapeRenderer::Impl {
  bool initialized = false;
  unsigned int rectangleVAO = 0;
  unsigned int circleVAO = 0;
  int viewportWidth = 800;
  int viewportHeight = 600;
  std::string shaderName = "shape";
};

ShapeRenderer::ShapeRenderer(std::shared_ptr<ShaderManager> shaderManager)
    : shaderManager_(shaderManager), impl_(std::make_unique<Impl>()) {}

ShapeRenderer::~ShapeRenderer() { cleanup(); }

bool ShapeRenderer::initialize(RenderMode mode) {
  if (mode == RenderMode::Headless) {
    impl_->initialized = true;
    return true;
  }

  // Create shader program for shapes
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

  if (!shaderManager_->createShaderProgram(
          impl_->shaderName, vertexShaderSource, fragmentShaderSource)) {
    std::cerr << "Failed to create shape shader program" << std::endl;
    return false;
  }

  // Rectangle geometry
  float rectangleVertices[] = {
      -0.5f, -0.5f, // bottom left
      0.5f,  -0.5f, // bottom right
      0.5f,  0.5f,  // top right
      -0.5f, 0.5f   // top left
  };

  unsigned int rectangleIndices[] = {
      0, 1, 2, // first triangle
      2, 3, 0  // second triangle
  };

  unsigned int VBO, EBO;
  glGenVertexArrays(1, &impl_->rectangleVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(impl_->rectangleVAO);

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

  // Circle geometry
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

  impl_->initialized = true;
  return true;
}

void ShapeRenderer::cleanup() {
  if (impl_->initialized) {
    if (impl_->rectangleVAO != 0) {
      glDeleteVertexArrays(1, &impl_->rectangleVAO);
      impl_->rectangleVAO = 0;
    }

    if (impl_->circleVAO != 0) {
      glDeleteVertexArrays(1, &impl_->circleVAO);
      impl_->circleVAO = 0;
    }
  }

  impl_->initialized = false;
}

void ShapeRenderer::setViewport(int width, int height) {
  impl_->viewportWidth = width;
  impl_->viewportHeight = height;
}

void ShapeRenderer::renderShape(const scene_graph::Shape &shape) {
  if (!impl_->initialized) {
    std::cerr << "ShapeRenderer not initialized!" << std::endl;
    return;
  }

  // Skip rendering in headless mode
  if (shaderManager_->isHeadlessMode()) {
    return;
  }

  // Use the shape shader program
  shaderManager_->useShader(impl_->shaderName);

  // Create projection matrix
  Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set projection uniform
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "projection",
                                      projection);

  // Set model matrix from the shape's transform
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "model",
                                      shape.getGlobalTransform().getMatrix());

  // Set color uniform
  shaderManager_->setUniform4f(impl_->shaderName, "color", shape.getColor());

  // Draw shape based on type
  if (const auto *rect = dynamic_cast<const scene_graph::Rectangle *>(&shape)) {
    const Vector2 &size = rect->getSize();

    // Create scale matrix for size
    Matrix4 sizeScale =
        glm::scale(Matrix4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    Matrix4 modelWithSize = shape.getGlobalTransform().getMatrix() * sizeScale;

    // Set the combined model matrix
    shaderManager_->setUniformMatrix4fv(impl_->shaderName, "model",
                                        modelWithSize);

    // Draw rectangle
    glBindVertexArray(impl_->rectangleVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  } else if (const auto *circle =
                 dynamic_cast<const scene_graph::Circle *>(&shape)) {
    float radius = circle->getRadius();

    // Create scale matrix for radius
    Matrix4 sizeScale = glm::scale(
        Matrix4(1.0f), glm::vec3(radius * 2.0f, radius * 2.0f, 1.0f));
    Matrix4 modelWithSize = shape.getGlobalTransform().getMatrix() * sizeScale;

    // Set the combined model matrix
    shaderManager_->setUniformMatrix4fv(impl_->shaderName, "model",
                                        modelWithSize);

    // Draw circle
    glBindVertexArray(impl_->circleVAO);
    glDrawElements(GL_TRIANGLES, 32 * 3, GL_UNSIGNED_INT, 0);
  }

  // Unbind VAO
  glBindVertexArray(0);
}

void ShapeRenderer::drawRectangle(float x, float y, float width, float height,
                                  const Vector4 &color) {
  if (!impl_->initialized) {
    std::cerr << "ShapeRenderer not initialized!" << std::endl;
    return;
  }

  // Skip rendering in headless mode
  if (shaderManager_->isHeadlessMode()) {
    return;
  }

  // Use the shape shader program
  shaderManager_->useShader(impl_->shaderName);

  // Create projection matrix
  Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set projection uniform
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "projection",
                                      projection);

  // Create model matrix with translation and scale
  Matrix4 model = Matrix4(1.0f);
  model = glm::translate(model,
                         glm::vec3(x + width / 2.0f, y + height / 2.0f, 0.0f));
  model = glm::scale(model, glm::vec3(width, height, 1.0f));

  // Set model and color uniforms
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "model", model);
  shaderManager_->setUniform4f(impl_->shaderName, "color", color);

  // Draw rectangle
  glBindVertexArray(impl_->rectangleVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void ShapeRenderer::drawLine(float x1, float y1, float x2, float y2,
                             const Vector4 &color, float thickness) {
  if (!impl_->initialized) {
    std::cerr << "ShapeRenderer not initialized!" << std::endl;
    return;
  }

  // Skip rendering in headless mode
  if (shaderManager_->isHeadlessMode()) {
    return;
  }

  // Calculate line length and angle
  float dx = x2 - x1;
  float dy = y2 - y1;
  float length = std::sqrt(dx * dx + dy * dy);
  float angle = std::atan2(dy, dx);

  // Use the shape shader program
  shaderManager_->useShader(impl_->shaderName);

  // Create projection matrix
  Matrix4 projection = glm::ortho(
      -10.0f, 10.0f, -10.0f * impl_->viewportHeight / impl_->viewportWidth,
      10.0f * impl_->viewportHeight / impl_->viewportWidth, -1.0f, 1.0f);

  // Set projection uniform
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "projection",
                                      projection);

  // Create model matrix with translation, rotation and scale
  Matrix4 model = Matrix4(1.0f);
  model = glm::translate(model,
                         glm::vec3((x1 + x2) / 2.0f, (y1 + y2) / 2.0f, 0.0f));
  model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(length, thickness, 1.0f));

  // Set model and color uniforms
  shaderManager_->setUniformMatrix4fv(impl_->shaderName, "model", model);
  shaderManager_->setUniform4f(impl_->shaderName, "color", color);

  // Draw line using the rectangle VAO
  glBindVertexArray(impl_->rectangleVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

bool ShapeRenderer::isInitialized() const { return impl_->initialized; }

} // namespace visualization