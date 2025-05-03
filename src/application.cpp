#include "application.h"
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application()
    : window_(std::make_unique<visualization::Window>()),
      renderer_(std::make_shared<visualization::Renderer>()),
      canvas_(std::make_shared<visualization::Canvas>()),
      root_(std::make_shared<scene_graph::Node>("Root")), isDragging_(false),
      draggedNode_(nullptr), animationTime_(0.0f) {}

bool Application::initialize() {
  if (!window_->create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) {
    std::cerr << "Failed to create window\n";
    return false;
  }
  // Initialize renderer
  if (!renderer_->initialize()) {
    std::cerr << "Failed to initialize renderer!\n";
    return false;
  }
  renderer_->setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Initialize canvas
  if (!canvas_->initialize(renderer_)) {
    std::cerr << "Failed to initialize canvas!\n";
    return false;
  }

  // Setup scene graph
  setupSceneGraph();

  // Set the root in canvas
  canvas_->setRoot(root_);

  // Setup input callbacks
  setupInputCallbacks();

  return true;
}

void Application::setupInputCallbacks() {
  // Mouse movement callback
  window_->setMouseCallback(
      [this](double xpos, double ypos) { handleMouseMoved(xpos, ypos); });

  // Mouse button callback
  window_->setMouseButtonCallback([this](int button, int action, int mods) {
    handleMouseButton(button, action, mods);
  });

  // Keyboard callback (could add this if needed)
  window_->setKeyCallback([this](int key, int scancode, int action, int mods) {
    // Handle keyboard input if needed
  });
}

void Application::handleMouseMoved(double xpos, double ypos) {
  scene_graph::Vector2 mousePos = windowToSceneCoordinates(xpos, ypos);

  if (isDragging_ && draggedNode_) {
    // Calculate delta movement
    scene_graph::Vector2 delta = mousePos - lastMousePos_;

    // Get current position and update it
    scene_graph::Vector2 currentPos = draggedNode_->getPosition();
    draggedNode_->setPosition(currentPos + delta);

    // Update last position
    lastMousePos_ = mousePos;
  }
}

void Application::handleMouseButton(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    double xpos, ypos;
    glfwGetCursorPos(static_cast<GLFWwindow *>(window_->getWindowHandle()),
                     &xpos, &ypos);

    scene_graph::Vector2 mousePos = windowToSceneCoordinates(xpos, ypos);

    if (action == GLFW_PRESS) {
      // Start dragging
      auto hitNode = canvas_->hitTest(mousePos);
      if (hitNode) {
        isDragging_ = true;
        draggedNode_ = hitNode;
        lastMousePos_ = mousePos;
        canvas_->selectNode(hitNode);
      } else {
        canvas_->selectNode(nullptr);
      }
    } else if (action == GLFW_RELEASE) {
      // Stop dragging
      isDragging_ = false;
      draggedNode_ = nullptr;
    }
  }
}

scene_graph::Vector2 Application::windowToSceneCoordinates(double xpos,
                                                           double ypos) const {
  float sceneX = static_cast<float>(xpos) / window_->getWidth() * 20.0f - 10.0f;
  float sceneY =
      10.0f - static_cast<float>(ypos) / window_->getHeight() * 20.0f;
  return scene_graph::Vector2(sceneX, sceneY);
}

void Application::setupSceneGraph() {
  // Create a red car
  auto redCar = createCar("RedCar", scene_graph::Vector2(-3.0f, 0.0f),
                          scene_graph::Vector4(0.8f, 0.2f, 0.2f, 1.0f) // Red
  );

  // Create a blue car
  auto blueCar = createCar("BlueCar", scene_graph::Vector2(3.0f, 0.0f),
                           scene_graph::Vector4(0.2f, 0.4f, 0.8f, 1.0f) // Blue
  );

  // Set up hierarchy
  root_->addChild(redCar);
  root_->addChild(blueCar);
}

std::shared_ptr<scene_graph::Node> Application::createRectangle(
    const std::string &name, const scene_graph::Vector2 &size,
    const scene_graph::Vector2 &position, const scene_graph::Vector4 &color) {

  auto rect = std::make_shared<scene_graph::Rectangle>(name, size);
  rect->setPosition(position);
  rect->setColor(color);
  return rect;
}

std::shared_ptr<scene_graph::Node>
Application::createCircle(const std::string &name, float radius,
                          const scene_graph::Vector2 &position,
                          const scene_graph::Vector4 &color) {

  auto circle = std::make_shared<scene_graph::Circle>(name, radius);
  circle->setPosition(position);
  circle->setColor(color);
  return circle;
}

void Application::updateAnimations(float deltaTime) {
  // Update animation time
  animationTime_ += deltaTime;

  // Find cars by name
  for (const auto &child : root_->getChildren()) {
    if (child->getName() == "RedCar_Body") {
      // Move the red car in a circle
      float radius = 4.0f;
      float speed = 0.5f;
      child->setPosition(
          scene_graph::Vector2(cos(animationTime_ * speed) * radius,
                               sin(animationTime_ * speed) * radius));

      // Rotate the car to face the direction of movement
      float angle =
          atan2(sin(animationTime_ * speed), cos(animationTime_ * speed));
      angle = angle * 180.0f / M_PI + 90.0f; // Convert to degrees and adjust
      child->setRotation(angle);

      // Rotate wheels
      for (const auto &carPart : child->getChildren()) {
        if (carPart->getName().find("Wheel") != std::string::npos) {
          float currentRotation = carPart->getRotation();
          carPart->setRotation(currentRotation +
                               constants::WHEEL_ROTATION_SPEED * deltaTime);
        }
      }
    } else if (child->getName() == "BlueCar_Body") {
      // Move the blue car back and forth
      float xPos =
          3.0f + sin(animationTime_ * constants::CAR_MOVEMENT_SPEED) * 3.0f;
      child->setPosition(scene_graph::Vector2(xPos, 0.0f));

      // Rotate wheels (speed depends on movement direction)
      float wheelDirection =
          -cos(animationTime_ * constants::CAR_MOVEMENT_SPEED);

      for (const auto &carPart : child->getChildren()) {
        if (carPart->getName().find("Wheel") != std::string::npos) {
          float currentRotation = carPart->getRotation();
          carPart->setRotation(currentRotation +
                               wheelDirection *
                                   constants::WHEEL_ROTATION_SPEED * deltaTime);
        }
      }
    }
  }
}

void Application::run() {
  // Timing variables
  float lastFrameTime = 0.0f;

  // Main loop
  while (!window_->shouldClose()) {
    // Update time and calculate delta time
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    // Update animations
    updateAnimations(deltaTime);

    // Render
    canvas_->render();

    // Swap buffers and poll events
    window_->swapBuffers();
    window_->pollEvents();
  }
}

std::shared_ptr<scene_graph::Node>
Application::createCar(const std::string &name,
                       const scene_graph::Vector2 &position,
                       const scene_graph::Vector4 &bodyColor) {

  // Create the car body (parent node)
  auto carBody =
      createRectangle(name + "_Body",
                      scene_graph::Vector2(constants::CAR_BODY_WIDTH,
                                           constants::CAR_BODY_HEIGHT),
                      position, bodyColor);

  // Create car roof (child of body)
  auto carRoof = createRectangle(
      name + "_Roof",
      scene_graph::Vector2(constants::CAR_BODY_WIDTH * 0.5f,
                           constants::CAR_BODY_HEIGHT * 0.8f),
      scene_graph::Vector2(0.0f, constants::CAR_BODY_HEIGHT *
                                     0.7f), // Position relative to body
      bodyColor);
  // Create car wheels (children of body)
  auto frontWheel = createCircle(
      name + "_FrontWheel", constants::CAR_WHEEL_RADIUS,
      scene_graph::Vector2(constants::CAR_WHEEL_OFFSET_X,
                           constants::CAR_WHEEL_OFFSET_Y),
      scene_graph::Vector4(0.2f, 0.2f, 0.2f, 1.0f) // Dark gray/black
  );

  auto rearWheel = createCircle(
      name + "_RearWheel", constants::CAR_WHEEL_RADIUS,
      scene_graph::Vector2(-constants::CAR_WHEEL_OFFSET_X,
                           constants::CAR_WHEEL_OFFSET_Y),
      scene_graph::Vector4(0.2f, 0.2f, 0.2f, 1.0f) // Dark gray/black
  );

  // Add details to wheels (hubcaps)
  auto frontHubcap =
      createCircle(name + "_FrontHubcap", constants::CAR_WHEEL_RADIUS * 0.4f,
                   scene_graph::Vector2(0.0f, 0.0f), // Center of wheel
                   scene_graph::Vector4(0.8f, 0.8f, 0.8f, 1.0f) // Silver
      );

  auto rearHubcap =
      createCircle(name + "_RearHubcap", constants::CAR_WHEEL_RADIUS * 0.4f,
                   scene_graph::Vector2(0.0f, 0.0f), // Center of wheel
                   scene_graph::Vector4(0.8f, 0.8f, 0.8f, 1.0f) // Silver
      );

  // Setup hierarchy
  carBody->addChild(carRoof);
  carBody->addChild(frontWheel);
  carBody->addChild(rearWheel);
  frontWheel->addChild(frontHubcap);
  rearWheel->addChild(rearHubcap);

  return carBody;
}
