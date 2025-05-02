#include "visualization/renderer.h"
#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
namespace visualization {

struct Renderer::Impl {
  int viewportWidth = 800;
  int viewportHeight = 600;
  bool initialized = false;

  // testing helper
  bool frameActive = false;
  int shapesRendered = 0;
};

Renderer::Renderer() : impl_(std::make_unique<Impl>()) {};

Renderer::~Renderer() { cleanup(); }

bool Renderer::initialize() {
  impl_->initialized = true;
  std::cout << "Renderer initialized with viewport " << impl_->viewportWidth
            << "x" << impl_->viewportHeight << '\n';
  return true;
}

void Renderer::cleanup() {
  impl_->initialized = false;
  impl_->frameActive = false;
  impl_->shapesRendered = 0;
  std::cout << "Renderer cleaned up" << '\n';
}

void Renderer::beginFrame() {
  if (!impl_->initialized) {
    std::cerr << "Renderer not initialized!" << '\n';
    return;
  }
  impl_->frameActive = true;
  impl_->shapesRendered = 0;
  std::cout << "Frame started" << '\n';
}

void Renderer::endFrame() {
  if (!impl_->initialized) {
    std::cerr << "Renderer not initialized!" << '\n';
    return;
  }
  if (!impl_->frameActive) {
    std::cerr << "Frame not active!" << '\n';
    return;
  }
  impl_->frameActive = false;
  std::cout << "Frame ended, " << impl_->shapesRendered << " shapes rendered"
            << '\n';
}

void Renderer::setViewport(int width, int height) {
  impl_->viewportWidth = width;
  impl_->viewportHeight = height;
  std::cout << "Viewport set to " << width << "x" << height << '\n';
}

void Renderer::renderShape(const scene_graph::Shape &shape) {
  if (!impl_->initialized) {
    std::cerr << "Renderer not initialized!\n";
    return;
  }
  if (!impl_->frameActive) {
    std::cerr << "Frame not active!\n";
    return;
  }
  scene_graph::Transform transform = shape.getGlobalTransform();
  scene_graph::Vector4 color = shape.getColor();

  // Log shape information
  std::cout << "Rendering " << shape.getName() << ": ";
  std::cout << "Position(" << transform.getPosition().x << ", "
            << transform.getPosition().y << ") ";
  std::cout << "Rotation(" << transform.getRotation() << ") ";
  std::cout << "Scale(" << transform.getScale().x << ", "
            << transform.getScale().y << ") ";
  std::cout << "Color(" << color[0] << ", " << color[1] << ", " << color[2]
            << ", " << color[3] << ")\n";

  // Check shape type for specific rendering
  if (const auto *const rect =
          dynamic_cast<const scene_graph::Rectangle *>(&shape)) {
    scene_graph::Vector2 size = rect->getSize();
    std::cout << "Rectangle size: " << size.x << "x" << size.y << '\n';
  } else if (const auto *const circle =
                 dynamic_cast<const scene_graph::Circle *>(&shape)) {
    float radius = circle->getRadius();
    std::cout << "Circle radius: " << radius << '\n';
  }

  // Increment render count
  impl_->shapesRendered++;
}

} // namespace visualization