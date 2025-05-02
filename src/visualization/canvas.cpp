#include "visualization/canvas.h"
#include "scene_graph/node.h"
#include "scene_graph/shape.h"
#include "visualization/renderer.h"
#include "scene_graph/types.h"


using namespace std;
using namespace scene_graph;
namespace visualization {

Canvas::Canvas() : renderer_(nullptr), root_(nullptr), selectedNode_(nullptr) {}

Canvas::~Canvas() {}

bool Canvas::initialize(shared_ptr<Renderer> renderer) {
    if (!renderer) {
        return false;
    }
    renderer_ = renderer;
    return true;
}

void Canvas::setRoot(shared_ptr<Node> root) {
    root_ = root;
}

shared_ptr<Node> Canvas::getRoot() const {
    return root_;
}

void Canvas::addShape(shared_ptr<Shape> shape) {
    shapes_.push_back(shape);
}

void Canvas::removeShape(shared_ptr<Shape> shape) {
    shapes_.erase(remove(shapes_.begin(), shapes_.end(), shape), shapes_.end());
}

void Canvas::clear() {
    root_ = nullptr;
    selectedNode_ = nullptr;
    shapes_.clear();
}

void Canvas::selectNode(const shared_ptr<Node>& node) {
    selectedNode_ = node;
}

shared_ptr<Node> Canvas::getSelectedNode() const {
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

    for (const shared_ptr<Shape>& shape : shapes_) {
        renderer_->renderShape(*shape);
    }
    renderer_->endFrame();
}

void Canvas::renderNode(shared_ptr<Node> node) {
   return;
}

shared_ptr<Node> Canvas::hitTest(const Vector2& position) const {
    return nullptr;
}

} // namespace visualization