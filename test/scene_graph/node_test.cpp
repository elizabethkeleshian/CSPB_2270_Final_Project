#include "scene_graph/node.h"
#include "types.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {
using namespace std;
using std::make_shared;

class NodeTest : public ::testing::Test {
protected:
  void SetUp() override { node = make_shared<Node>("testNode"); }

  shared_ptr<Node> node;
};

TEST_F(NodeTest, Constructor_SetsName) {
  EXPECT_EQ(node->getName(), "testNode");
}

TEST_F(NodeTest, SetName_UpdatesName) {
  node->setName("newName");
  EXPECT_EQ(node->getName(), "newName");
}

TEST_F(NodeTest, Transform_DefaultValues) {
  EXPECT_EQ(node->getPosition(), Vector2(0.0F));
  EXPECT_EQ(node->getRotation(), 0.0F);
  EXPECT_EQ(node->getScale(), Vector2(1.0F));
}

TEST_F(NodeTest, Transform_SetPosition) {
  node->setPosition(Vector2(1.0f, 2.0f));
  EXPECT_EQ(node->getPosition(), Vector2(1.0f, 2.0f));
}

TEST_F(NodeTest, Transform_SetRotation) {
  node->setRotation(45.0f);
  EXPECT_EQ(node->getRotation(), 45.0f);
}

TEST_F(NodeTest, Transform_SetScale) {
  node->setScale(Vector2(2.0f, 3.0f));
  EXPECT_EQ(node->getScale(), Vector2(2.0f, 3.0f));
}

TEST_F(NodeTest, ParentChild_AddChild) {
  shared_ptr<Node> child = make_shared<Node>("child");
  node->addChild(child);
  EXPECT_TRUE(child->hasParent(node));
  EXPECT_EQ(node->getChildren().size(), 1);
}

TEST_F(NodeTest, ParentChild_RemoveChild) {
  shared_ptr<Node> child = make_shared<Node>("child");
  node->addChild(child);
  node->removeChild(child);
  EXPECT_TRUE(child->isOrphaned());
  EXPECT_EQ(node->getChildren().size(), 0);
}

TEST_F(NodeTest, TransformMatrix_LocalTransform) {
  // Test local transform matrix calculation
  node->setPosition(Vector2(1.0f, 2.0f));
  node->setRotation(45.0f);
  node->setScale(Vector2(2.0f, 3.0f));

  Transform localTransform = node->getLocalTransform();
  Matrix4 matrix = localTransform.getMatrix();

  // Check scale (upper 2x2 matrix after accounting for rotation)
  float scaleX =
      sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1]);
  float scaleY =
      sqrt(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1]);
  EXPECT_NEAR(scaleX, 2.0f, 0.0001f);
  EXPECT_NEAR(scaleY, 3.0f, 0.0001f);

  // Check translation (right column)
  EXPECT_NEAR(matrix[3][0], 1.0f, 0.0001f);
  EXPECT_NEAR(matrix[3][1], 2.0f, 0.0001f);

  // Check rotation (test a point transformation)
  Vector2 testPoint(1.0f, 0.0f);
  Vector2 transformed = localTransform.transformPoint(testPoint);
  // A 45-degree rotation of (1,0) should be approximately (0.7071, 0.7071)
  EXPECT_NEAR(transformed.x, 0.7071f * 2.0f + 1.0f, 0.01f);
  EXPECT_NEAR(transformed.y, 3.414f, 0.01f);
}

TEST_F(NodeTest, TransformMatrix_WorldTransform) {
  // Test world transform matrix calculation with parent
  shared_ptr<Node> parent = make_shared<Node>("parent");
  parent->setPosition(Vector2(1.0f, 1.0f));
  node->setPosition(Vector2(1.0f, 1.0f));
  parent->addChild(node);

  Transform worldTransform = node->getGlobalTransform();
  Vector2 worldPos = worldTransform.getPosition();

  // Child is at (1,1) in parent space, parent is at (1,1) in world space
  // So child should be at (2,2) in world space
  EXPECT_NEAR(worldPos.x, 2.0f, 0.0001f);
  EXPECT_NEAR(worldPos.y, 2.0f, 0.0001f);
}

TEST_F(NodeTest, AddChild_AlreadyHasParent_UpdatesParent) {
  shared_ptr<Node> parent1 = make_shared<Node>("parent1");
  shared_ptr<Node> parent2 = make_shared<Node>("parent2");
  shared_ptr<Node> child = make_shared<Node>("child");

  parent1->addChild(child);
  EXPECT_TRUE(child->hasParent(parent1));

  parent2->addChild(child);
  EXPECT_TRUE(child->hasParent(parent2));
  EXPECT_EQ(parent1->getChildren().size(), 0);
}

TEST_F(NodeTest, TransformPropagation_ChildInheritsParentTransform) {
  shared_ptr<Node> parent = make_shared<Node>("parent");
  shared_ptr<Node> child = make_shared<Node>("child");

  parent->setPosition(Vector2(5.0f, 0.0f));
  parent->setRotation(90.0f);
  child->setPosition(Vector2(0.0f, 1.0f));

  parent->addChild(child);

  // In parent space, child is at (0,1)
  // After parent's rotation and translation, child should be at (5,-1) in world
  // space

  Transform worldTransform = child->getGlobalTransform();
  Vector2 worldPos = worldTransform.getPosition();

  EXPECT_NEAR(worldPos.x, 4.0f, 0.0001f); // 5 + (-1) after rotation
  EXPECT_NEAR(worldPos.y, 0.0f, 0.0001f); // 0 + 0 after rotation
}

TEST_F(NodeTest, Memory_ParentChildCleanup) {
  // Create parent with child
  auto parent = std::make_shared<scene_graph::Node>("Parent");
  auto child = std::make_shared<scene_graph::Node>("Child");
  parent->addChild(child);

  // Make sure the child knows about its parent
  EXPECT_FALSE(child->isOrphaned());

  // Verify parent's children list contains the child
  EXPECT_EQ(parent->getChildren().size(), 1);

  // Explicitly remove the child
  parent->removeChild(child);

  // Verify parent's children list is updated
  EXPECT_EQ(parent->getChildren().size(), 0);
}
} // namespace scene_graph
