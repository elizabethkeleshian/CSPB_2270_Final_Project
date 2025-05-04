#include "visualization/tree_view.h"
#include "visualization/renderer.h"
#include <iostream>

namespace visualization {

TreeView::TreeView()
    : root_(nullptr), textRenderer_(nullptr), selectedNode_(nullptr),
      renderer_(nullptr) {}

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

  if (!renderer_) {
    std::cout << "Renderer is null" << std::endl;
    return;
  }

  // Calculate position for this node
  int xPosition = 10 + depth * INDENT_SIZE;
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

  // Draw selection background if selected
  if (isSelected && textRenderer_) {
    // Draw a background rectangle for selected item
    Vector4 bgColor(0.25f, 0.27f, 0.32f,
                    1.0f); // Slightly lighter than background

    // Calculate background rectangle size based on text
    float textWidth = node->getName().length() * 0.15f; // Approximate width
    float rectX = sceneX - 0.1f;                        // Slight padding
    float rectY = sceneY - 0.1f;
    float rectWidth = textWidth + 0.2f;
    float rectHeight = 0.2f;

    // Draw rectangle
    renderer_->drawRectangle(rectX, rectY, rectWidth, rectHeight, bgColor);
  }

  // Draw line indicators for hierarchy
  if (depth > 0 && textRenderer_) {
    Vector4 lineColor(0.4f, 0.4f, 0.4f, 1.0f); // Subtle gray

    // Draw vertical line
    float lineX = sceneX - INDENT_SIZE / constants::TREE_VIEW_X_SCALE;
    float lineY = sceneY;

    // Simple line rendering
    renderer_->drawLine(lineX, lineY - 0.1f, lineX, lineY, lineColor);
    renderer_->drawLine(lineX, lineY, sceneX - 0.2f, lineY, lineColor);
  }

  // Draw text with appropriate color
  if (textRenderer_) {
    Vector4 textColor =
        isSelected
            ? Vector4(
                  constants::colors::PRIMARY[0], constants::colors::PRIMARY[1],
                  constants::colors::PRIMARY[2], constants::colors::PRIMARY[3])
            : Vector4(constants::colors::TEXT[0], constants::colors::TEXT[1],
                      constants::colors::TEXT[2], constants::colors::TEXT[3]);

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