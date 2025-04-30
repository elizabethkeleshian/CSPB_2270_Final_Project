// File: test/scene_graph/transform_test.cpp

#include "gtest/gtest.h"
#include "scene_graph/transform.h"
#include "scene_graph/types.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace scene_graph {
namespace test {

// Check if two vectors are equal within a small epsilon value
template<typename T>
bool areVectorsEqual(const T& v1, const T& v2, float epsilon = 0.0001f) {
    return glm::all(glm::epsilonEqual(v1, v2, epsilon));
}

// Check if two matrices are equal within a small epsilon value
bool areMatricesEqual(const Matrix4& m1, const Matrix4& m2, float epsilon = 0.0001f) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::abs(m1[i][j] - m2[i][j]) > epsilon) {
                std::cout << "Matrix difference at [" << i << "][" << j << "]: " 
                          << m1[i][j] << " vs " << m2[i][j] << std::endl;
                return false;
            }
        }
    }
    return true;
}

class TransformTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::cout << "Setting up test fixture..." << std::endl;
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
        std::cout << "Test fixture setup complete." << std::endl;
    }
    
    // Common test transforms
    Transform defaultTransform_;
    Transform translatedTransform_;
    Transform rotatedTransform_;
    Transform scaledTransform_;
    Transform combinedTransform_;
};

TEST_F(TransformTest, DefaultConstructor) {
    std::cout << "Running DefaultConstructor test..." << std::endl;
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0, 0));
    EXPECT_EQ(defaultTransform_.getRotation(), 0.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(1, 1));
    std::cout << "DefaultConstructor test complete." << std::endl;
}

TEST_F(TransformTest, SetAndGetPosition) {
    std::cout << "Running SetAndGetPosition test..." << std::endl;
    Vector2 position(5.0f, 10.0f);
    defaultTransform_.setPosition(position);
    EXPECT_EQ(defaultTransform_.getPosition(), position);
    std::cout << "SetAndGetPosition test complete." << std::endl;
}

TEST_F(TransformTest, SetAndGetRotation) {
    std::cout << "Running SetAndGetRotation test..." << std::endl;
    float rotation = 45.0f;
    rotatedTransform_.setRotation(rotation);
    EXPECT_FLOAT_EQ(rotatedTransform_.getRotation(), rotation);
    std::cout << "SetAndGetRotation test complete." << std::endl;
}

TEST_F(TransformTest, SetAndGetScale) {
    std::cout << "Running SetAndGetScale test..." << std::endl;
    Vector2 scale(2.0f, 3.0f);  
    scaledTransform_.setScale(scale);
    EXPECT_EQ(scaledTransform_.getScale(), scale);
    std::cout << "SetAndGetScale test complete." << std::endl;
}

TEST_F(TransformTest, GetMatrix) {
    std::cout << "Running GetMatrix test..." << std::endl;
    Matrix4 identityMatrix = Matrix4(1.0f);
    
    // A default transformation should be the identity matrix
    Matrix4 defaultMatrix = defaultTransform_.getMatrix();
    std::cout << "Testing default matrix..." << std::endl;
    EXPECT_TRUE(areMatricesEqual(defaultMatrix, identityMatrix));

    // A translated transformation should have a translation component
    Matrix4 translatedMatrix = translatedTransform_.getMatrix();
    Matrix4 expectedTranslated = translate(identityMatrix, Vector3(10.0f, 20.0f, 0.0f));
    std::cout << "Testing translated matrix..." << std::endl;
    EXPECT_TRUE(areMatricesEqual(translatedMatrix, expectedTranslated));

    // A rotated transformation should have a rotation component
    Matrix4 rotatedMatrix = rotatedTransform_.getMatrix();
    Matrix4 expectedRotated = rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f));
    std::cout << "Testing rotated matrix..." << std::endl;
    EXPECT_TRUE(areMatricesEqual(rotatedMatrix, expectedRotated));

    // A scaled transformation should have a scale component
    Matrix4 scaledMatrix = scaledTransform_.getMatrix();
    Matrix4 expectedScaled = scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f));
    std::cout << "Testing scaled matrix..." << std::endl;
    EXPECT_TRUE(areMatricesEqual(scaledMatrix, expectedScaled));

    // A combined transformation should have all components
    Matrix4 combinedMatrix = combinedTransform_.getMatrix();
    Matrix4 expectedCombined = translate(
        rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f)), 
        Vector3(10.0f, 20.0f, 0.0f)
    );
    std::cout << "Testing combined matrix..." << std::endl;
    EXPECT_TRUE(areMatricesEqual(combinedMatrix, expectedCombined));
    std::cout << "GetMatrix test complete." << std::endl;
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

TEST_F(TransformTest, SetMatrixBasicTransformations) {
    Matrix4 identityMatrix = Matrix4(1.0f);
    Matrix4 translatedMatrix = translate(identityMatrix, Vector3(10.0f, 20.0f, 0.0f));
    Matrix4 rotatedMatrix = rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f));
    Matrix4 scaledMatrix = scale(identityMatrix, Vector3(2.0f, 3.0f, 1.0f));
    Matrix4 combinedMatrix = translate(rotate(identityMatrix, radians(90.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(10.0f, 20.0f, 0.0f));
    
    defaultTransform_.setMatrix(translatedMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(10.0f, 20.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 0.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(1, 1));

    defaultTransform_.setMatrix(rotatedMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0.0f, 0.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 90.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(1, 1));

    defaultTransform_.setMatrix(scaledMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0.0f, 0.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 90.0f);  
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(2, 3));

    defaultTransform_.setMatrix(combinedMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(10.0f, 20.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 90.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(2, 3));

    // Test with a matrix that has scaling on the x-axis
    Matrix4 scaledXMatrix = scale(identityMatrix, Vector3(2.0f, 1.0f, 1.0f));
    defaultTransform_.setMatrix(scaledXMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0.0f, 0.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 90.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(2, 1));


    // Test with a matrix that has scaling on the y-axis
    Matrix4 scaledYMatrix = scale(identityMatrix, Vector3(1.0f, 2.0f, 1.0f));
    defaultTransform_.setMatrix(scaledYMatrix);
    EXPECT_EQ(defaultTransform_.getPosition(), Vector2(0.0f, 0.0f));
    EXPECT_EQ(defaultTransform_.getRotation(), 90.0f);
    EXPECT_EQ(defaultTransform_.getScale(), Vector2(1, 2));

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

TEST_F(TransformTest, EdgeCases) {
    // Test various edge cases in a single test
    Transform edgeTransform;
    
    // Zero and negative scale
    edgeTransform.setScale(Vector2(0.0f, 0.0f));
    EXPECT_EQ(edgeTransform.getScale(), Vector2(0.0f, 0.0f));
    edgeTransform.setScale(Vector2(-1.0f, -2.0f));
    EXPECT_EQ(edgeTransform.getScale(), Vector2(-1.0f, -2.0f));
    
    // Large rotation
    edgeTransform.setRotation(720.0f);
    EXPECT_FLOAT_EQ(edgeTransform.getRotation(), 0.0f);
    
    // Extreme coordinates
    edgeTransform.setPosition(Vector2(1e6f, -1e6f));
    EXPECT_EQ(edgeTransform.getPosition(), Vector2(1e6f, -1e6f));
}

TEST_F(TransformTest, NumericalStability) {
    // Test numerical stability with small values
    Transform smallTransform;
    smallTransform.setPosition(Vector2(1e-6f, 1e-6f));
    smallTransform.setScale(Vector2(1e-6f, 1e-6f));
    smallTransform.setRotation(1e-6f);
    
    EXPECT_TRUE(areVectorsEqual(smallTransform.getPosition(), Vector2(1e-6f, 1e-6f), 1e-7f));
    EXPECT_TRUE(areVectorsEqual(smallTransform.getScale(), Vector2(1e-6f, 1e-6f), 1e-7f));
    EXPECT_NEAR(smallTransform.getRotation(), 1e-6f, 1e-7f);
}

TEST_F(TransformTest, MatrixOperations) {
    // Test matrix decomposition and composition together
    Transform transform1, transform2;
    
    // Set up transforms with different properties
    transform1.setPosition(Vector2(5.0f, 10.0f));
    transform1.setRotation(45.0f);
    transform1.setScale(Vector2(2.0f, 3.0f));
    
    transform2.setPosition(Vector2(1.0f, 2.0f));
    transform2.setRotation(30.0f);
    transform2.setScale(Vector2(1.0f, 2.0f));
    
    // Test decomposition
    Matrix4 matrix1 = transform1.getMatrix();
    Transform decomposed;
    decomposed.setMatrix(matrix1);
    EXPECT_TRUE(areVectorsEqual(decomposed.getPosition(), transform1.getPosition(), 1e-5f));
    EXPECT_NEAR(decomposed.getRotation(), transform1.getRotation(), 1e-5f);
    EXPECT_TRUE(areVectorsEqual(decomposed.getScale(), transform1.getScale(), 1e-5f));
    
    // Test composition
    Transform composed = Transform::combine(transform1, transform2);
    Matrix4 expectedMatrix = transform1.getMatrix() * transform2.getMatrix();
    EXPECT_EQ(composed.getMatrix(), expectedMatrix);
}

TEST_F(TransformTest, TransformChaining) {
    // Test chaining and equality in one test
    Transform base, chain, direct;
    
    // Set up base transform
    base.setPosition(Vector2(1.0f, 1.0f));
    
    // Chain transformations
    chain = base;
    chain.setPosition(chain.getPosition() + Vector2(1.0f, 1.0f));
    chain.setRotation(chain.getRotation() + 30.0f);
    chain.setScale(chain.getScale() * Vector2(2.0f, 2.0f));
    
    // Direct transformations
    direct.setPosition(Vector2(2.0f, 2.0f));
    direct.setRotation(30.0f);
    direct.setScale(Vector2(2.0f, 2.0f));
    
    // Verify results
    EXPECT_EQ(chain.getMatrix(), direct.getMatrix());
    
    // Test equality with small differences
    Transform similar = direct;
    similar.setPosition(Vector2(2.0f + 1e-6f, 2.0f));
    EXPECT_TRUE(areVectorsEqual(direct.getPosition(), similar.getPosition(), 1e-5f));
}
} // namespace test
} // namespace scene_graph