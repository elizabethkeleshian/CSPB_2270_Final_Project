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
  return;
}

std::shared_ptr<scene_graph::Node>
Canvas::hitTest(const scene_graph::Vector2 &position) const {
  return nullptr;
}

} // namespace visualization