#include "scene_graph/circle.h"
#include "scene_graph/node.h"
#include "scene_graph/rectangle.h"
#include "types.h"
#include "visualization/canvas.h"
#include "visualization/renderer.h"
#include <gtest/gtest.h>
#include <memory>

namespace visualization {
using namespace std;
using namespace scene_graph;
class CanvasTest : public ::testing::Test {
protected:
  void SetUp() override {
    renderer = make_shared<Renderer>();
    // Enable headless mode for testing without OpenGL context
    renderer->setHeadlessMode(true);

    canvas = make_shared<Canvas>();

    // Initialize the renderer and canvas
    ASSERT_TRUE(renderer->initialize());
    ASSERT_TRUE(canvas->initialize(renderer));

    // Create a simple scene graph
    root = make_shared<Node>("Root");
    child1 = make_shared<Rectangle>("Rectangle", Vector2(2.0F, 1.0F));
    child2 = make_shared<Circle>("Circle", 1.0F);

    // Set up the scene hierarchy
    root->addChild(child1);
    root->addChild(child2);

    // Position the shapes
    child1->setPosition(Vector2(-2.0f, 0.0f));
    child2->setPosition(Vector2(2.0f, 0.0f));
  }

  shared_ptr<Renderer> renderer;
  shared_ptr<Canvas> canvas;
  shared_ptr<Node> root;
  shared_ptr<Rectangle> child1;
  shared_ptr<Circle> child2;
};

TEST_F(CanvasTest, Initialize_Success) {
  // Setup is already performed, just check result
  EXPECT_TRUE(canvas != nullptr);
}

TEST_F(CanvasTest, SetRoot_UpdatesRoot) {
  canvas->setRoot(root);
  EXPECT_EQ(canvas->getRoot(), root);
}

void printTransformInfo(const std::string &label,
                        const scene_graph::Transform &transform) {
  std::cout << label << ":" << std::endl;
  std::cout << "  Position: (" << transform.getPosition().x << ", "
            << transform.getPosition().y << ")" << std::endl;
  std::cout << "  Rotation: " << transform.getRotation() << std::endl;
  std::cout << "  Scale: (" << transform.getScale().x << ", "
            << transform.getScale().y << ")" << std::endl;
}

TEST_F(CanvasTest, ContainsPoint_Basic) {
  // Create a rectangle at origin with size 2x2
  auto rect =
      std::make_shared<scene_graph::Rectangle>("TestRect", Vector2(2.0f, 2.0f));
  rect->setPosition(Vector2(0.0f, 0.0f));

  // Print transform information
  printTransformInfo("Local Transform", rect->getLocalTransform());

  // Test if center point is contained
  bool containsCenter = rect->containsPoint(Vector2(0.0f, 0.0f));
  std::cout << "Contains center point: " << (containsCenter ? "YES" : "NO")
            << std::endl;
  EXPECT_TRUE(containsCenter);

  // Test if point inside is contained
  bool containsInside = rect->containsPoint(Vector2(0.9f, 0.9f));
  std::cout << "Contains inside point (0.9, 0.9): "
            << (containsInside ? "YES" : "NO") << std::endl;
  EXPECT_TRUE(containsInside);

  // Test if point outside is not contained
  bool containsOutside = rect->containsPoint(Vector2(1.1f, 1.1f));
  std::cout << "Contains outside point (1.1, 1.1): "
            << (containsOutside ? "YES" : "NO") << std::endl;
  EXPECT_FALSE(containsOutside);
}

TEST_F(CanvasTest, HitTest_StandaloneShape) {
  auto rect = std::make_shared<scene_graph::Rectangle>("StandaloneRect",
                                                       Vector2(2.0f, 2.0f));

  // Set position to origin
  rect->setPosition(Vector2(0.0f, 0.0f));

  // Add to canvas
  canvas->addShape(rect);

  // Additional debug information
  std::cout << "=== Standalone shape test ===" << std::endl;
  std::cout << "Shape position: (" << rect->getPosition().x << ", "
            << rect->getPosition().y << ")" << std::endl;
  std::cout << "Shape size: (" << rect->getSize().x << ", " << rect->getSize().y
            << ")" << std::endl;

  // Test center point
  auto hitCenter = canvas->hitTest(Vector2(0.0f, 0.0f));
  EXPECT_EQ(hitCenter, rect) << "Failed to hit at center point";

  // Test corner points (should be inside)
  auto hitTopRight = canvas->hitTest(Vector2(0.9f, 0.9f));
  EXPECT_EQ(hitTopRight, rect) << "Failed to hit at top-right inside corner";

  auto hitBottomLeft = canvas->hitTest(Vector2(-0.9f, -0.9f));
  EXPECT_EQ(hitBottomLeft, rect)
      << "Failed to hit at bottom-left inside corner";

  // Test points outside
  auto hitOutside1 = canvas->hitTest(Vector2(1.1f, 1.1f));
  EXPECT_NE(hitOutside1, rect) << "Incorrectly hit outside top-right";

  auto hitOutside2 = canvas->hitTest(Vector2(-1.1f, -1.1f));
  EXPECT_NE(hitOutside2, rect) << "Incorrectly hit outside bottom-left";
}

TEST_F(CanvasTest, AddShape_IncreasesShapeCount) {
  std::shared_ptr<Rectangle> shape =
      std::make_shared<Rectangle>("TestShape", Vector2(2.0f, 2.0f));

  // Add to canvas
  canvas->addShape(shape);

  // Position at origin
  shape->setPosition(Vector2(0.0f, 0.0f));

  // Debug
  std::cout << "Shape position: " << shape->getPosition().x << ", "
            << shape->getPosition().y << std::endl;
  std::cout << "Shape size: " << shape->getSize().x << ", "
            << shape->getSize().y << std::endl;

  // Directly test if shape contains the test point
  bool directContains = shape->containsPoint(Vector2(0.0f, 0.0f));
  std::cout << "Shape directly contains point: "
            << (directContains ? "YES" : "NO") << std::endl;

  // Test hit testing
  shared_ptr<Node> hitNode = canvas->hitTest(Vector2(0.0f, 0.0f));

  // Show the result
  if (hitNode) {
    std::cout << "Hit found: " << hitNode->getName() << std::endl;
  } else {
    std::cout << "No hit found" << std::endl;
  }

  EXPECT_EQ(hitNode, shape);
}

TEST_F(CanvasTest, RemoveShape_DecreasesShapeCount) {
  shared_ptr<Rectangle> shape = make_shared<Rectangle>("TestShape");
  canvas->addShape(shape);
  canvas->removeShape(shape);

  // Shape should no longer be hit-testable
  shape->setPosition(Vector2(0.0f, 0.0f));
  shared_ptr<Node> hitNode = canvas->hitTest(Vector2(0.0f, 0.0f));
  EXPECT_NE(hitNode, shape);
}

/**
 * @brief Test that hitTest returns the correct node
 *
 * Hit tests are used to determine which node is at a specific point in the
 * canvas.
 */
TEST_F(CanvasTest, HitTest_ReturnsCorrectNode) {
  canvas->setRoot(root);

  // Test hit on first child
  shared_ptr<Node> hit1 = canvas->hitTest(Vector2(-2.0f, 0.0f));
  EXPECT_EQ(hit1, child1);

  // Test hit on second child
  shared_ptr<Node> hit2 = canvas->hitTest(Vector2(2.0f, 0.0f));
  EXPECT_EQ(hit2, child2);

  // Test hit on empty space
  shared_ptr<Node> hit3 = canvas->hitTest(Vector2(0.0f, 5.0f));
  EXPECT_EQ(hit3, nullptr);
}

TEST_F(CanvasTest, SelectNode_UpdatesSelectedNode) {
  canvas->selectNode(child1);
  EXPECT_EQ(canvas->getSelectedNode(), child1);

  canvas->selectNode(nullptr);
  EXPECT_EQ(canvas->getSelectedNode(), nullptr);
}

TEST_F(CanvasTest, Render_NoExceptions) {
  canvas->setRoot(root);
  EXPECT_NO_THROW(canvas->render());
}

TEST_F(CanvasTest, Clear_RemovesRoot) {
  canvas->setRoot(root);
  canvas->clear();
  EXPECT_EQ(canvas->getRoot(), nullptr);
}

TEST_F(CanvasTest, HitTest_FindsNodeAtExactPosition) {
  canvas->setRoot(root);
  child1->setPosition(Vector2(5.0f, 5.0f));

  // Test hit on exact position
  std::shared_ptr<scene_graph::Node> hit = canvas->hitTest(Vector2(5.0f, 5.0f));
  EXPECT_EQ(hit, child1);
}

TEST_F(CanvasTest, HitTest_WithinShape) {
  canvas->setRoot(root);
  auto rect =
      std::make_shared<scene_graph::Rectangle>("TestRect", Vector2(2.0f, 2.0f));
  rect->setPosition(Vector2(0.0f, 0.0f));
  canvas->addShape(rect);

  // Test various points within the rectangle
  EXPECT_EQ(canvas->hitTest(Vector2(0.0f, 0.0f)), rect);   // Center
  EXPECT_EQ(canvas->hitTest(Vector2(0.9f, 0.9f)), rect);   // Near corner but
                                                           // inside
  EXPECT_EQ(canvas->hitTest(Vector2(-0.9f, -0.9f)), rect); // Near opposite
                                                           // corner but inside

  // Test points outside
  EXPECT_NE(canvas->hitTest(Vector2(1.1f, 1.1f)), rect); // Just outside corner
  EXPECT_NE(canvas->hitTest(Vector2(-1.1f, -1.1f)), rect); // Just outside
                                                           // opposite corner
}

TEST_F(CanvasTest, GetLocalTransform_ReturnsCorrectTransform) {
  auto shape =
      std::make_shared<scene_graph::Rectangle>("TestRect", Vector2(2.0f, 2.0f));

  // Set position
  Vector2 testPos(3.0f, 4.0f);
  shape->setPosition(testPos);

  // Get transform
  auto transform = shape->getLocalTransform();
  Vector2 transformPos = transform.getPosition();

  std::cout << "Set position: (" << testPos.x << ", " << testPos.y << ")"
            << std::endl;
  std::cout << "Transform position: (" << transformPos.x << ", "
            << transformPos.y << ")" << std::endl;

  // Check if they match
  EXPECT_FLOAT_EQ(transformPos.x, testPos.x);
  EXPECT_FLOAT_EQ(transformPos.y, testPos.y);
}

TEST_F(CanvasTest, HitTest_MultipleObjects_ReturnsCorrectZOrder) {
  // Create overlapping shapes
  auto rect1 =
      std::make_shared<scene_graph::Rectangle>("Rect1", Vector2(3.0f, 3.0f));
  rect1->setPosition(Vector2(0.0f, 0.0f));

  auto rect2 =
      std::make_shared<scene_graph::Rectangle>("Rect2", Vector2(2.0f, 2.0f));
  rect2->setPosition(Vector2(1.0f, 1.0f)); // Overlaps with rect1

  // Add shapes to canvas in specific order (rect2 added last so it's on top)
  canvas->addShape(rect1);
  canvas->addShape(rect2);

  // Hit test at a position where both rectangles overlap
  auto hit = canvas->hitTest(Vector2(1.5f, 1.5f));

  // Should return the top-most shape (rect2)
  EXPECT_EQ(hit, rect2);
}
} // namespace visualization