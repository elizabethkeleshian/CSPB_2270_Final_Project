// File: test/scene_graph/transform_test.cpp

#include "scene_graph/transform.h"
#include "types.h"
#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene_graph::test {

// Constants for floating-point equality checks
constexpr float kEpsilon = 0.0001F;

// Check if two vectors are equal within a small epsilon value
template <typename T>
bool areVectorsEqual(const T &v1, const T &v2, float epsilon = kEpsilon) {
  return glm::all(glm::epsilonEqual(v1, v2, epsilon));
}

// Check if two matrices are equal within a small epsilon value
bool areMatricesEqual(const Matrix4 &m1, const Matrix4 &m2,
                      float epsilon = kEpsilon) {
  // Use glm's built-in epsilon comparison for matrices
  return glm::all(glm::epsilonEqual(Vector4(m1[0]), Vector4(m2[0]), epsilon)) &&
         glm::all(glm::epsilonEqual(Vector4(m1[1]), Vector4(m2[1]), epsilon)) &&
         glm::all(glm::epsilonEqual(Vector4(m1[2]), Vector4(m2[2]), epsilon)) &&
         glm::all(glm::epsilonEqual(Vector4(m1[3]), Vector4(m2[3]), epsilon));
}

class TransformTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize common test values
    defaultTransform_ = Transform();
    translatedTransform_ = Transform();
    translatedTransform_.setPosition(Vector2(10.0F, 20.0F));

    rotatedTransform_ = Transform();
    rotatedTransform_.setRotation(90.0F);

    scaledTransform_ = Transform();
    scaledTransform_.setScale(Vector2(2.0F, 3.0F));

    combinedTransform_ = Transform();
    combinedTransform_.setPosition(Vector2(10.0F, 20.0F));
    combinedTransform_.setRotation(90.0F);
    combinedTransform_.setScale(Vector2(2.0F, 3.0F));
  }

  // Common test transforms
  Transform defaultTransform_;
  Transform translatedTransform_;
  Transform rotatedTransform_;
  Transform scaledTransform_;
  Transform combinedTransform_;
};

TEST_F(TransformTest, DefaultConstructor) {
  EXPECT_TRUE(areVectorsEqual(defaultTransform_.getPosition(), Vector2(0, 0)));
  EXPECT_FLOAT_EQ(defaultTransform_.getRotation(), 0.0F);
  EXPECT_TRUE(areVectorsEqual(defaultTransform_.getScale(), Vector2(1, 1)));
}

TEST_F(TransformTest, SetAndGetPosition) {
  Vector2 position(5.0F, 10.0F);
  defaultTransform_.setPosition(position);
  EXPECT_TRUE(areVectorsEqual(defaultTransform_.getPosition(), position));
}

TEST_F(TransformTest, SetAndGetRotation) {
  float rotation = 45.0F;
  defaultTransform_.setRotation(rotation);
  EXPECT_FLOAT_EQ(defaultTransform_.getRotation(), rotation);
}

TEST_F(TransformTest, SetAndGetScale) {
  Vector2 scale(2.0F, 3.0F);
  defaultTransform_.setScale(scale);
  EXPECT_TRUE(areVectorsEqual(defaultTransform_.getScale(), scale));
}

TEST_F(TransformTest, GetMatrix) {
  Matrix4 identityMatrix = Matrix4(1.0F);

  // A default transformation should be the identity matrix
  Matrix4 defaultMatrix = defaultTransform_.getMatrix();
  EXPECT_TRUE(areMatricesEqual(defaultMatrix, identityMatrix));

  // A translated transformation should have a translation component
  Matrix4 translatedMatrix = translatedTransform_.getMatrix();
  Matrix4 expectedTranslated =
      translate(identityMatrix, Vector3(10.0F, 20.0F, 0.0F));
  EXPECT_TRUE(areMatricesEqual(translatedMatrix, expectedTranslated));

  // A rotated transformation should have a rotation component
  Matrix4 rotatedMatrix = rotatedTransform_.getMatrix();
  Matrix4 expectedRotated =
      rotate(identityMatrix, radians(90.0F), Vector3(0.0F, 0.0F, 1.0F));
  EXPECT_TRUE(areMatricesEqual(rotatedMatrix, expectedRotated));

  // A scaled transformation should have a scale component
  Matrix4 scaledMatrix = scaledTransform_.getMatrix();
  Matrix4 expectedScaled = scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f));
  EXPECT_TRUE(areMatricesEqual(scaledMatrix, expectedScaled));

  // A combined transformation should have all components
  Matrix4 combinedMatrix = combinedTransform_.getMatrix();

  // Create individual transformation matrices
  Matrix4 scaleMatrix = scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f));
  Matrix4 rotationMatrix =
      rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f));
  Matrix4 translationMatrix =
      translate(identityMatrix, Vector3(10.0f, 20.0f, 0.0f));

  // Calculate the expected matrix by explicitly multiplying matrices in TRS
  // order (giving SRT transformation)
  Matrix4 expectedCombined = translationMatrix * rotationMatrix * scaleMatrix;

  EXPECT_TRUE(areMatricesEqual(combinedMatrix, expectedCombined));
}

TEST_F(TransformTest, TransformPoint) {
  Vector2 point(1.0f, 1.0f);

  // A default transformation should not change the point
  Vector2 transformedPoint = defaultTransform_.transformPoint(point);
  EXPECT_TRUE(areVectorsEqual(transformedPoint, point));

  // A translated point should be moved by the translation
  Vector2 translatedPoint = translatedTransform_.transformPoint(point);
  Vector2 expectedTranslatedPoint = point + Vector2(10.0f, 20.0f);
  EXPECT_TRUE(areVectorsEqual(translatedPoint, expectedTranslatedPoint));

  // A rotated point should be rotated by the rotation (90 degrees around
  // origin)
  Vector2 rotatedPoint = rotatedTransform_.transformPoint(point);
  Vector2 expectedRotatedPoint =
      Vector2(-1.0f, 1.0f); // 90 degree rotation of (1,1)
  EXPECT_TRUE(areVectorsEqual(rotatedPoint, expectedRotatedPoint));

  // A scaled point should be scaled by the scale
  Vector2 scaledPoint = scaledTransform_.transformPoint(point);
  Vector2 expectedScaledPoint = Vector2(2.0f, 3.0f); // (1,1) * (2,3)
  EXPECT_TRUE(areVectorsEqual(scaledPoint, expectedScaledPoint));

  // combined transformation should apply all transformations in the correct
  // order
  Vector2 combinedPoint = combinedTransform_.transformPoint(point);

  // Calculate expected result by applying transforms in the correct order
  // (Scale → Rotate → Translate)
  Vector2 temp = point;
  temp = Vector2(temp.x * 2.0f, temp.y * 3.0f); // Scale first
  temp = Vector2(-temp.y, temp.x);              // Rotate 90 degrees second
  temp = temp + Vector2(10.0f, 20.0f);          // Translate last

  EXPECT_TRUE(areVectorsEqual(combinedPoint, temp));
}

TEST_F(TransformTest, SetMatrixBasicTransformations) {
  Matrix4 identityMatrix = Matrix4(1.0f);

  // Test translation matrix decomposition
  Transform testTransform;
  Matrix4 translatedMatrix =
      translate(identityMatrix, Vector3(10.0f, 20.0f, 0.0f));
  testTransform.setMatrix(translatedMatrix);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getPosition(), Vector2(10.0f, 20.0f)));
  EXPECT_FLOAT_EQ(testTransform.getRotation(), 0.0f);
  EXPECT_TRUE(areVectorsEqual(testTransform.getScale(), Vector2(1, 1)));

  // Test rotation matrix decomposition
  testTransform = Transform(); // Reset
  Matrix4 rotatedMatrix =
      rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f));
  testTransform.setMatrix(rotatedMatrix);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getPosition(), Vector2(0.0f, 0.0f)));
  EXPECT_NEAR(testTransform.getRotation(), 90.0f, kEpsilon);
  EXPECT_TRUE(areVectorsEqual(testTransform.getScale(), Vector2(1, 1)));

  // Test scale matrix decomposition
  testTransform = Transform(); // Reset
  Matrix4 scaledMatrix = scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f));
  testTransform.setMatrix(scaledMatrix);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getPosition(), Vector2(0.0f, 0.0f)));
  EXPECT_FLOAT_EQ(testTransform.getRotation(), 0.0f);
  EXPECT_TRUE(areVectorsEqual(testTransform.getScale(), Vector2(2, 3)));

  // Test combined matrix decomposition
  testTransform = Transform(); // Reset

  Transform expectedTransform;
  expectedTransform.setScale(Vector2(2.0f, 3.0f));
  expectedTransform.setRotation(90.0f);
  expectedTransform.setPosition(Vector2(10.0f, 20.0f));

  Matrix4 combinedMatrix = expectedTransform.getMatrix();
  testTransform.setMatrix(combinedMatrix);

  // Use a larger epsilon for the combined transform tests
  const float kCombinedEpsilon = 0.01f;
  EXPECT_NEAR(testTransform.getPosition().x, 10.0f, kCombinedEpsilon);
  EXPECT_NEAR(testTransform.getPosition().y, 20.0f, kCombinedEpsilon);
  EXPECT_NEAR(testTransform.getRotation(), 90.0f, kCombinedEpsilon);
  EXPECT_NEAR(testTransform.getScale().x, 2.0f, kCombinedEpsilon);
  EXPECT_NEAR(testTransform.getScale().y, 3.0f, kCombinedEpsilon);

  // Test x-axis scaling
  testTransform = Transform(); // Reset
  Matrix4 scaledXMatrix = scale(identityMatrix, Vector3(2.0f, 1.0f, 1.0f));
  testTransform.setMatrix(scaledXMatrix);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getPosition(), Vector2(0.0f, 0.0f)));
  EXPECT_FLOAT_EQ(testTransform.getRotation(), 0.0f);
  EXPECT_TRUE(areVectorsEqual(testTransform.getScale(), Vector2(2, 1)));

  // Test y-axis scaling
  testTransform = Transform(); // Reset
  Matrix4 scaledYMatrix = scale(identityMatrix, Vector3(1.0f, 2.0f, 1.0f));
  testTransform.setMatrix(scaledYMatrix);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getPosition(), Vector2(0.0f, 0.0f)));
  EXPECT_FLOAT_EQ(testTransform.getRotation(), 0.0f);
  EXPECT_TRUE(areVectorsEqual(testTransform.getScale(), Vector2(1, 2)));
}

TEST_F(TransformTest, InverseTransformPoint) {
  Vector2 point(5.0f, 7.0f);

  // Transform the point and then inverse-transform it back to the original
  Vector2 transformedPoint;
  Vector2 inverseTransformedPoint;

  // Test with default transform
  transformedPoint = defaultTransform_.transformPoint(point);
  inverseTransformedPoint =
      defaultTransform_.inverseTransformPoint(transformedPoint);
  EXPECT_TRUE(areVectorsEqual(inverseTransformedPoint, point));

  // Test with translated transform
  transformedPoint = translatedTransform_.transformPoint(point);
  inverseTransformedPoint =
      translatedTransform_.inverseTransformPoint(transformedPoint);
  EXPECT_TRUE(areVectorsEqual(inverseTransformedPoint, point));

  // Expected transformed point for translation
  Vector2 expectedTranslatedPoint = Vector2(15.0f, 27.0f);
  EXPECT_TRUE(areVectorsEqual(transformedPoint, expectedTranslatedPoint));

  // Test with rotated transform
  transformedPoint = rotatedTransform_.transformPoint(point);
  inverseTransformedPoint =
      rotatedTransform_.inverseTransformPoint(transformedPoint);
  EXPECT_TRUE(areVectorsEqual(inverseTransformedPoint, point));

  // Expected transformed point for 90-degree rotation
  Vector2 expectedRotatedPoint = Vector2(-7.0f, 5.0f);
  EXPECT_TRUE(areVectorsEqual(transformedPoint, expectedRotatedPoint));

  // Test with scaled transform
  transformedPoint = scaledTransform_.transformPoint(point);
  inverseTransformedPoint =
      scaledTransform_.inverseTransformPoint(transformedPoint);
  EXPECT_TRUE(areVectorsEqual(inverseTransformedPoint, point));

  // Expected transformed point for scaling
  Vector2 expectedScaledPoint = Vector2(10.0f, 21.0f);
  EXPECT_TRUE(areVectorsEqual(transformedPoint, expectedScaledPoint));

  // Test with combined transform
  transformedPoint = combinedTransform_.transformPoint(point);
  inverseTransformedPoint =
      combinedTransform_.inverseTransformPoint(transformedPoint);
  EXPECT_TRUE(areVectorsEqual(inverseTransformedPoint, point));

  // Calculate expected combined point by applying transforms in the correct
  // order (Scale → Rotate → Translate)
  Vector2 temp = point;
  temp = Vector2(temp.x * 2.0f, temp.y * 3.0f); // Scale first
  temp = Vector2(-temp.y, temp.x);              // Rotate 90 degrees second
  temp = temp + Vector2(10.0f, 20.0f);          // Translate last

  EXPECT_TRUE(areVectorsEqual(transformedPoint, temp));
}

TEST_F(TransformTest, CombineTransformations) {
  Transform parentTransform;
  parentTransform.setPosition(Vector2(1.0f, 2.0f));
  parentTransform.setRotation(30.0f);
  parentTransform.setScale(Vector2(4.0f, 5.0f));

  Transform childTransform;
  childTransform.setPosition(Vector2(6.0f, 7.0f));
  childTransform.setRotation(45.0f);
  childTransform.setScale(Vector2(2.0f, 3.0f));

  // Test point transformation through the hierarchy
  Vector2 testPoint(1.0f, 1.0f);

  // Transform point by child, then by parent (this is how hierarchical
  // transforms work)
  Vector2 childResult = childTransform.transformPoint(testPoint);
  Vector2 expectedCombineResult = parentTransform.transformPoint(childResult);

  // Create a combined transform and test that it gives the same result
  Transform combinedTransform =
      Transform::combine(parentTransform, childTransform);
  Vector2 actualCombineResult = combinedTransform.transformPoint(testPoint);
  EXPECT_TRUE(areVectorsEqual(actualCombineResult, expectedCombineResult));

  // Test matrix combination
  Matrix4 parentMatrix = parentTransform.getMatrix();
  Matrix4 childMatrix = childTransform.getMatrix();
  Matrix4 expectedCombineMatrix = parentMatrix * childMatrix;
  Matrix4 actualCombineMatrix = combinedTransform.getMatrix();
  EXPECT_TRUE(areMatricesEqual(actualCombineMatrix, expectedCombineMatrix));
}

TEST_F(TransformTest, InverseCombineTransformations) {
  Transform parentTransform;
  parentTransform.setPosition(Vector2(1.0f, 2.0f));
  parentTransform.setRotation(30.0f);
  parentTransform.setScale(Vector2(4.0f, 5.0f));

  Transform childTransform;
  childTransform.setPosition(Vector2(6.0f, 7.0f));
  childTransform.setRotation(45.0f);
  childTransform.setScale(Vector2(2.0f, 3.0f));

  // Create combined transform and its inverse
  Transform combinedTransform =
      Transform::combine(parentTransform, childTransform);
  Transform inverseCombinedTransform = combinedTransform.inverse();

  // Test that applying a transform and then its inverse gets us back to the
  // original point
  Vector2 testPoint(1.0f, 1.0f);
  Vector2 combinedResult = combinedTransform.transformPoint(testPoint);
  Vector2 inverseCombinedResult =
      inverseCombinedTransform.transformPoint(combinedResult);
  EXPECT_TRUE(areVectorsEqual(inverseCombinedResult, testPoint));

  // Test matrix inversion
  Matrix4 combinedMatrix = combinedTransform.getMatrix();
  Matrix4 expectedInverseMatrix = glm::inverse(combinedMatrix);
  Matrix4 actualInverseMatrix = inverseCombinedTransform.getMatrix();
  EXPECT_TRUE(areMatricesEqual(actualInverseMatrix, expectedInverseMatrix));
}

TEST_F(TransformTest, InterpolateTransformations) {
  Transform transform1;
  transform1.setPosition(Vector2(1.0f, 2.0f));
  transform1.setRotation(30.0f);
  transform1.setScale(Vector2(4.0f, 5.0f));

  Transform transform2;
  transform2.setPosition(Vector2(6.0f, 7.0f));
  transform2.setRotation(45.0f);
  transform2.setScale(Vector2(2.0f, 3.0f));

  float t = 0.5f;
  Transform interpolatedTransform =
      Transform::interpolate(transform1, transform2, t);

  // Calculate expected interpolation results
  Vector2 expectedPosition = Vector2(3.5f, 4.5f); // Lerp between positions
  float expectedRotation = 37.5f;                 // Lerp between rotations
  Vector2 expectedScale = Vector2(3.0f, 4.0f);    // Lerp between scales

  EXPECT_TRUE(
      areVectorsEqual(interpolatedTransform.getPosition(), expectedPosition));
  EXPECT_FLOAT_EQ(interpolatedTransform.getRotation(), expectedRotation);
  EXPECT_TRUE(areVectorsEqual(interpolatedTransform.getScale(), expectedScale));
}

TEST_F(TransformTest, CopyConstructor) {
  Transform transform1;
  transform1.setPosition(Vector2(1.0f, 2.0f));
  transform1.setRotation(30.0f);
  transform1.setScale(Vector2(4.0f, 5.0f));

  Transform transform2(transform1);
  EXPECT_TRUE(
      areVectorsEqual(transform2.getPosition(), transform1.getPosition()));
  EXPECT_FLOAT_EQ(transform2.getRotation(), transform1.getRotation());
  EXPECT_TRUE(areVectorsEqual(transform2.getScale(), transform1.getScale()));
  EXPECT_TRUE(areMatricesEqual(transform2.getMatrix(), transform1.getMatrix()));
}

TEST_F(TransformTest, LocalToGlobalCoordinates) {
  Transform parentTransform;
  parentTransform.setPosition(Vector2(5.0f, 5.0f));
  parentTransform.setRotation(30.0f);
  parentTransform.setScale(Vector2(2.0f, 2.0f));

  Transform childTransform;
  childTransform.setPosition(Vector2(2.0f, 2.0f));
  childTransform.setRotation(45.0f);
  childTransform.setScale(Vector2(1.5f, 1.5f));

  Vector2 localPoint(1.0f, 1.0f);

  // Calculate expected result manually
  Vector2 childTransformResult = childTransform.transformPoint(localPoint);
  Vector2 expectedResult = parentTransform.transformPoint(childTransformResult);

  // Test the convenience function
  Vector2 actualResult = Transform::localToGlobalCoordinates(
      parentTransform, childTransform, localPoint);
  EXPECT_TRUE(areVectorsEqual(actualResult, expectedResult));
}

TEST_F(TransformTest, GlobalToLocalCoordinates) {
  Transform parentTransform;
  parentTransform.setPosition(Vector2(5.0f, 5.0f));
  parentTransform.setRotation(30.0f);
  parentTransform.setScale(Vector2(2.0f, 2.0f));

  Transform childTransform;
  childTransform.setPosition(Vector2(2.0f, 2.0f));
  childTransform.setRotation(45.0f);
  childTransform.setScale(Vector2(1.5f, 1.5f));

  Vector2 globalPoint(10.0f, 10.0f);

  // Test that global->local->global conversion gets us back to the original
  // point
  Vector2 localPoint = Transform::globalToLocalCoordinates(
      parentTransform, childTransform, globalPoint);
  Vector2 backToGlobal = Transform::localToGlobalCoordinates(
      parentTransform, childTransform, localPoint);
  EXPECT_TRUE(areVectorsEqual(backToGlobal, globalPoint));

  // Calculate expected result manually
  Vector2 parentLocalPoint = parentTransform.inverseTransformPoint(globalPoint);
  Vector2 expectedLocalPoint =
      childTransform.inverseTransformPoint(parentLocalPoint);
  EXPECT_TRUE(areVectorsEqual(localPoint, expectedLocalPoint));
}

TEST_F(TransformTest, EdgeCases) {
  // Test with zero scale
  Transform zeroScaleTransform;
  zeroScaleTransform.setScale(Vector2(0.0f, 0.0f));
  EXPECT_TRUE(
      areVectorsEqual(zeroScaleTransform.getScale(), Vector2(0.0f, 0.0f)));

  // Test with negative scale
  Transform negativeScaleTransform;
  negativeScaleTransform.setScale(Vector2(-1.0f, -2.0f));
  EXPECT_TRUE(areVectorsEqual(negativeScaleTransform.getScale(),
                              Vector2(-1.0f, -2.0f)));

  // Test with large rotation value (should wrap to [0, 360])
  Transform largeRotationTransform;
  largeRotationTransform.setRotation(720.0f); // 2 full rotations
  EXPECT_NEAR(largeRotationTransform.getRotation(), 0.0f, kEpsilon);

  // Test with negative rotation value
  Transform negativeRotationTransform;
  negativeRotationTransform.setRotation(-90.0f);
  EXPECT_NEAR(negativeRotationTransform.getRotation(), 270.0f, kEpsilon);

  // Test with extreme coordinates
  Transform extremePositionTransform;
  extremePositionTransform.setPosition(Vector2(1e6f, -1e6f));
  EXPECT_TRUE(areVectorsEqual(extremePositionTransform.getPosition(),
                              Vector2(1e6f, -1e6f)));
}

TEST_F(TransformTest, NumericalStability) {
  // Test numerical stability with small values
  Transform smallTransform;
  smallTransform.setPosition(Vector2(1e-6f, 1e-6f));
  smallTransform.setScale(Vector2(1e-6f, 1e-6f));
  smallTransform.setRotation(1e-6f);

  EXPECT_TRUE(areVectorsEqual(smallTransform.getPosition(),
                              Vector2(1e-6f, 1e-6f), 1e-7f));
  EXPECT_TRUE(
      areVectorsEqual(smallTransform.getScale(), Vector2(1e-6f, 1e-6f), 1e-7f));
  EXPECT_NEAR(smallTransform.getRotation(), 1e-6f, 1e-7f);

  // Transform a point with very small transform values
  Vector2 point(1.0f, 1.0f);
  Vector2 transformedPoint = smallTransform.transformPoint(point);

  // Calculate expected result
  Vector2 expectedPoint =
      Vector2(1.0f, 1.0f); // With such small values, point should barely change
  expectedPoint =
      Vector2(expectedPoint.x * 1e-6f, expectedPoint.y * 1e-6f); // Scale
  // Rotation by 1e-6 radians is negligible
  expectedPoint = expectedPoint + Vector2(1e-6f, 1e-6f); // Translate

  EXPECT_TRUE(areVectorsEqual(transformedPoint, expectedPoint, 1e-6f));
}

TEST_F(TransformTest, MatrixDecompositionWithNearZeroScale) {
  // Test that matrix decomposition handles near-zero scale correctly
  Transform testTransform;

  // Create a matrix with near-zero x-scale
  Matrix4 identityMatrix = Matrix4(1.0F);
  Matrix4 nearZeroScaleMatrix =
      scale(identityMatrix, Vector3(1e-6F, 1.0F, 1.0F));

  testTransform.setMatrix(nearZeroScaleMatrix);

  // Verify that rotation is defaulted to 0 when x-scale is near zero
  EXPECT_FLOAT_EQ(testTransform.getRotation(), 0.0F);
  EXPECT_TRUE(
      areVectorsEqual(testTransform.getScale(), Vector2(1e-6F, 1.0F), 1e-7F));
}

TEST_F(TransformTest, TransformChaining) {
  // Test chaining transformations
  Transform baseTransform;
  baseTransform.setPosition(Vector2(1.0F, 1.0F));

  // Apply transformations one after another
  Transform chainedTransform = baseTransform;
  chainedTransform.setPosition(chainedTransform.getPosition() +
                               Vector2(1.0F, 1.0F));
  chainedTransform.setRotation(chainedTransform.getRotation() + 30.0F);
  chainedTransform.setScale(chainedTransform.getScale() * Vector2(2.0F, 2.0F));

  // Create a transform with the same final values directly
  Transform directTransform;
  directTransform.setPosition(Vector2(2.0F, 2.0F));
  directTransform.setRotation(30.0F);
  directTransform.setScale(Vector2(2.0F, 2.0F));

  // Verify both approaches give the same result
  EXPECT_TRUE(areVectorsEqual(chainedTransform.getPosition(),
                              directTransform.getPosition()));
  EXPECT_FLOAT_EQ(chainedTransform.getRotation(),
                  directTransform.getRotation());
  EXPECT_TRUE(
      areVectorsEqual(chainedTransform.getScale(), directTransform.getScale()));
  EXPECT_TRUE(areMatricesEqual(chainedTransform.getMatrix(),
                               directTransform.getMatrix()));

  // Test with a small difference
  Transform similarTransform = directTransform;
  similarTransform.setPosition(Vector2(2.0f + 1e-6f, 2.0f));
  EXPECT_TRUE(areVectorsEqual(directTransform.getPosition(),
                              similarTransform.getPosition(), 1e-5f));
}

TEST_F(TransformTest, NumericalStabilityEdgeCases) {
  // Test with very small scale values
  Transform tinyScaleTransform;
  tinyScaleTransform.setScale(Vector2(1e-10f, 1e-10f));

  // Now try to create an inverse transform
  Transform inverseTiny = tinyScaleTransform.inverse();

  // Test properties of the inverse
  Vector2 testPoint(1.0f, 1.0f);
  Vector2 transformed = tinyScaleTransform.transformPoint(testPoint);
  Vector2 backToOriginal = inverseTiny.transformPoint(transformed);

  // With very small scales, we expect larger error bounds
  EXPECT_NEAR(backToOriginal.x, testPoint.x, 1e-3f);
  EXPECT_NEAR(backToOriginal.y, testPoint.y, 1e-3f);
}

} // namespace scene_graph::test