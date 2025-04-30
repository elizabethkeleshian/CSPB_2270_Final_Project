#include "scene_graph/rectangle.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {

class RectangleTest : public ::testing::Test {
protected:
    void SetUp() override {
        rectangle = std::make_shared<Rectangle>("testRectangle");
    }

    std::shared_ptr<Rectangle> rectangle;
};

TEST_F(RectangleTest, Constructor_DefaultSize) {
    EXPECT_EQ(rectangle->getSize(), Vector2(1.0f, 1.0f));
}

TEST_F(RectangleTest, Constructor_CustomSize) {
    auto customRect = std::make_shared<Rectangle>("custom", Vector2(2.0f, 3.0f));
    EXPECT_EQ(customRect->getSize(), Vector2(2.0f, 3.0f));
}

TEST_F(RectangleTest, Size_SetSize) {
    rectangle->setSize(Vector2(4.0f, 5.0f));
    EXPECT_EQ(rectangle->getSize(), Vector2(4.0f, 5.0f));
}

TEST_F(RectangleTest, ContainsPoint_Inside) {
    rectangle->setSize(Vector2(2.0f, 2.0f));
    EXPECT_TRUE(rectangle->containsPoint(Vector2(0.5f, 0.5f)));
}

TEST_F(RectangleTest, ContainsPoint_Outside) {
    rectangle->setSize(Vector2(2.0f, 2.0f));
    EXPECT_FALSE(rectangle->containsPoint(Vector2(2.0f, 2.0f)));
}

TEST_F(RectangleTest, ContainsPoint_Transformed) {
    rectangle->setSize(Vector2(2.0f, 2.0f));
    rectangle->setPosition(Vector2(1.0f, 1.0f));
    rectangle->setRotation(45.0f);
    // TODO: Test points in transformed space
}

// Note: render() will be tested in visualization tests

} // namespace scene_graph 