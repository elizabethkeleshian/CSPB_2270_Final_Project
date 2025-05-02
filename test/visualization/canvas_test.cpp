#include "visualization/canvas.h"
#include "visualization/renderer.h"
#include "scene_graph/rectangle.h"
#include "scene_graph/circle.h"
#include "scene_graph/node.h"
#include "scene_graph/types.h"
#include <gtest/gtest.h>
#include <memory>

namespace visualization {
using namespace std;
using namespace scene_graph;
class CanvasTest : public ::testing::Test {
protected:
    void SetUp() override {
        renderer = make_shared<Renderer>();
        canvas = make_shared<Canvas>();
        
        // Initialize the renderer and canvas
        ASSERT_TRUE(renderer->initialize());
        ASSERT_TRUE(canvas->initialize(renderer));
        
        // Create a simple scene graph
        root = make_shared<Node>("Root");
        child1 = make_shared<Rectangle>("Rectangle", Vector2(2.0f, 1.0f));
        child2 = make_shared<Circle>("Circle", 1.0f);
        
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

TEST_F(CanvasTest, AddShape_IncreasesShapeCount) {
    std::shared_ptr<Rectangle> shape = std::make_shared<Rectangle>("TestShape");
    canvas->addShape(shape);
    
    // Since shapes_ is private, we'll test the rendering by checking 
    // if the shape is found in hit testing
    shape->setPosition(Vector2(0.0f, 0.0f));
    shared_ptr<Node> hitNode = canvas->hitTest(Vector2(0.0f, 0.0f));
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
 * Hit tests are used to determine which node is at a specific point in the canvas.
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

} // namespace visualization