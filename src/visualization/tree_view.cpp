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

  // Draw a background for the tree view
  float treeWidth = constants::TREE_VIEW_WIDTH;
  float treeHeight = constants::SCENE_HEIGHT;
  float treeX = -constants::SCENE_HALF_WIDTH;
  float treeY = -constants::SCENE_HALF_HEIGHT;

  // Draw background
  Vector4 bgColor(0.12F, 0.14F, 0.17F,
                  1.0F); // Slightly darker than main background
  renderer_->drawRectangle(treeX, treeY, treeWidth, treeHeight, bgColor);

  // Draw a title for the tree view
  Vector4 titleColor(1.0F, 1.0F, 1.0F, 1.0F);
  float titleY = constants::SCENE_HALF_HEIGHT - 0.8F;
  renderer_->drawText("Scene Hierarchy", treeX + 0.5F, titleY, titleColor);

  // Add a separator line
  Vector4 lineColor(0.4F, 0.4F, 0.5F, 0.8F);
  float separatorY = titleY - 0.4F;
  renderer_->drawLine(treeX + 0.2F, separatorY, treeX + treeWidth - 0.5F,
                      separatorY, lineColor, 0.02F);

  int yPosition = constants::TREE_VIEW_STARTING_Y;

  // Render all nodes in the tree recursively
  renderNode(root_, 0, yPosition);
}

void TreeView::renderNode(const std::shared_ptr<scene_graph::Node> &node,
                          int depth, int &yPosition) {
  if (!node || !renderer_) {
    return;
  }

  // Constants for better layout
  const float baseX =
      -constants::SCENE_HALF_WIDTH + 0.5F; // Starting X position
  const float indentSize = 0.6f;           // Indent for children (scene space)
  const float nodeHeight = 0.6f;      // Taller nodes for better clickability
  const float verticalSpacing = 0.7f; // Space between nodes

  // Calculate position for this node
  float sceneX = baseX + (depth * indentSize);
  float sceneY =
      constants::SCENE_HALF_HEIGHT - 1.5f - (yPosition * verticalSpacing);

  // Make sure we don't exceed tree view width
  float treeViewWidth = constants::TREE_VIEW_WIDTH;
  float maxTextWidth = treeViewWidth - (depth * indentSize) -
                       1.0f; // Subtract padding and indentation

  // Calculate node dimensions with constraint on width
  float textWidth =
      std::min(node->getName().length() * 0.2f + 0.4f, maxTextWidth);

  // Store node position for hit testing (in scene coordinates)
  NodePosition pos;
  pos.node = node;
  pos.x = sceneX;
  pos.y = sceneY;
  pos.width = textWidth;
  pos.height = nodeHeight;
  nodePositions_.push_back(pos);

  // Determine if this node is selected
  bool isSelected = (node == selectedNode_);

  // Draw node background - always draw a background for better clickability
  Vector4 bgColor =
      isSelected ? Vector4(0.3F, 0.6F, 1.0F, 0.5F)
                 :                           // Highlighted blue for selected
          Vector4(0.2F, 0.22F, 0.25F, 0.7F); // Dark gray for normal

  // Calculate background rectangle
  float rectHeight = nodeHeight;
  renderer_->drawRectangle(sceneX, sceneY - 0.25f, pos.width, rectHeight,
                           bgColor);

  // Draw connection lines
  if (depth > 0) {
    Vector4 lineColor(0.6f, 0.6f, 0.6f, 0.8f); // Brighter, more visible lines

    // Horizontal connector to this node
    float parentX = baseX + ((depth - 1) * indentSize);
    float lineStartX =
        parentX + 0.5f; // Start halfway into the parent's text area
    renderer_->drawLine(lineStartX, sceneY, sceneX - 0.1f, sceneY, lineColor,
                        0.02f);

    // Don't draw vertical lines for every child - that gets messy
    // Instead, draw one vertical line from the parent down to this node's level
    auto parent = node->getParent().lock();
    if (parent && !parent->getChildren().empty() &&
        node == parent->getChildren().front()) {
      // For first child, draw vertical line from parent
      float parentY = sceneY + verticalSpacing; // Approximate parent position
      renderer_->drawLine(lineStartX, parentY, lineStartX, sceneY, lineColor,
                          0.02f);
    }
  }

  // Draw the text
  Vector4 textColor =
      isSelected ? Vector4(1.0f, 1.0f, 1.0f, 1.0f) : // White for selected
          Vector4(0.9f, 0.9f, 0.9f, 0.9f);           // Light gray for normal

  renderer_->drawText(node->getName(), sceneX + 0.2f, sceneY, textColor);

  // Update yPosition for the next node
  yPosition += 1;

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
  for (const auto &nodePos : nodePositions_) {
    // Check if the point is inside the node's rectangle
    if (position.x >= nodePos.x && position.x <= nodePos.x + nodePos.width &&
        position.y >= nodePos.y - nodePos.height / 2 &&
        position.y <= nodePos.y + nodePos.height / 2) {

      selectedNode_ = nodePos.node;
      return;
    }
  }
  // No node was clicked
}

std::shared_ptr<scene_graph::Node> TreeView::getSelectedNode() const {
  return selectedNode_;
}

} // namespace visualization