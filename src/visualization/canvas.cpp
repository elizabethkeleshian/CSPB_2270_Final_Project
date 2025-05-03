#include "visualization/canvas.h"
#include "scene_graph/node.h"
#include "scene_graph/shape.h"
#include "scene_graph/types.h"
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
  selectedNode_ = node;
}

std::shared_ptr<scene_graph::Node> Canvas::getSelectedNode() const {
  return selectedNode_;
}

void Canvas::render() {
  if (!renderer_) {
    return;
  }
  renderer_->beginFrame();
  if (root_) {
    renderNode(root_);
  }

  for (const std::shared_ptr<scene_graph::Shape> &shape : shapes_) {
    renderer_->renderShape(*shape);
  }
  renderer_->endFrame();
}

void Canvas::renderNode(const std::shared_ptr<scene_graph::Node> &node) {
  // check if node is a shape
  auto shape = std::dynamic_pointer_cast<scene_graph::Shape>(node);
  if (shape) {
    renderer_->renderShape(*shape);

    // add highlight effect
    if (selectedNode_ == node) {
      // TODO: add highlight effect if time permits
    }
  }

  // recursively render children
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
Canvas::hitTest(const scene_graph::Vector2 &position) const {
  if (!root_) {
    return nullptr;
  }

  // first check against single shapes
  for (auto iterator = shapes_.rbegin(); iterator != shapes_.rend();
       iterator++) {
    auto shape = *iterator;
    if (shape->containsPoint(position)) {
      return shape;
    }
  }

  // recursively check against children nodes. Need to find the topmost node
  return hitTestRecursive(root_, position);
}

std::shared_ptr<scene_graph::Node>
Canvas::hitTestRecursive(const std::shared_ptr<scene_graph::Node> &node,
                         const scene_graph::Vector2 &position) const {

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