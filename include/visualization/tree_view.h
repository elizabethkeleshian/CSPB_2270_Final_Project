#pragma once

#include "constants.h"
#include "scene_graph/node.h"
#include "types.h"
#include <utility> // For std::move

namespace visualization {

class TreeView {
public:
  TreeView();
  ~TreeView();

  // disable copy constructor and assignment operator
  TreeView(const TreeView &) = delete;
  TreeView &operator=(const TreeView &) = delete;

  // disable move constructor and assignment operator
  TreeView(TreeView &&) = delete;
  TreeView &operator=(TreeView &&) = delete;

  void setRoot(const std::shared_ptr<scene_graph::Node> &root);
  [[nodiscard]] std::shared_ptr<scene_graph::Node> getRoot() const;
  void render();

  void setTextRenderer(std::shared_ptr<Renderer> renderer) {
    textRenderer_ = std::move(renderer);
  }

  void setRenderer(std::shared_ptr<Renderer> renderer) {
    renderer_ = std::move(renderer);
  }

  void selectAt(const Vector2 &position);
  [[nodiscard]] std::shared_ptr<scene_graph::Node> getSelectedNode() const;

  // Set the selected node directly
  void setSelectedNode(const std::shared_ptr<scene_graph::Node> &node) {
    selectedNode_ = node;
  }

  // Scroll methods
  void scroll(float amount);
  bool isPointInScrollBar(const Vector2 &point) const;
  void startScrollDrag(const Vector2 &position);
  void updateScrollDrag(const Vector2 &position);
  void endScrollDrag();

  // Check if we're currently scrolling
  [[nodiscard]] bool isScrolling() const { return isScrolling_; }

private:
  // helper to render node and its children
  void renderNode(const std::shared_ptr<scene_graph::Node> &node, int depth,
                  int &yPosition);

  // Helper to count nodes in a subtree (for layout)
  [[nodiscard]] int
  countNodes(const std::shared_ptr<scene_graph::Node> &node) const;

  // Helper to render scrollbar
  void renderScrollBar();

  // Helper to calculate total content height
  void calculateContentHeight();

  std::shared_ptr<scene_graph::Node> root_;
  std::shared_ptr<scene_graph::Node> selectedNode_;
  std::shared_ptr<Renderer> renderer_;
  std::shared_ptr<Renderer> textRenderer_;

  struct NodePosition {
    std::shared_ptr<scene_graph::Node> node;
    int x;
    int y;
    int width;
    int height;
    float visualY; // Actual visual Y position
  };

  std::vector<NodePosition> nodePositions_;

  // Scrolling properties
  float scrollPosition_ = 0.0f;
  float contentHeight_ = 0.0f;
  float visibleHeight_ = 0.0f;
  bool isScrolling_ = false;
  float scrollStartPosition_ = 0.0f;
  float scrollBarWidth_ = 0.2f;
  float scrollBarMinHeight_ = 0.5f;
};

} // namespace visualization
