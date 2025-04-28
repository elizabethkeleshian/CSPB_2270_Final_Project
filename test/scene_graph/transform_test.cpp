// File: test/scene_graph/transform_test.cpp

#include "gtest/gtest.h"
#include "scene_graph/transform.h"
#include "scene_graph/types.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene_graph {
namespace test {


// Check if two vectors are equal within a small epsilon value
bool areVectorsEqual(const Vector3& v1, const Vector3& v2, float epsilon = 0.0001f) {
    return glm::all(glm::epsilonEqual(v1, v2, epsilon));
}

class TransformTest : public ::testing::Test {
protected:
    void SetUp() override 
    {
        // Initialize common test values
        defaultTransform_ = Transform();
        translatedTransform_ = Transform();
        translatedTransform_.setPosition(glm::vec2(10.0f, 20.0f));
        
        rotatedTransform_ = Transform();
        rotatedTransform_.setRotation(90.0f);
        
        scaledTransform_ = Transform();
        scaledTransform_.setScale(glm::vec2(2.0f, 3.0f));
        
        combinedTransform_ = Transform();
        combinedTransform_.setPosition(glm::vec2(10.0f, 20.0f));
        combinedTransform_.setRotation(90.0f);
        combinedTransform_.setScale(glm::vec2(2.0, 3.0f));
    }
    
    // Common test transforms
    Transform defaultTransform_;
    Transform translatedTransform_;
    Transform rotatedTransform_;
    Transform scaledTransform_;
    Transform combinedTransform_;
};

TEST_F(TransformTest, DefaultConstructor) {
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0, 0));
    EXPECT_EQ(defaultTransform_.getRotation(), 0.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(1, 1));
}

TEST_F(TransformTest, SetAndGetPosition) {
    Vector2 position(5.0f, 10.0f);
    defaultTransform_.setPosition(position);
    EXPECT_EQ(defaultTransform_.getPosition(), position);
}

TEST_F(TransformTest, SetAndGetRotation) {
    float rotation = 45.0f;
    rotatedTransform_.setRotation(rotation);
    EXPECT_FLOAT_EQ(rotatedTransform_.getRotation(), rotation);
}

TEST_F(TransformTest, SetAndGetScale) {
    Vector2 scale(2.0f, 3.0f);  
    scaledTransform_.setScale(scale);
    EXPECT_EQ(scaledTransform_.getScale(), scale);
}

TEST_F(TransformTest, GetMatrix) {
    Matrix4 identityMatrix = Matrix4(1.0f);
    // A default transformation should be the identity matrix
    Matrix4 defaultMatrix = defaultTransform_.getMatrix();
    EXPECT_EQ(defaultMatrix, identityMatrix);

    // A translated transformation should have a translation component
    Matrix4 translatedMatrix = translatedTransform_.getMatrix();
    EXPECT_EQ(translatedMatrix, translate(identityMatrix, Vector3(10.0f, 20.0f, 0.0f)));

    // A rotated transformation should have a rotation component
    // For 90-degree rotation around Z:
    // cos(90°) = 0, sin(90°) = 1
    Matrix4 rotatedMatrix = rotatedTransform_.getMatrix();
    EXPECT_EQ(rotatedMatrix, rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f)));

    // A scaled transformation should have a scale component
    Matrix4 scaledMatrix = scaledTransform_.getMatrix();
    EXPECT_EQ(scaledMatrix, scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f)));

    // A combined transformation should have all components
    Matrix4 combinedMatrix = combinedTransform_.getMatrix();
    EXPECT_EQ(combinedMatrix, translate(rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(10.0f, 20.0f, 0.0f)));
}

TEST_F(TransformTest, TransformPoint) {
    Vector2 point(1.0f, 1.0f);

    // A defualt transformation should not change the point
    Vector2 transformedPoint = defaultTransform_.transformPoint(point);
    EXPECT_EQ(transformedPoint, point);

    // A translated point should be moved by the translation
    Vector2 translatedPoint = translatedTransform_.transformPoint(point);
    EXPECT_EQ(translatedPoint, Vector2(11.0f, 21.0f));

    // A rotated point should be rotated by the rotation
    Vector2 rotatedPoint = rotatedTransform_.transformPoint(point);
    EXPECT_EQ(rotatedPoint, Vector2(-1.0f, 1.0f));

    // A scaled point should be scaled by the scale
    Vector2 scaledPoint = scaledTransform_.transformPoint(point);
    EXPECT_EQ(scaledPoint, Vector2(2.0f, 3.0f));

    // A combined transformation should apply all transformations
    Vector2 combinedPoint = combinedTransform_.transformPoint(point);
    EXPECT_EQ(combinedPoint, Vector2(-1.0f, 11.0f));
}

TEST_F(TransformTest, InverseTransformPoint) {
    Vector2 point(5.0f, 7.0f);

    // A defualt transformation should not change the point
    Vector2 transformedPoint = defaultTransform_.transformPoint(point);
    EXPECT_EQ(transformedPoint, point);

    // A translated point should be moved by the translation
    Vector2 translatedPoint = translatedTransform_.transformPoint(point);
    EXPECT_EQ(translatedPoint, Vector2(15.0f, 27.0f));

    // A rotated point should be rotated by the rotation
    Vector2 rotatedPoint = rotatedTransform_.transformPoint(point);
    EXPECT_EQ(rotatedPoint, Vector2(-7.0f, 5.0f));

    // A scaled point should be scaled by the scale
    Vector2 scaledPoint = scaledTransform_.transformPoint(point);
    EXPECT_EQ(scaledPoint, Vector2(10.0f, 21.0f));

    // A combined transformation should apply all transformations
    Vector2 combinedPoint = combinedTransform_.transformPoint(point);
    EXPECT_EQ(combinedPoint, Vector2(-7.0f, 11.0f));
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

    Vector2 testPoint(1.0f, 1.0f);
    Vector2 parentResult = parentTransform.transformPoint(testPoint);
    Vector2 childResult = childTransform.transformPoint(testPoint);
    
    Vector2 expectedCombine = parentTransform.transformPoint(childResult);
    
    Transform combinedTransform = Transform::combine(parentTransform, childTransform);
    Vector2 actualCombine = combinedTransform.transformPoint(testPoint);
    EXPECT_EQ(actualCombine, expectedCombine);

    Matrix4 parentMatrix = parentTransform.getMatrix();
    Matrix4 childMatrix = childTransform.getMatrix();
    Matrix4 expectedCombineMatrix = parentMatrix * childMatrix;
    Matrix4 actualCombineMatrix = combinedTransform.getMatrix();
    EXPECT_EQ(actualCombineMatrix, expectedCombineMatrix);
}

TEST_F(TransformTest, InverseCombineTransformations) {
    Transform parentTransform ;
    parentTransform.setPosition(Vector2(1.0f, 2.0f));
    parentTransform.setRotation(30.0f);
    parentTransform.setScale(Vector2(4.0f, 5.0f));

    Transform childTransform;
    childTransform.setPosition(Vector2(6.0f, 7.0f));
    childTransform.setRotation(45.0f);
    childTransform.setScale(Vector2(2.0f, 3.0f));

    Transform combinedTransform = Transform::combine(parentTransform, childTransform);   
    Transform inverseCombinedTransform = combinedTransform.inverse();

    Vector2 testPoint(1.0f, 1.0f);
    Vector2 parentResult = parentTransform.transformPoint(testPoint);
    Vector2 childResult = childTransform.transformPoint(testPoint);
    Vector2 combinedResult = combinedTransform.transformPoint(testPoint);
    Vector2 inverseCombinedResult = inverseCombinedTransform.transformPoint(combinedResult);
    EXPECT_EQ(inverseCombinedResult, testPoint);

    Matrix4 parentMatrix = parentTransform.getMatrix();
    Matrix4 childMatrix = childTransform.getMatrix();
    Matrix4 expectedCombineMatrix = parentMatrix * childMatrix;
    Matrix4 actualCombineMatrix = combinedTransform.getMatrix();
    EXPECT_EQ(actualCombineMatrix, expectedCombineMatrix);

    Matrix4 expectedInverseMatrix = glm::inverse(expectedCombineMatrix);
    Matrix4 actualInverseMatrix = inverseCombinedTransform.getMatrix();
    EXPECT_EQ(actualInverseMatrix, expectedInverseMatrix);
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
    Transform interpolatedTransform = Transform::interpolate(transform1, transform2, t);    

    EXPECT_EQ(interpolatedTransform.getPosition(), Vector2(3.5f, 4.5f));
    EXPECT_EQ(interpolatedTransform.getRotation(), 37.5f);
    EXPECT_EQ(interpolatedTransform.getScale(), Vector2(3.0f, 4.0f));
}

TEST_F(TransformTest, CopyConstructor) {
    Transform transform1;
    transform1.setPosition(Vector2(1.0f, 2.0f));
    transform1.setRotation(30.0f);
    transform1.setScale(Vector2(4.0f, 5.0f));

    Transform transform2(transform1);
    EXPECT_EQ(transform2.getPosition(), Vector2(1.0f, 2.0f));
    EXPECT_EQ(transform2.getRotation(), 30.0f);
    EXPECT_EQ(transform2.getScale(), Vector2(4.0f, 5.0f));
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

    Vector2 childTransformResult = childTransform.transformPoint(localPoint);
    Vector2 parentTransformResult = parentTransform.transformPoint(childTransformResult);
    Vector2 actualGlobal = Transform::localToGlobalCoordinates(parentTransform, childTransform, localPoint);
    EXPECT_EQ(actualGlobal, parentTransformResult);
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

    Vector2 localPoint = Transform::globalToLocalCoordinates(parentTransform, childTransform, globalPoint);
    Vector2 backToGlobal = Transform::localToGlobalCoordinates(parentTransform, childTransform, localPoint);
    EXPECT_EQ(backToGlobal, globalPoint);
}
} // namespace test
} // namespace scene_graph