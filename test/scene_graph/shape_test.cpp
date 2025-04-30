#include "scene_graph/shape.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {

class ShapeTest : public ::testing::Test {
protected:
    void SetUp() override {
        shape = std::make_shared<Shape>("testShape");
    }

    std::shared_ptr<Shape> shape;
};

TEST_F(ShapeTest, Constructor_SetsName) {
    EXPECT_EQ(shape->getName(), "testShape");
}

TEST_F(ShapeTest, Color_DefaultValue) {
    EXPECT_EQ(shape->getColor(), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

TEST_F(ShapeTest, Color_SetColor) {
    shape->setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_EQ(shape->getColor(), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

// Note: render() and containsPoint() are pure virtual,
// so they'll be tested in Rectangle and Circle tests

} // namespace scene_graph 