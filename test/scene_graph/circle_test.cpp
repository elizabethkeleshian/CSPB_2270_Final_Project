#include "scene_graph/circle.h"
#include <gtest/gtest.h>
#include <memory>

namespace scene_graph {

class CircleTest : public ::testing::Test {
protected:
    void SetUp() override {
        circle = std::make_shared<Circle>("testCircle");
    }

    std::shared_ptr<Circle> circle;
};

TEST_F(CircleTest, Constructor_DefaultRadius) {
    EXPECT_FLOAT_EQ(circle->getRadius(), 0.5f);
}

TEST_F(CircleTest, Constructor_CustomRadius) {
    std::shared_ptr<Circle> customCircle = std::make_shared<Circle>("custom", 2.0f);
    EXPECT_FLOAT_EQ(customCircle->getRadius(), 2.0f);
}

TEST_F(CircleTest, Radius_SetRadius) {
    circle->setRadius(3.0f);
    EXPECT_FLOAT_EQ(circle->getRadius(), 3.0f);
}

TEST_F(CircleTest, ContainsPoint_Inside) {
    circle->setRadius(1.0f);
    EXPECT_TRUE(circle->containsPoint(Vector2(0.5f, 0.5f)));
}

TEST_F(CircleTest, ContainsPoint_Outside) {
    circle->setRadius(1.0f);
    EXPECT_FALSE(circle->containsPoint(Vector2(2.0f, 2.0f)));
}

TEST_F(CircleTest, ContainsPoint_OnEdge) {
    circle->setRadius(1.0f);
    EXPECT_TRUE(circle->containsPoint(Vector2(1.0f, 0.0f)));
}

TEST_F(CircleTest, ContainsPoint_Transformed) {
    circle->setRadius(1.0f);
    circle->setPosition(Vector2(1.0f, 1.0f));
    
    // ,ake sure the point is inside the circle after transformation
    EXPECT_TRUE(circle->containsPoint(Vector2(1.5f, 1.5f)));
    
    // check if point is outside the circle after transformation
    EXPECT_FALSE(circle->containsPoint(Vector2(2.5f, 2.5f)));
}

// Note: render() will be tested in visualization tests

} // namespace scene_graph 