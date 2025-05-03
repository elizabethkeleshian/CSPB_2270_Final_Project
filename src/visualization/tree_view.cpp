#include "visualization/tree_view.h"
#include "visualization/renderer.h"
#include <algorithm>

namespace visualization {

TreeView::TreeView()
    : root_(nullptr), textRenderer_(nullptr), selectedNode_(nullptr) {}

TreeView::~TreeView() = default;

void TreeView::setRoot(const std::shared_ptr<scene_graph::Node> &root) {
  root_ = root;
}

std::shared_ptr<scene_graph::Node> TreeView::getRoot() const { return root_; }

void TreeView::render() {
  if (!textRenderer_ || !root_) {
    return;
  }

  // Clear node positions for hit testing
  nodePositions_.clear();

  // Start rendering from the top left corner
  int yPosition = 20;

  // Render all nodes in the tree recursively
  renderNode(root_, 0, yPosition);
}

void TreeView::renderNode(const std::shared_ptr<scene_graph::Node> &node,
                          int depth, int &yPosition) {
  if (!node) {
    return;
  }

  // Calculate position for this node
  int xPosition = 10 + depth * INDENT_SIZE;

  // Convert screen space to scene space
  // These conversion factors may need adjustment based on your viewport
  // settings
  float sceneX = -9.0f + (float)xPosition / 50.0f;
  float sceneY = 8.0f - (float)yPosition / 35.0f;

  // Store node position for hit testing
  NodePosition pos;
  pos.node = node;
  pos.x = xPosition;
  pos.y = yPosition;
  pos.height = NODE_HEIGHT;
  nodePositions_.push_back(pos);

  // Determine if this node is selected
  bool isSelected = (node == selectedNode_);

  // Draw node name text
  if (textRenderer_) {
    Vector4 textColor = isSelected ? Vector4(0.3f, 0.6f, 1.0f, 1.0f)
                                   : // Blue for selected
                            Vector4(0.8f, 0.8f, 0.8f,
                                    1.0f); // White for unselected

    textRenderer_->drawText(node->getName(), sceneX, sceneY, textColor);
  }

  // Update yPosition for the next node
  yPosition += NODE_HEIGHT;

  // Recursively render all children
  for (const auto &child : node->getChildren()) {
    renderNode(child, depth + 1, yPosition);
  }
}

int TreeView::countNodes(const std::shared_ptr<scene_graph::Node> &node) const {
  if (!node) {
    return 0;
  }

  int count = 1; // Count this node

  // Add count of all children
  for (const auto &child : node->getChildren()) {
    count += countNodes(child);
  }

  return count;
}

void TreeView::selectAt(const Vector2 &position) {
  // Convert scene space to screen space
  // These conversion factors should match those in renderNode
  float screenX = (position.x + 9.0f) * 50.0f;
  float screenY = (8.0f - position.y) * 35.0f;

  // Check if the position is within any node's area
  for (const auto &nodePos : nodePositions_) {
    float nodeWidth = 150.0f - (nodePos.x - 10) / INDENT_SIZE * 10.0f;

    if (screenX >= nodePos.x && screenX <= nodePos.x + nodeWidth &&
        screenY >= nodePos.y && screenY <= nodePos.y + nodePos.height) {
      selectedNode_ = nodePos.node;
      return;
    }
  }

  // If we get here, no node was clicked in the tree view
}

std::shared_ptr<scene_graph::Node> TreeView::getSelectedNode() const {
  return selectedNode_;
}

} // namespace visualization