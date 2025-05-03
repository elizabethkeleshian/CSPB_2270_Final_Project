#include "visualization/tree_view.h"

namespace visualization {

TreeView::TreeView() : root_(nullptr), renderer_(nullptr) {}

TreeView::~TreeView() {}

void TreeView::setRoot(const std::shared_ptr<scene_graph::Node> &root) {
  root_ = root;
}

// std::shared_ptr<scene_graph::Node> TreeView::getRoot() const { return root_;
// }

// void TreeView::render() {
//   // clear node positions
//   nodePositions_.clear();

//   const int yPosition = 10;

//   if (!root_) {
//     renderNode(root_, 0, yPosition);
//   }
// }

} // namespace visualization
