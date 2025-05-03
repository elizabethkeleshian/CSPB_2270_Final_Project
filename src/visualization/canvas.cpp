#include "visualization/canvas.h"
#include "scene_graph/node.h"
#include "scene_graph/shape.h"
#include "types.h"
#include "visualization/renderer.h"

namespace visualization {

Canvas::Canvas() : renderer_(nullptr), root_(nullptr), selectedNode_(nullptr) {}

Canvas::~Canvas() {}

bool Canvas::initialize(const std::shared_ptr<Renderer> &renderer) {
  if (!renderer) {
    return false;
  }
  renderer_ = renderer;
  return true;
}

void Canvas::setRoot(const std::shared_ptr<scene_graph::Node> &root) {
  root_ = root;
}

std::shared_ptr<scene_graph::Node> Canvas::getRoot() const { return root_; }

void Canvas::addShape(const std::shared_ptr<scene_graph::Shape> &shape) {
  shapes_.push_back(shape);
}

void Canvas::removeShape(const std::shared_ptr<scene_graph::Shape> &shape) {
  shapes_.erase(remove(shapes_.begin(), shapes_.end(), shape), shapes_.end());
}

void Canvas::clear() {
  root_ = nullptr;
  selectedNode_ = nullptr;
  shapes_.clear();
}

void Canvas::selectNode(const std::shared_ptr<scene_graph::Node> &node) {
  // If we already have a selected node, reset its color
  if (selectedNode_) {
    if (auto shape =
            std::dynamic_pointer_cast<scene_graph::Shape>(selectedNode_)) {
      Vector4 originalColor;
      // Get the original color (before selection)
      // This is a simplified approach - you might want to store original colors
      originalColor = shape->getColor();
      // Remove highlight
      shape->setColor(originalColor);
    }
  }

  // Set the new selected node
  selectedNode_ = node;

  // If the new node is not null, highlight it
  if (node) {
    if (auto shape = std::dynamic_pointer_cast<scene_graph::Shape>(node)) {
      Vector4 originalColor = shape->getColor();
      Vector4 highlightColor(1.0f, 1.0f, 0.0f, originalColor.a);
      shape->setColor(highlightColor);
    }
  }
}

std::shared_ptr<scene_graph::Node> Canvas::getSelectedNode() const {
  return selectedNode_;
}

void Canvas::render() {
  if (!renderer_) {
    return;
  }
  renderer_->beginFrame();

  // Render the scene graph starting from the root
  if (root_) {
    renderNode(root_);
  }

  // Render any standalone shapes
  for (const auto &shape : shapes_) {
    renderer_->renderShape(*shape);
  }

  renderer_->endFrame();
}

void Canvas::renderNode(const std::shared_ptr<scene_graph::Node> &node) {
  // If node is a shape, render it
  auto shape = std::dynamic_pointer_cast<scene_graph::Shape>(node);
  if (shape) {
    // Highlight selected node with a different color
    Vector4 originalColor;
    bool isSelected = (selectedNode_ == node);

    if (isSelected) {
      originalColor = shape->getColor();
      // Adjust color to indicate selection (e.g., add a yellow tint)
      Vector4 highlightColor(1.0f, 1.0f, 0.0f, originalColor.a);
      shape->setColor(highlightColor);
    }

    renderer_->renderShape(*shape);

    // Restore original color if changed
    if (isSelected) {
      shape->setColor(originalColor);
    }
  }

  // Recursively render all children
  for (const auto &child : node->getChildren()) {
    renderNode(child);
  }
}

/**
 * @brief Hit test the canvas to determine which node is at a specific point.
 *
 * One of the most important methods for interaction with the canvas. It
 * determines which object or node is at a certain position. For instance,
 * what's under a mouse cursor
 *
 * @param position The position to hit test.
 * @return The node at the position, or nullptr if no node is at the position.
 */
std::shared_ptr<scene_graph::Node>
Canvas::hitTest(const Vector2 &position) const {
  // first check against single shapes
  for (auto iterator = shapes_.rbegin(); iterator != shapes_.rend();
       iterator++) {
    auto shape = *iterator;
    if (shape->containsPoint(position)) {
      return shape;
    }
  }

  // If not found in standalone shapes, check the scene graph
  if (root_) {
    return hitTestRecursive(root_, position);
  }

  return nullptr;
}

std::shared_ptr<scene_graph::Node>
Canvas::hitTestRecursive(const std::shared_ptr<scene_graph::Node> &node,
                         const Vector2 &position) const {

  // check against shapes
  auto shape = std::dynamic_pointer_cast<scene_graph::Shape>(node);
  if (shape && shape->containsPoint(position)) {
    return shape;
  }

  // check against children nodes
  const auto &children = node->getChildren();
  for (auto iterator = children.rbegin(); iterator != children.rend();
       iterator++) {
    auto result = hitTestRecursive(*iterator, position);
    if (result) {
      return result;
    }
  }
  return nullptr;
}

} // namespace visualization