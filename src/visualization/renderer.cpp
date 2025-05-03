#include "visualization/renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace visualization {

using std::make_shared;

Renderer::Renderer()
    : shaderManager_(make_shared<ShaderManager>()),
      fontManager_(make_shared<FontManager>()), mode_(RenderMode::Normal) {
  // Create dependent renderers
  textRenderer_ = make_shared<TextRenderer>(fontManager_, shaderManager_);
  shapeRenderer_ = make_shared<ShapeRenderer>(shaderManager_);
}

Renderer::~Renderer() { cleanup(); }

bool Renderer::initialize() {
  // Initialize components in correct order
  if (!shaderManager_->initialize(mode_)) {
    std::cerr << "Failed to initialize ShaderManager" << std::endl;
    return false;
  }

  if (!fontManager_->initialize(mode_)) {
    std::cerr << "Failed to initialize FontManager" << std::endl;
    return false;
  }

  if (!textRenderer_->initialize(mode_)) {
    std::cerr << "Failed to initialize TextRenderer" << std::endl;
    return false;
  }

  if (!shapeRenderer_->initialize(mode_)) {
    std::cerr << "Failed to initialize ShapeRenderer" << std::endl;
    return false;
  }

  // Set initial viewport
  shapeRenderer_->setViewport(viewportWidth_, viewportHeight_);

  // Enable alpha blending for semi-transparent shapes
  if (mode_ != RenderMode::Headless) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  return true;
}

void Renderer::cleanup() {
  // Clean up components in reverse order
  if (shapeRenderer_) {
    shapeRenderer_->cleanup();
  }

  if (textRenderer_) {
    textRenderer_->cleanup();
  }

  if (fontManager_) {
    fontManager_->cleanup();
  }

  if (shaderManager_) {
    shaderManager_->cleanup();
  }
}

void Renderer::setHeadlessMode(bool headless) {
  mode_ = headless ? RenderMode::Headless : RenderMode::Normal;
}

bool Renderer::isHeadlessMode() const { return mode_ == RenderMode::Headless; }

void Renderer::beginFrame() {
  if (mode_ == RenderMode::Headless) {
    return;
  }

  glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
  if (mode_ == RenderMode::Headless) {
    return;
  }

  glFlush();
}

void Renderer::setViewport(int width, int height) {
  viewportWidth_ = width;
  viewportHeight_ = height;

  if (shapeRenderer_) {
    shapeRenderer_->setViewport(width, height);
  }

  if (mode_ != RenderMode::Headless) {
    glViewport(0, 0, width, height);
  }
}

void Renderer::renderShape(const scene_graph::Shape &shape) {
  if (shapeRenderer_) {
    shapeRenderer_->renderShape(shape);
  }
}

void Renderer::drawRectangle(float x, float y, float width, float height,
                             const Vector4 &color) {
  if (shapeRenderer_) {
    shapeRenderer_->drawRectangle(x, y, width, height, color);
  }
}

void Renderer::drawLine(float x1, float y1, float x2, float y2,
                        const Vector4 &color, float thickness) {
  if (shapeRenderer_) {
    shapeRenderer_->drawLine(x1, y1, x2, y2, color, thickness);
  }
}

void Renderer::drawText(const std::string &text, float x, float y,
                        const Vector4 &color) {
  if (textRenderer_) {
    textRenderer_->drawText(text, x, y, color);
  }
}

std::shared_ptr<ShapeRenderer> Renderer::getShapeRenderer() const {
  return shapeRenderer_;
}

std::shared_ptr<TextRenderer> Renderer::getTextRenderer() const {
  return textRenderer_;
}

} // namespace visualization