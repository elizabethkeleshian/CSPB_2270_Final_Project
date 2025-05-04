#include "application.h"
#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include "types.h"
#include "visualization/canvas.h"
#include "visualization/renderer.h"
#include "visualization/tree_view.h"
#include "visualization/window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application()
    : window_(std::make_unique<visualization::Window>()),
      renderer_(std::make_shared<visualization::Renderer>()),
      canvas_(std::make_shared<visualization::Canvas>()),
      root_(std::make_shared<scene_graph::Node>("Root")), isDragging_(false),
      treeView_(std::make_shared<visualization::TreeView>()),
      draggedNode_(nullptr), showTreeView_(true) {}

bool Application::initialize() {
  try {
    if (!window_->create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) {
      std::cerr << "Failed to create window\n";
      return false;
    }

    GLenum err = glewInit();
    if (err != GLEW_OK) {
      std::cerr << "GLEW initialization failed: " << glewGetErrorString(err)
                << std::endl;
      return false;
    }
    // Initialize renderer
    try {
      if (!renderer_->initialize()) {
        std::cerr << "Failed to initialize renderer!\n";
        return false;
      }
    } catch (const std::exception &e) {
      std::cerr << "Exception in renderer initialization: " << e.what() << "\n";
      return false;
    } catch (...) {
      std::cerr << "Unknown exception in renderer initialization\n";
      return false;
    }

    renderer_->setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize canvas
    try {
      if (!canvas_->initialize(renderer_)) {
        std::cerr << "Failed to initialize canvas!\n";
        return false;
      }
    } catch (const std::exception &e) {
      std::cerr << "Exception in canvas initialization: " << e.what() << "\n";
      return false;
    } catch (...) {
      std::cerr << "Unknown exception in canvas initialization\n";
      return false;
    }

    treeView_->setRoot(root_);
    treeView_->setTextRenderer(renderer_);
    treeView_->setRenderer(renderer_);

    // Setup scene graph
    try {
      setupSceneGraph();
    } catch (const std::exception &e) {
      std::cerr << "Exception in setupSceneGraph: " << e.what() << "\n";
      return false;
    } catch (...) {
      std::cerr << "Unknown exception in setupSceneGraph\n";
      return false;
    }

    // Set the root in canvas
    canvas_->setRoot(root_);

    // Setup input callbacks
    setupInputCallbacks();

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Exception in application initialization: " << e.what()
              << "\n";
    return false;
  } catch (...) {
    std::cerr << "Unknown exception in application initialization\n";
    return false;
  }
}

void Application::toggleTreeView() { showTreeView_ = !showTreeView_; }

void Application::syncSelectionWithCanvas() {
  // When selecting in tree view, update canvas selection
  if (treeView_ && treeView_->getSelectedNode()) {
    canvas_->selectNode(treeView_->getSelectedNode());
  }
  // When selecting in canvas, update tree view selection
  else if (canvas_ && canvas_->getSelectedNode()) {
    if (treeView_) {
      // This would need a method to set selection directly
      // treeView_->setSelectedNode(canvas_->getSelectedNode());
    }
  }
}

void Application::setupInputCallbacks() {
  // Mouse movement callback
  window_->setMouseCallback(
      [this](double xpos, double ypos) { handleMouseMoved(xpos, ypos); });

  // Mouse button callback
  window_->setMouseButtonCallback([this](int button, int action, int mods) {
    handleMouseButton(button, action, mods);
  });

  window_->setKeyCallback([this](int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
      toggleTreeView();
    }
  });
}

void Application::handleMouseMoved(double xpos, double ypos) {
  Vector2 mousePos = windowToSceneCoordinates(xpos, ypos);

  if (isDragging_ && draggedNode_) {
    // Calculate delta movement
    Vector2 delta = mousePos - lastMousePos_;

    // Get current position and update it
    Vector2 currentPos = draggedNode_->getPosition();
    draggedNode_->setPosition(currentPos + delta);

    // Check if we're dealing with a car or car body
    bool isTopLevelCar =
        draggedNode_->getName().find("Car") != std::string::npos &&
        draggedNode_->getName().find("Body") == std::string::npos;
    bool isCarBody = draggedNode_->getName().find("Body") != std::string::npos;

    if (isTopLevelCar || isCarBody) {
      // Calculate a rotation factor based on horizontal movement
      float rotationFactor =
          delta.x *
          constants::WHEEL_ROTATION_FACTOR; // Using constant from constants.h

      // Apply rotation to wheel children - need to traverse the hierarchy
      // correctly
      auto wheelSearch =
          [&rotationFactor](const std::shared_ptr<scene_graph::Node> &node) {
            if (node->getName().find("Wheel") != std::string::npos) {
              float currentRotation = node->getRotation();
              node->setRotation(currentRotation -
                                rotationFactor); // Negative to rotate correctly
            }

            // Recursively search children for wheels
            for (const auto &child : node->getChildren()) {
              if (child->getName().find("Wheel") != std::string::npos) {
                float currentRotation = child->getRotation();
                child->setRotation(currentRotation - rotationFactor);
              }
            }
          };

      // If it's a top-level car node, search its children
      if (isTopLevelCar) {
        for (const auto &child : draggedNode_->getChildren()) {
          wheelSearch(child);
        }
      } else {
        // If it's a car body, search direct children
        for (const auto &child : draggedNode_->getChildren()) {
          wheelSearch(child);
        }
      }
    }

    // Update last position
    lastMousePos_ = mousePos;
  }
}

void Application::handleMouseButton(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    double xpos, ypos;
    glfwGetCursorPos(static_cast<GLFWwindow *>(window_->getWindowHandle()),
                     &xpos, &ypos);

    Vector2 mousePos = windowToSceneCoordinates(xpos, ypos);

    // Add debugging
    std::cout << "Mouse clicked at window: (" << xpos << ", " << ypos << ")"
              << std::endl;
    std::cout << "Converted to scene: (" << mousePos.x << ", " << mousePos.y
              << ")" << std::endl;

    // Check if click is in tree view area (left side of screen)
    // In handleMouseButton method
    bool clickedInTreeView =
        showTreeView_ &&
        mousePos.x < -constants::SCENE_HALF_WIDTH + constants::TREE_VIEW_WIDTH;
    if (clickedInTreeView) {
      std::cout << "Clicked in tree view area" << std::endl;
    }

    if (action == GLFW_PRESS) {
      if (clickedInTreeView && treeView_) {
        // Handle tree view selection
        treeView_->selectAt(mousePos);
        // Sync with canvas
        if (treeView_->getSelectedNode()) {
          canvas_->selectNode(treeView_->getSelectedNode());
          // Don't start dragging when selecting in tree view
          return;
        }
      } else {
        // Handle canvas selection/dragging as before
        auto hitNode = canvas_->hitTest(mousePos);

        // If no node was hit directly, check if we're within a car's bounding
        // area
        if (!hitNode && root_) {
          // Check direct children of root (top-level objects)
          for (const auto &child : root_->getChildren()) {
            // For car objects, we can use a rough rectangular hit test
            if (child->getName().find("Car") != std::string::npos) {
              // Get car position
              Vector2 carPos = child->getPosition();

              // Simple rectangular hit test using car body dimensions
              float halfWidth = constants::CAR_BODY_WIDTH / 2.0f;
              float halfHeight = constants::CAR_BODY_HEIGHT / 2.0f;

              // Check if point is inside car's bounding box, accounting for
              // rotation
              float carRot =
                  child->getRotation() * M_PI / 180.0f; // Convert to radians

              // Translate point to car's local space
              Vector2 localPos = mousePos - carPos;

              // Rotate point to account for car's rotation
              float cosRot = cos(-carRot);
              float sinRot = sin(-carRot);
              Vector2 rotatedPos(localPos.x * cosRot - localPos.y * sinRot,
                                 localPos.x * sinRot + localPos.y * cosRot);

              // Check if rotated point is within bounds
              if (rotatedPos.x >= -halfWidth && rotatedPos.x <= halfWidth &&
                  rotatedPos.y >= -halfHeight && rotatedPos.y <= halfHeight) {
                hitNode = child;
                break;
              }
            }
          }
        }

        if (hitNode) {
          isDragging_ = true;
          draggedNode_ = hitNode;
          lastMousePos_ = mousePos;
          canvas_->selectNode(hitNode);

          // Sync tree view with canvas selection
          if (treeView_) {
            treeView_->setSelectedNode(hitNode);
          }
        } else {
          canvas_->selectNode(nullptr);

          // Sync tree view with canvas selection (clear selection)
          if (treeView_) {
            treeView_->setSelectedNode(nullptr);
          }
        }
      }
    } else if (action == GLFW_RELEASE) {
      // Stop dragging
      isDragging_ = false;
      draggedNode_ = nullptr;
    }
  }
}

Vector2 Application::windowToSceneCoordinates(double xpos, double ypos) const {
  // Use consistent scaling for both dimensions
  float aspectRatio =
      static_cast<float>(window_->getWidth()) / window_->getHeight();
  float sceneX = ((float)xpos / window_->getWidth() * constants::SCENE_WIDTH) -
                 constants::SCENE_HALF_WIDTH;
  float sceneY =
      constants::SCENE_HALF_HEIGHT - ((float)ypos / window_->getHeight() *
                                      (constants::SCENE_WIDTH / aspectRatio));

  return Vector2(sceneX, sceneY);
}

void Application::setupSceneGraph() {
  // Create a red car
  auto redCar = createCar(
      "RedCar", Vector2(constants::RED_CAR_START_X, constants::RED_CAR_START_Y),
      Vector4(constants::colors::RED_CAR[0], constants::colors::RED_CAR[1],
              constants::colors::RED_CAR[2], constants::colors::RED_CAR[3]));

  auto blueCar = createCar(
      "BlueCar",
      Vector2(constants::BLUE_CAR_START_X, constants::BLUE_CAR_START_Y),
      Vector4(constants::colors::BLUE_CAR[0], constants::colors::BLUE_CAR[1],
              constants::colors::BLUE_CAR[2], constants::colors::BLUE_CAR[3]));

  // Set up hierarchy - the cars are now direct children of the root
  root_->addChild(redCar);
  root_->addChild(blueCar);
}

std::shared_ptr<scene_graph::Node>
Application::createRectangle(const std::string &name, const Vector2 &size,
                             const Vector2 &position, const Vector4 &color) {
  auto rect = std::make_shared<scene_graph::Rectangle>(name, size);
  rect->setPosition(position);
  rect->setColor(color);
  return rect;
}

std::shared_ptr<scene_graph::Node>
Application::createCircle(const std::string &name, float radius,
                          const Vector2 &position, const Vector4 &color) {
  auto circle = std::make_shared<scene_graph::Circle>(name, radius);
  circle->setPosition(position);
  circle->setColor(color);
  return circle;
}

void Application::updateAnimations(float deltaTime) {
  // // Update animation time
  // animationTime_ += deltaTime;

  // // Find cars by name
  // for (const auto &child : root_->getChildren()) {
  //   if (child->getName() == "RedCar") {
  //     // Move the red car in a circle
  //     float radius = constants::RED_CAR_CIRCLE_RADIUS;
  //     float speed = constants::RED_CAR_MOVEMENT_SPEED;
  //     child->setPosition(Vector2(cos(animationTime_ * speed) * radius,
  //                                sin(animationTime_ * speed) * radius));

  //     // Rotate the car to face the direction of movement
  //     float angle =
  //         atan2(sin(animationTime_ * speed), cos(animationTime_ * speed));
  //     angle =
  //         angle * 180.0f / M_PI +
  //         constants::CAR_ROTATION_ADJUSTMENT; // Convert to degrees and
  //         adjust
  //     child->setRotation(angle);

  //     // Find the body to update the wheels
  //     for (const auto &carPart : child->getChildren()) {
  //       if (carPart->getName() == "RedCar_Body") {
  //         // Rotate wheels
  //         for (const auto &bodyPart : carPart->getChildren()) {
  //           if (bodyPart->getName().find("Wheel") != std::string::npos) {
  //             float currentRotation = bodyPart->getRotation();
  //             bodyPart->setRotation(currentRotation +
  //                                   constants::WHEEL_ROTATION_SPEED *
  //                                       deltaTime);
  //           }
  //         }
  //       }
  //     }
  //   } else if (child->getName() == "BlueCar") {
  //     // Move the blue car back and forth
  //     float xPos = constants::BLUE_CAR_OSCILLATION_CENTER +
  //                  sin(animationTime_ * constants::CAR_MOVEMENT_SPEED) *
  //                      constants::BLUE_CAR_OSCILLATION_AMPLITUDE;
  //     child->setPosition(Vector2(xPos, 0.0f));

  //     // Rotate wheels (speed depends on movement direction)
  //     float wheelDirection =
  //         -cos(animationTime_ * constants::CAR_MOVEMENT_SPEED);

  //     // Find the body to update the wheels
  //     for (const auto &carPart : child->getChildren()) {
  //       if (carPart->getName() == "BlueCar_Body") {
  //         // Rotate wheels
  //         for (const auto &bodyPart : carPart->getChildren()) {
  //           if (bodyPart->getName().find("Wheel") != std::string::npos) {
  //             float currentRotation = bodyPart->getRotation();
  //             bodyPart->setRotation(
  //                 currentRotation +
  //                 wheelDirection * constants::WHEEL_ROTATION_SPEED *
  //                 deltaTime);
  //           }
  //         }
  //       }
  //     }
  //   }
  // }
}

void Application::run() {
  try {

    std::cout << "Application starting" << std::endl;

    // Timing variables
    float lastFrameTime = 0.0f;

    std::cout << "Scene hierarchy:" << std::endl;
    printSceneHierarchy(root_, 0);

    // Main loop
    while (!window_->shouldClose()) {
      try {
        // Update time and calculate delta time
        auto currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        try {
          // Update animations
          updateAnimations(deltaTime);
        } catch (const std::exception &e) {
          std::cerr << "Exception in updateAnimations: " << e.what() << "\n";
          break;
        } catch (...) {
          std::cerr << "Unknown exception in updateAnimations\n";
          break;
        }

        try {
          // Render
          canvas_->render();
        } catch (const std::exception &e) {
          std::cerr << "Exception in canvas rendering: " << e.what() << "\n";
          break;
        } catch (...) {
          std::cerr << "Unknown exception in canvas rendering\n";
          break;
        }

        // Check if tree view is properly initialized
        if (!treeView_ || !treeView_->getRoot()) {
          std::cerr << "TreeView not properly initialized" << std::endl;
          // Handle error
        }

        // Render tree view if enabled
        if (showTreeView_) {
          try {
            treeView_->render();
          } catch (const std::exception &e) {
            std::cerr << "Exception in tree view rendering: " << e.what()
                      << "\n";
            break;
          } catch (...) {
            std::cerr << "Unknown exception in tree view rendering\n";
            break;
          }
        }

        // Swap buffers and poll events
        window_->swapBuffers();
        window_->pollEvents();
      } catch (const std::exception &e) {
        std::cerr << "Exception in main loop: " << e.what() << "\n";
        break;
      } catch (...) {
        std::cerr << "Unknown exception in main loop\n";
        break;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception in run method: " << e.what() << "\n";
  } catch (...) {
    std::cerr << "Unknown exception in run method\n";
  }
}

void Application::printOpenGLInfo() {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

std::shared_ptr<scene_graph::Node>
Application::createCar(const std::string &name, const Vector2 &position,
                       const Vector4 &bodyColor) {

  // Create a parent node for the entire car
  auto car = std::make_shared<scene_graph::Node>(name);
  car->setPosition(position);

  // Create the car body
  auto carBody = createRectangle(
      name + "_Body",
      Vector2(constants::CAR_BODY_WIDTH, constants::CAR_BODY_HEIGHT),
      Vector2(0.0f, 0.0f), // Position is relative to car
      bodyColor);

  // Create car roof (child of body)
  auto carRoof = createRectangle(
      name + "_Roof",
      Vector2(constants::CAR_BODY_WIDTH * constants::CAR_ROOF_WIDTH_FACTOR,
              constants::CAR_BODY_HEIGHT * constants::CAR_ROOF_HEIGHT_FACTOR),
      Vector2(
          0.0f,
          constants::CAR_BODY_HEIGHT *
              constants::CAR_ROOF_POSITION_FACTOR), // Position relative to body
      bodyColor);
  // Create car wheels (children of body)
  auto frontWheel = createCircle(
      name + "_FrontWheel", constants::CAR_WHEEL_RADIUS,
      Vector2(constants::CAR_WHEEL_OFFSET_X, constants::CAR_WHEEL_OFFSET_Y),
      Vector4(constants::colors::CAR_WHEEL[0], constants::colors::CAR_WHEEL[1],
              constants::colors::CAR_WHEEL[2],
              constants::colors::CAR_WHEEL[3]));

  auto rearWheel = createCircle(
      name + "_RearWheel", constants::CAR_WHEEL_RADIUS,
      Vector2(-constants::CAR_WHEEL_OFFSET_X, constants::CAR_WHEEL_OFFSET_Y),
      Vector4(constants::colors::CAR_WHEEL[0], constants::colors::CAR_WHEEL[1],
              constants::colors::CAR_WHEEL[2],
              constants::colors::CAR_WHEEL[3]));

  // Add details to wheels (hubcaps)
  auto frontHubcap = createCircle(
      name + "_FrontHubcap",
      constants::CAR_WHEEL_RADIUS * constants::CAR_HUBCAP_RADIUS_FACTOR,
      Vector2(0.0f, 0.0f), // Center of wheel
      Vector4(
          constants::colors::CAR_HUBCAP[0], constants::colors::CAR_HUBCAP[1],
          constants::colors::CAR_HUBCAP[2], constants::colors::CAR_HUBCAP[3]));

  auto rearHubcap = createCircle(
      name + "_RearHubcap",
      constants::CAR_WHEEL_RADIUS * constants::CAR_HUBCAP_RADIUS_FACTOR,
      Vector2(0.0f, 0.0f), // Center of wheel
      Vector4(
          constants::colors::CAR_HUBCAP[0], constants::colors::CAR_HUBCAP[1],
          constants::colors::CAR_HUBCAP[2], constants::colors::CAR_HUBCAP[3]));

  // Setup hierarchy
  car->addChild(carBody); // Attach body to car
  carBody->addChild(carRoof);
  carBody->addChild(frontWheel);
  carBody->addChild(rearWheel);
  frontWheel->addChild(frontHubcap);
  rearWheel->addChild(rearHubcap);

  return car;
}

void Application::printSceneHierarchy(
    const std::shared_ptr<scene_graph::Node> &node, int depth) {
  if (!node)
    return;

  // Print indentation
  for (int i = 0; i < depth; i++) {
    std::cout << "  ";
  }

  // Print node info
  std::cout << node->getName() << " (pos: " << node->getPosition().x << ", "
            << node->getPosition().y << ")" << std::endl;

  // Print children
  for (const auto &child : node->getChildren()) {
    printSceneHierarchy(child, depth + 1);
  }
}
