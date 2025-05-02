#include "scene_graph/types.h"
#include <cmath>
#include <gtest/gtest.h>

namespace scene_graph {

TEST(TypesTest, WrapAngle_WithinRange) {
  // Test angles already within [-pi, pi]
  EXPECT_FLOAT_EQ(wrapAngle(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(wrapAngle(glm::pi<float>()), glm::pi<float>());
  EXPECT_FLOAT_EQ(wrapAngle(-glm::pi<float>()), -glm::pi<float>());
  EXPECT_FLOAT_EQ(wrapAngle(glm::pi<float>() / 2), glm::pi<float>() / 2);
}

TEST(TypesTest, WrapAngle_OutsideRange) {
  // Test angles outside [-pi, pi]
  EXPECT_FLOAT_EQ(wrapAngle(2.0f * glm::pi<float>()), 0.0f);
  EXPECT_FLOAT_EQ(wrapAngle(3.0f * glm::pi<float>()), glm::pi<float>());
  EXPECT_FLOAT_EQ(wrapAngle(-2.0f * glm::pi<float>()), 0.0f);
  EXPECT_FLOAT_EQ(wrapAngle(-3.0f * glm::pi<float>()), -glm::pi<float>());
}

TEST(TypesTest, WrapAngle_ShortestPath) {
  // Test angles that should take the shortest path
  const float epsilon = 0.0001f;

  // Test near pi boundary
  EXPECT_NEAR(wrapAngle(glm::pi<float>() + 0.1f), -glm::pi<float>() + 0.1f,
              epsilon);
  EXPECT_NEAR(wrapAngle(-glm::pi<float>() - 0.1f), glm::pi<float>() - 0.1f,
              epsilon);

  // Test near 2*pi boundary
  EXPECT_NEAR(wrapAngle(2.0f * glm::pi<float>() + 0.1f), 0.1f, epsilon);
  EXPECT_NEAR(wrapAngle(-2.0f * glm::pi<float>() - 0.1f), -0.1f, epsilon);
}

TEST(TypesTest, WrapAngle_EdgeCases) {
  // Test edge cases and special values
  EXPECT_FLOAT_EQ(wrapAngle(std::numeric_limits<float>::infinity()), 0.0f);
  EXPECT_FLOAT_EQ(wrapAngle(-std::numeric_limits<float>::infinity()), 0.0f);
  EXPECT_TRUE(std::isnan(wrapAngle(std::numeric_limits<float>::quiet_NaN())));
}

} // namespace scene_graph