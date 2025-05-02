#include "scene_graph/shape.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {

// Create a concrete implementation of the abstract Shape class for testing
class TestShape : public Shape {
public:
    TestShape(const std::string& name) : Shape(name) {}
    
    // Implement pure virtual methods
    void render() const override {}
    bool containsPoint(const Vector2& point) const override { return false; }
};

class ShapeTest : public ::testing::Test {
protected:
    void SetUp() override {
        shape = std::make_shared<TestShape>("testShape");
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