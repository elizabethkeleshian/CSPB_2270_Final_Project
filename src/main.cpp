#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include "visualization/canvas.h"
#include "visualization/renderer.h"
#include "visualization/window.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
  // Create window
  auto window = std::make_unique<visualization::Window>();
  if (!window->create(800, 600, "Scene Graph Visualization")) {
    std::cerr << "Failed to create window!\n";
    return -1;
  }

  // Create renderer
  auto renderer = std::make_shared<visualization::Renderer>();
  if (!renderer->initialize()) {
    std::cerr << "Failed to initialize renderer!\n";
    return -1;
  }
  renderer->setViewport(800, 600);

  // Create canvas
  auto canvas = std::make_shared<visualization::Canvas>();
  if (!canvas->initialize(renderer)) {
    std::cerr << "Failed to initialize canvas!" << std::endl;
    return -1;
  }

  // Create scene graph
  auto root = std::make_shared<scene_graph::Node>("Root");

  // Create some shapes
  auto redRect = std::make_shared<scene_graph::Rectangle>(
      "RedRectangle", scene_graph::Vector2(2.0f, 1.0f));
  redRect->setPosition(scene_graph::Vector2(-3.0f, 0.0f));
  redRect->setColor(scene_graph::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

  auto blueCircle = std::make_shared<scene_graph::Circle>("BlueCircle", 1.0f);
  blueCircle->setPosition(scene_graph::Vector2(3.0f, 0.0f));
  blueCircle->setColor(scene_graph::Vector4(0.0f, 0.0f, 1.0f, 1.0f));

  // Create a parent-child relationship (green rectangle child of root)
  auto greenRect = std::make_shared<scene_graph::Rectangle>(
      "GreenRectangle", scene_graph::Vector2(1.0f, 1.0f));
  greenRect->setPosition(scene_graph::Vector2(0.0f, 2.0f));
  greenRect->setColor(scene_graph::Vector4(0.0f, 1.0f, 0.0f, 1.0f));

  // Add yellow circle as child of green rectangle
  auto yellowCircle =
      std::make_shared<scene_graph::Circle>("YellowCircle", 0.5f);
  yellowCircle->setPosition(scene_graph::Vector2(0.0f, 0.0f));
  yellowCircle->setColor(scene_graph::Vector4(1.0f, 1.0f, 0.0f, 1.0f));

  // Set up hierarchy
  root->addChild(redRect);
  root->addChild(blueCircle);
  root->addChild(greenRect);
  greenRect->addChild(yellowCircle);

  // Set the root in canvas
  canvas->setRoot(root);

  // Set up mouse callback to select shapes
  window->setMouseCallback([&](double xpos, double ypos) {
    // Convert mouse coordinates to scene coordinates
    float sceneX = (float)xpos / window->getWidth() * 20.0f - 10.0f;
    float sceneY = 10.0f - (float)ypos / window->getHeight() * 20.0f;

    // Hit test at this position
    auto hitNode = canvas->hitTest(scene_graph::Vector2(sceneX, sceneY));
    if (hitNode) {
      canvas->selectNode(hitNode);
    } else {
      canvas->selectNode(nullptr);
    }
  });

  // Animation variables
  float time = 0.0f;
  float rotationSpeed = 45.0f; // degrees per second

  // Main loop
  while (!window->shouldClose()) {
    // Update time
    static float lastFrameTime = 0.0f;
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    time += deltaTime;

    // Animate objects
    redRect->setRotation(time * rotationSpeed);
    blueCircle->setPosition(
        scene_graph::Vector2(3.0f + sin(time) * 2.0f, cos(time) * 2.0f));

    // Render
    canvas->render();

    // Swap buffers and poll events
    window->swapBuffers();
    window->pollEvents();
  }

  return 0;
}