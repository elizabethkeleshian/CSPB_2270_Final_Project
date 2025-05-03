// include/application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include "constants.h"
#include "scene_graph/circle.h"
#include "scene_graph/node.h"
#include "scene_graph/rectangle.h"
#include "visualization/canvas.h"
#include "visualization/renderer.h"
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

private:
  // Window properties
  static constexpr int WINDOW_WIDTH = constants::DEFAULT_WINDOW_WIDTH;
  static constexpr int WINDOW_HEIGHT = constants::DEFAULT_WINDOW_HEIGHT;
  static constexpr char WINDOW_TITLE[] = "Scene Graph Visualization";

  // Input handling
  void setupInputCallbacks();
  void handleMouseMoved(double xpos, double ypos);
  void handleMouseButton(int button, int action, int mods);
  [[nodiscard]] scene_graph::Vector2
  windowToSceneCoordinates(double xpos, double ypos) const;

  // Scene graph setup
  void setupSceneGraph();
  std::shared_ptr<scene_graph::Node>
  createRectangle(const std::string &name, const scene_graph::Vector2 &size,
                  const scene_graph::Vector2 &position,
                  const scene_graph::Vector4 &color);
  std::shared_ptr<scene_graph::Node>
  createCircle(const std::string &name, float radius,
               const scene_graph::Vector2 &position,
               const scene_graph::Vector4 &color);

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
  scene_graph::Vector2 lastMousePos_;

  // Animation state
  float animationTime_;

  // Car creation
  std::shared_ptr<scene_graph::Node>
  createCar(const std::string &name, const scene_graph::Vector2 &position,
            const scene_graph::Vector4 &bodyColor);
};

#endif // APPLICATION_H
