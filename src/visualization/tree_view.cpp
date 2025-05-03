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

  // Start rendering from the top left corner with the constant Y position
  int yPosition = constants::TREE_VIEW_STARTING_Y;

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
  float sceneX = constants::TREE_VIEW_X_OFFSET +
                 ((float)xPosition / constants::TREE_VIEW_X_SCALE);
  float sceneY = constants::TREE_VIEW_Y_OFFSET -
                 ((float)yPosition / constants::TREE_VIEW_Y_SCALE);

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
  // Convert scene space to screen space using constants
  float screenX = (position.x + constants::TREE_VIEW_SCREEN_X_OFFSET) *
                  constants::TREE_VIEW_SCREEN_X_FACTOR;
  float screenY = (constants::TREE_VIEW_SCREEN_Y_OFFSET - position.y) *
                  constants::TREE_VIEW_SCREEN_Y_FACTOR;

  // Check if the position is within any node's area
  for (const auto &nodePos : nodePositions_) {
    float nodeWidth =
        constants::TREE_VIEW_NODE_WIDTH_BASE -
        (nodePos.x - 10) / INDENT_SIZE * constants::TREE_VIEW_NODE_WIDTH_ADJUST;

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