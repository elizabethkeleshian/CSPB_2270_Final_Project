// include/application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include "constants.h"
#include "scene_graph/node.h"
#include "types.h"
#include "visualization/canvas.h"
#include "visualization/renderer.h"
#include "visualization/tree_view.h"
#include "visualization/window.h"
#include <memory>

/**
 * @brief Main application class
 *
 * This class is responsible for running the application.
 * It creates the window, renderer, and canvas, and then runs the main loop.
 */
class Application {
public:
  Application();
  ~Application() = default;

  bool initialize();
  void run();
  void toggleTreeView();
  void syncSelectionWithCanvas();

private:
  // Window properties
  static constexpr int WINDOW_WIDTH = constants::DEFAULT_WINDOW_WIDTH;
  static constexpr int WINDOW_HEIGHT = constants::DEFAULT_WINDOW_HEIGHT;
  static constexpr const char *WINDOW_TITLE = constants::WINDOW_TITLE;

  // Input handling
  void setupInputCallbacks();
  void handleMouseMoved(double xpos, double ypos);
  void handleMouseButton(int button, int action, int mods);
  [[nodiscard]] Vector2 windowToSceneCoordinates(double xpos,
                                                 double ypos) const;
  void printSceneHierarchy(const std::shared_ptr<scene_graph::Node> &node,
                           int depth);
  void printOpenGLInfo();

  // Scene graph setup
  void setupSceneGraph();
  std::shared_ptr<scene_graph::Node> createRectangle(const std::string &name,
                                                     const Vector2 &size,
                                                     const Vector2 &position,
                                                     const Vector4 &color);
  std::shared_ptr<scene_graph::Node> createCircle(const std::string &name,
                                                  float radius,
                                                  const Vector2 &position,
                                                  const Vector4 &color);

  // Animation
  void updateAnimations(float deltaTime);

  // Components
  std::unique_ptr<visualization::Window> window_;
  std::shared_ptr<visualization::Renderer> renderer_;
  std::shared_ptr<visualization::Canvas> canvas_;
  std::shared_ptr<scene_graph::Node> root_;

  // Interaction state
  bool isDragging_;
  std::shared_ptr<scene_graph::Node> draggedNode_;
  Vector2 lastMousePos_;

  // Animation state
  float animationTime_;

  // Car creation
  std::shared_ptr<scene_graph::Node> createCar(const std::string &name,
                                               const Vector2 &position,
                                               const Vector4 &bodyColor);

  std::shared_ptr<visualization::TreeView> treeView_;
  bool showTreeView_ = true; // Toggle for showing/hiding tree view
};

#endif // APPLICATION_H
