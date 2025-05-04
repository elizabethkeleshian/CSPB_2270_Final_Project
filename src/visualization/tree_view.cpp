#include "visualization/tree_view.h"
#include "constants.h"
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

  // Calculate coordinates for the tree view background
  float treeWidth = constants::TREE_VIEW_WIDTH;
  float treeHeight = constants::SCENE_HEIGHT;
  float treeX = -constants::SCENE_HALF_WIDTH;
  float treeY = -constants::SCENE_HALF_HEIGHT;

  // Update the visible height
  visibleHeight_ = treeHeight;

  // Draw background
  Vector4 treeViewBg(constants::colors::TREE_VIEW_BACKGROUND[0],
                     constants::colors::TREE_VIEW_BACKGROUND[1],
                     constants::colors::TREE_VIEW_BACKGROUND[2],
                     constants::colors::TREE_VIEW_BACKGROUND[3]);
  renderer_->drawRectangle(treeX, treeY, treeWidth, treeHeight, treeViewBg);

  // Position title at ABSOLUTE top - zero offset
  Vector4 titleColor(
      constants::colors::TITLE_TEXT[0], constants::colors::TITLE_TEXT[1],
      constants::colors::TITLE_TEXT[2], constants::colors::TITLE_TEXT[3]);

  // Use small offset from the top to keep it visible
  float titleY = constants::SCENE_HALF_HEIGHT - 0.3f;

  renderer_->drawText("Scene Hierarchy",
                      treeX + constants::TREE_VIEW_TITLE_PADDING, titleY,
                      titleColor);

  // Start nodes below the title with a percentage-based spacing
  int yPosition = 1; // Start with a simple index

  // Apply scroll position offset - using virtual nodes for the layout
  // but applying the scroll offset when actually rendering

  // Calculate content height first to ensure scrollbar accuracy
  calculateContentHeight();

  // Create clipping by adjusting the viewport
  // Note: In a real implementation, you'd use scissor testing here
  // For our simplified version, we'll just adjust the visibility in renderNode

  // Render all nodes in the tree recursively with updated spacing
  renderNode(root_, 0, yPosition);

  // Render scrollbar if needed
  renderScrollBar();
}

void TreeView::renderNode(const std::shared_ptr<scene_graph::Node> &node,
                          int depth, int &yPosition) {
  if (!node || !renderer_) {
    return;
  }

  // Constants for better layout
  const float baseX =
      -constants::SCENE_HALF_WIDTH + constants::TREE_VIEW_ELEMENT_PADDING;
  const float indentSize = constants::TREE_INDENT_SIZE;
  const float nodeHeight = constants::TREE_NODE_HEIGHT;
  const float verticalSpacing = constants::TREE_VERT_SPACING;

  // Calculate position for this node - use absolute positioning from top
  float sceneX = baseX + (depth * indentSize);
  // Use very small spacing for first node and consistent spacing for all
  // Apply scrolling offset here
  float sceneY = constants::SCENE_HALF_HEIGHT - 0.6f -
                 (yPosition * verticalSpacing) + scrollPosition_;

  // Check if this node is within the visible area
  float topBoundary = constants::SCENE_HALF_HEIGHT;
  float bottomBoundary = -constants::SCENE_HALF_HEIGHT;

  // Skip rendering if node is outside visible area
  if (sceneY < bottomBoundary || sceneY - nodeHeight > topBoundary) {
    // Still store the position for hierarchy consistency
    NodePosition pos;
    pos.node = node;
    pos.x = sceneX;
    pos.y = sceneY;
    pos.width = 0; // Not visible, so no width
    pos.height = nodeHeight;
    pos.visualY = sceneY - constants::TREE_NODE_VERTICAL_OFFSET;
    nodePositions_.push_back(pos);

    // Continue updating yPosition for the next node
    yPosition += 1;

    // Recursively render all children
    for (const auto &child : node->getChildren()) {
      renderNode(child, depth + 1, yPosition);
    }

    return;
  }

  // Make sure we don't exceed tree view width
  float treeViewWidth = constants::TREE_VIEW_WIDTH;
  float maxTextWidth = treeViewWidth - (depth * indentSize) -
                       1.0f; // Subtract padding and indentation

  // Calculate node dimensions with constraint on width
  float textWidth =
      std::min(node->getName().length() * constants::TEXT_CHAR_WIDTH_FACTOR +
                   constants::TEXT_WIDTH_PADDING,
               maxTextWidth);

  // Store node position for hit testing (in scene coordinates)
  NodePosition pos;
  pos.node = node;
  pos.x = sceneX;
  pos.y = sceneY;
  pos.width = textWidth;
  pos.height = nodeHeight;

  // Store the actual visual area for debugging
  pos.visualY = sceneY - constants::TREE_NODE_VERTICAL_OFFSET;

  nodePositions_.push_back(pos);

  // Determine if this node is selected
  bool isSelected = (node == selectedNode_);

  // Draw node background - only for selected nodes
  if (isSelected) {
    Vector4 bgColor = Vector4(
        constants::colors::UI_SELECTED[0], constants::colors::UI_SELECTED[1],
        constants::colors::UI_SELECTED[2], constants::colors::UI_SELECTED[3]);

    // Calculate background rectangle - ensure it's aligned with the text
    float rectHeight =
        nodeHeight * 0.8f; // Make slightly smaller for better appearance
    float vertOffset = constants::TREE_NODE_VERTICAL_OFFSET;
    renderer_->drawRectangle(sceneX, sceneY - vertOffset, pos.width, rectHeight,
                             bgColor);
  }

  // Draw connection lines
  if (depth > 0) {
    Vector4 lineColor(
        constants::colors::UI_LINE[0], constants::colors::UI_LINE[1],
        constants::colors::UI_LINE[2], constants::colors::UI_LINE[3]);

    // Horizontal connector to this node
    float parentX = baseX + ((depth - 1) * indentSize);
    float lineStartX = parentX + constants::TREE_VIEW_ELEMENT_PADDING;
    renderer_->drawLine(lineStartX, sceneY,
                        sceneX - constants::TREE_NODE_CONNECTOR_PADDING, sceneY,
                        lineColor, constants::DEFAULT_LINE_THICKNESS);

    // Don't draw vertical lines for every child - that gets messy
    // Instead, draw a short vertical line from the connection point up
    auto parent = node->getParent().lock();
    if (parent && !parent->getChildren().empty() &&
        node == parent->getChildren().front()) {
      // Find the previous node position for better line drawing
      float parentY = sceneY + verticalSpacing; // Approximate parent position

      // Limit the vertical line length to prevent going off-screen
      float maxLineLength = std::min(verticalSpacing, 2.0f);
      float lineEndY = sceneY + maxLineLength;

      // Draw the vertical connector line
      renderer_->drawLine(lineStartX, lineEndY, lineStartX, sceneY, lineColor,
                          constants::DEFAULT_LINE_THICKNESS);
    }
  }

  // Draw the text
  Vector4 textColor;
  if (isSelected) {
    textColor = Vector4(constants::colors::UI_SELECTED_TEXT[0],
                        constants::colors::UI_SELECTED_TEXT[1],
                        constants::colors::UI_SELECTED_TEXT[2],
                        constants::colors::UI_SELECTED_TEXT[3]);
  } else {
    textColor = Vector4(constants::colors::UI_NORMAL_TEXT[0],
                        constants::colors::UI_NORMAL_TEXT[1],
                        constants::colors::UI_NORMAL_TEXT[2],
                        constants::colors::UI_NORMAL_TEXT[3]);
  }

  // Position text with proper padding and centering
  float textX = sceneX + constants::TEXT_PADDING_X;
  float textY = sceneY - constants::TREE_TEXT_VERT_OFFSET;

  renderer_->drawText(node->getName(), textX, textY, textColor);

  // Update yPosition for the next node
  yPosition += 1;

  // Recursively render all children
  for (const auto &child : node->getChildren()) {
    renderNode(child, depth + 1, yPosition);
  }
}

void TreeView::renderScrollBar() {
  // Only render scrollbar if content exceeds visible area
  if (contentHeight_ <= visibleHeight_) {
    return;
  }

  // Calculate scrollbar metrics
  float treeX = -constants::SCENE_HALF_WIDTH;
  float treeWidth = constants::TREE_VIEW_WIDTH;
  float treeHeight = constants::SCENE_HEIGHT;
  float scrollBarX = treeX + treeWidth - scrollBarWidth_ - 0.1f;
  float scrollBarFullHeight = treeHeight - 0.6f; // Leave space for title

  // Calculate the thumb size and position
  float visibleRatio = visibleHeight_ / contentHeight_;
  float thumbHeight =
      std::max(scrollBarFullHeight * visibleRatio, scrollBarMinHeight_);
  float scrollRange = contentHeight_ - visibleHeight_;
  float scrollRatio =
      (scrollPosition_ <= 0) ? 0 : (-scrollPosition_ / scrollRange);
  float maxThumbTravel = scrollBarFullHeight - thumbHeight;
  float thumbY =
      constants::SCENE_HALF_HEIGHT - 0.6f - (scrollRatio * maxThumbTravel);

  // Draw the scrollbar track
  Vector4 trackColor(0.2f, 0.2f, 0.2f, 0.5f); // Semi-transparent dark gray
  renderer_->drawRectangle(scrollBarX, -constants::SCENE_HALF_HEIGHT,
                           scrollBarWidth_, scrollBarFullHeight, trackColor);

  // Draw the scrollbar thumb
  Vector4 thumbColor(0.5f, 0.5f, 0.5f, 0.8f); // Light gray
  renderer_->drawRectangle(scrollBarX, thumbY - thumbHeight, scrollBarWidth_,
                           thumbHeight, thumbColor);
}

void TreeView::calculateContentHeight() {
  if (!root_) {
    contentHeight_ = 0.0f;
    return;
  }

  // Count total nodes in the tree
  int totalNodes = countNodes(root_);

  // Calculate total height based on node count and spacing
  // Include title space
  contentHeight_ = 0.6f + (totalNodes * constants::TREE_VERT_SPACING);
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
  std::cout << "Click at position: (" << position.x << ", " << position.y << ")"
            << std::endl;

  // Check if click is on scrollbar first
  if (isPointInScrollBar(position)) {
    startScrollDrag(position);
    return;
  }

  // Debug output of all node positions
  for (const auto &pos : nodePositions_) {
    std::cout << "Node: " << pos.node->getName() << " Box: x=" << pos.x
              << " y=" << pos.y << " visual_y=" << pos.visualY
              << " w=" << pos.width << " h=" << pos.height << std::endl;
  }

  // Find the closest node to the click position
  std::shared_ptr<scene_graph::Node> closestNode = nullptr;
  float minDistance = std::numeric_limits<float>::max();

  for (const auto &nodePos : nodePositions_) {
    // Skip nodes with zero width (outside view)
    if (nodePos.width == 0)
      continue;

    // Check if within horizontal bounds
    if (position.x >= nodePos.x && position.x <= nodePos.x + nodePos.width) {
      // Calculate vertical distance to node center
      float nodeCenter = nodePos.visualY + (nodePos.height / 2.0f);
      float distance = std::abs(position.y - nodeCenter);

      // If this is closer than previous best match, update
      if (distance < minDistance) {
        minDistance = distance;
        closestNode = nodePos.node;
      }
    }
  }

  // If we found a node within reasonable distance, select it
  if (closestNode && minDistance < constants::TREE_VERT_SPACING * 0.7f) {
    selectedNode_ = closestNode;
    std::cout << "Selected node: " << closestNode->getName()
              << " (distance: " << minDistance << ")" << std::endl;
  } else {
    // No node was clicked or too far from any node
    std::cout << "No node selected (min distance: " << minDistance << ")"
              << std::endl;
  }
}

std::shared_ptr<scene_graph::Node> TreeView::getSelectedNode() const {
  return selectedNode_;
}

void TreeView::scroll(float amount) {
  // Calculate the new scroll position
  float newScrollPosition = scrollPosition_ + amount;

  // Limit scrolling to content boundaries
  float minScroll = -std::max(0.0f, contentHeight_ - visibleHeight_);
  newScrollPosition = std::min(0.0f, std::max(minScroll, newScrollPosition));

  // Update the scroll position
  scrollPosition_ = newScrollPosition;
}

bool TreeView::isPointInScrollBar(const Vector2 &point) const {
  float treeX = -constants::SCENE_HALF_WIDTH;
  float treeWidth = constants::TREE_VIEW_WIDTH;
  float scrollBarX = treeX + treeWidth - scrollBarWidth_ - 0.1f;

  // Check if point is within scrollbar bounds
  return (point.x >= scrollBarX && point.x <= scrollBarX + scrollBarWidth_ &&
          point.y >= -constants::SCENE_HALF_HEIGHT &&
          point.y <= constants::SCENE_HALF_HEIGHT - 0.3f);
}

void TreeView::startScrollDrag(const Vector2 &position) {
  isScrolling_ = true;
  scrollStartPosition_ = position.y;
}

void TreeView::updateScrollDrag(const Vector2 &position) {
  if (!isScrolling_)
    return;

  // Calculate drag delta
  float delta = position.y - scrollStartPosition_;

  // Scale the delta based on content height
  float scaledDelta = delta * (contentHeight_ / visibleHeight_);

  // Update scroll position
  scroll(scaledDelta);

  // Update start position for next drag update
  scrollStartPosition_ = position.y;
}

void TreeView::endScrollDrag() { isScrolling_ = false; }

} // namespace visualization