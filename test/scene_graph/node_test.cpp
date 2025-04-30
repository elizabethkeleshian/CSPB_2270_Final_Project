#include "scene_graph/node.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {

class NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        node = std::make_shared<Node>("testNode");
    }

    std::shared_ptr<Node> node;
};

TEST_F(NodeTest, Constructor_SetsName) {
    EXPECT_EQ(node->getName(), "testNode");
}

TEST_F(NodeTest, SetName_UpdatesName) {
    node->setName("newName");
    EXPECT_EQ(node->getName(), "newName");
}

TEST_F(NodeTest, Transform_DefaultValues) {
    EXPECT_EQ(node->getPosition(), Vector2(0.0f));
    EXPECT_EQ(node->getRotation(), 0.0f);
    EXPECT_EQ(node->getScale(), Vector2(1.0f));
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
    auto child = std::make_shared<Node>("child");
    node->addChild(child);
    EXPECT_EQ(child->getParent(), node);
    EXPECT_EQ(node->getChildren().size(), 1);
}

TEST_F(NodeTest, ParentChild_RemoveChild) {
    auto child = std::make_shared<Node>("child");
    node->addChild(child);
    node->removeChild(child);
    EXPECT_EQ(child->getParent(), nullptr);
    EXPECT_EQ(node->getChildren().size(), 0);
}

TEST_F(NodeTest, TransformMatrix_LocalTransform) {
    // Test local transform matrix calculation
    node->setPosition(Vector2(1.0f, 2.0f));
    node->setRotation(45.0f);
    node->setScale(Vector2(2.0f, 3.0f));
    // TODO: Verify matrix components
}

TEST_F(NodeTest, TransformMatrix_WorldTransform) {
    // Test world transform matrix calculation with parent
    auto parent = std::make_shared<Node>("parent");
    parent->setPosition(Vector2(1.0f, 1.0f));
    node->setPosition(Vector2(1.0f, 1.0f));
    parent->addChild(node);
    // TODO: Verify world transform components
}

} // namespace scene_graph

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
