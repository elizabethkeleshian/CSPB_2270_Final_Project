#include "scene_graph/rectangle.h"
#include "types.h"
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
  EXPECT_EQ(rectangle->getSize(), Vector2(1.0F, 1.0F));
}

TEST_F(RectangleTest, Constructor_CustomSize) {
  std::shared_ptr<Rectangle> customRect =
      std::make_shared<Rectangle>("custom", Vector2(2.0F, 3.0F));
  EXPECT_EQ(customRect->getSize(), Vector2(2.0F, 3.0F));
}

TEST_F(RectangleTest, Size_SetSize) {
  rectangle->setSize(Vector2(4.0F, 5.0F));
  EXPECT_EQ(rectangle->getSize(), Vector2(4.0F, 5.0F));
}

TEST_F(RectangleTest, ContainsPoint_Inside) {
  rectangle->setSize(Vector2(2.0F, 2.0F));
  EXPECT_TRUE(rectangle->containsPoint(Vector2(0.5F, 0.5F)));
}

TEST_F(RectangleTest, ContainsPoint_Outside) {
  rectangle->setSize(Vector2(2.0F, 2.0F));
  EXPECT_FALSE(rectangle->containsPoint(Vector2(2.0F, 2.0F)));
}

TEST_F(RectangleTest, ContainsPoint_Transformed) {
  rectangle->setSize(Vector2(2.0F, 2.0F));
  rectangle->setPosition(Vector2(1.0F, 1.0F));
  rectangle->setRotation(45.0F);

  // This point would be outside the rectangle if not transformed
  // But after applying the transform, it should be inside
  EXPECT_TRUE(rectangle->containsPoint(Vector2(2.0F, 1.0F)));

  // This point should still be outside even after transform
  EXPECT_FALSE(rectangle->containsPoint(Vector2(3.0F, 3.0F)));
}

TEST_F(RectangleTest, ContainsPoint_WithRotation) {
  rectangle->setSize(Vector2(2.0F, 2.0F));
  rectangle->setPosition(Vector2(0.0F, 0.0F));
  rectangle->setRotation(45.0F); // 45 degrees rotation

  // A point that would be outside without rotation, but inside with rotation
  EXPECT_TRUE(rectangle->containsPoint(Vector2(1.0F, 0.0F)));

  // A point that would be inside without rotation, but outside with rotation
  EXPECT_FALSE(rectangle->containsPoint(Vector2(1.0F, 1.0F)));
}
} // namespace scene_graph