#include "scene_graph/transform.h"
#include "scene_graph/types.h"
#include <iostream>

namespace scene_graph {

/**
 * @brief Default constructor for the Transform class.
 * 
 * Initializes the transform with:
 * - Position at origin (0, 0)
 * - No rotation (0 degrees)
 * - Uniform scale of 1.0
 * 
 * The transformation matrix is automatically updated to reflect these values.
 */
Transform::Transform() : 
    position_(Vector2(0.0f, 0.0f)),
    rotation_(radians(0.0f)),
    scale_(Vector3(1.0f, 1.0f, 1.0f)) 
{
    updateMatrix();
}

/**
 * @brief Copy constructor for the Transform class.
 * 
 * Creates a new transform with the same properties as the original:
 * - Position
 * - Rotation
 * - Scale
 * 
 * The transformation matrix is automatically updated to reflect these values.
 * 
 * @param original The transform to copy values from
 */
Transform::Transform(const Transform& original) 
    : position_(original.position_), 
      rotation_(original.rotation_), 
      scale_(original.scale_) 
{
    updateMatrix();
}

/**
 * @brief Assignment operator for the Transform class.
 * 
 * Copies all properties from the original transform:
 * - Position
 * - Rotation
 * - Scale
 * 
 * The transformation matrix is automatically updated to reflect these values.
 * 
 * @param original The transform to assign values from
 * @return Reference to this transform after assignment
 */
Transform& Transform::operator=(const Transform& original) {
    position_ = original.position_;
    rotation_ = original.rotation_;
    scale_ = original.scale_;
    updateMatrix();
    return *this;
}

/**
 * @brief Destructor for the Transform class.
 * 
 * Default destructor as no special cleanup is required.
 */
Transform::~Transform() = default;

/**
 * @brief Gets the current transformation matrix.
 * 
 * The transformation matrix represents the combined effect of:
 * 1. Scaling
 * 2. Rotation
 * 3. Translation
 * 
 * The matrix is stored in column-major order as required by OpenGL.
 * 
 * @return The 4x4 transformation matrix
 */
Matrix4 Transform::getMatrix() const {
    return matrix_;
}

/**
 * @brief Updates the internal transformation matrix.
 * 
 * The matrix is constructed by applying transformations in the following order:
 * 1. Scale
 * 2. Rotate
 * 3. Translate
 * 
 * This order ensures that transformations are applied in the expected way:
 * - Objects are first scaled around their origin
 * - Then rotated around their origin
 * - Finally translated to their position
 */
void Transform::updateMatrix() {
    matrix_ = Matrix4(1.0f);
    matrix_ = scale(matrix_, Vector3(scale_, 1.0f));
    matrix_ = rotate(matrix_, rotation_, Vector3(0.0f, 0.0f, 1.0f));
    matrix_ = translate(matrix_, Vector3(position_, 0.0f));
}

/**
 * @brief Sets the transformation matrix and extracts position, rotation, and scale.
 * 
 * Decomposes the given matrix into its components:
 * - Position is extracted from the translation components (m[3][0] and m[3][1])
 * - Scale is calculated from the length of the basis vectors
 * - Rotation is calculated from the angle of the x-axis basis vector
 * 
 * If the x-axis scale is near zero, rotation is set to 0 to avoid division by zero.
 * 
 * @param matrix The 4x4 transformation matrix to set
 */
void Transform::setMatrix(const Matrix4& matrix) {
    matrix_ = matrix;
    
    // Extract translation from matrix
    position_ = Vector2(matrix_[3][0], matrix_[3][1]);

    // Calculate scale from basis vectors
    scale_.x = length(Vector2(matrix_[0][0], matrix_[0][1]));
    scale_.y = length(Vector2(matrix_[1][0], matrix_[1][1]));

    // Calculate rotation from x-axis basis vector
    if (scale_.x > 0.0001f) {
        float xAxis = matrix_[0][0] / scale_.x;
        float yAxis = matrix_[1][0] / scale_.x;
        rotation_ = atan2(yAxis, xAxis);
    } else {
        rotation_ = 0.0f;
    }
}

/**
 * @brief Calculates the inverse of this transform.
 * 
 * The inverse transform will undo the transformations applied by this transform.
 * For a transform T, the inverse T⁻¹ satisfies: T * T⁻¹ = I (identity matrix).
 * 
 * The inverse is calculated by inverting the transformation matrix.
 * 
 * @return A new Transform representing the inverse transformation
 */
Transform Transform::inverse() const {
    Transform result;
    result.setMatrix(glm::inverse(matrix_));
    return result;
}

/**
 * @brief Transforms a point from local to global coordinates.
 * 
 * Applies this transform's matrix to the given point:
 * 1. Scales the point
 * 2. Rotates the point
 * 3. Translates the point
 * 
 * @param point The point to transform in local coordinates
 * @return The transformed point in global coordinates
 */
Vector2 Transform::transformPoint(const Vector2& point) const {
    Vector4 transformed = matrix_ * Vector4(point, 0.0f, 1.0f);
    return Vector2(transformed.x, transformed.y);
}

/**
 * @brief Transforms a point from global to local coordinates.
 * 
 * Applies the inverse of this transform's matrix to the given point.
 * This effectively undoes the transformation applied by transformPoint.
 * 
 * @param point The point to transform in global coordinates
 * @return The transformed point in local coordinates
 */
Vector2 Transform::inverseTransformPoint(const Vector2& point) const {
    Vector4 transformed = inverse().getMatrix() * Vector4(point, 0.0f, 1.0f);
    return Vector2(transformed.x, transformed.y);
}

/**
 * @brief Combines two transforms into a single transform.
 * 
 * The resulting transform represents the composition of parent and child transforms.
 * When applied to a point, it's equivalent to applying the child transform followed by the parent transform.
 * 
 * The composition is calculated by multiplying the transformation matrices:
 * result = parent.matrix * child.matrix
 * 
 * @param parent The parent transform
 * @param child The child transform
 * @return A new Transform representing the combined transformation
 */
Transform Transform::combine(const Transform& parent, const Transform& child) {
    Transform result;
    result.matrix_ = parent.matrix_ * child.matrix_;
    result.setMatrix(result.matrix_);
    return result;
}

/**
 * @brief Interpolates between two transforms.
 * 
 * Linearly interpolates each component of the transform:
 * - Position is interpolated using linear interpolation
 * - Rotation is interpolated using linear interpolation of angles
 * - Scale is interpolated using linear interpolation
 * 
 * @param transform1 The first transform
 * @param transform2 The second transform
 * @param factor The interpolation factor in the range [0, 1]
 * @return A new Transform representing the interpolated transformation
 */
Transform Transform::interpolate(const Transform& transform1, const Transform& transform2, float factor) {
    Transform result;
    result.position_ = glm::mix(transform1.position_, transform2.position_, factor);
    result.rotation_ = wrapAngle(glm::mix(wrapAngle(transform1.rotation_), wrapAngle(transform2.rotation_), factor));
    result.scale_ = glm::mix(transform1.scale_, transform2.scale_, factor);
    result.updateMatrix();
    return result;
}

/**
 * @brief Transforms a point from local to global coordinates in a hierarchy.
 * 
 * This method handles hierarchical transformations where objects can be nested inside each other.
 * For example, in a robot arm:
 * - The hand's position is relative to the lower arm
 * - The lower arm's position is relative to the upper arm
 * - The upper arm's position is relative to the robot base
 * - The robot base's position is relative to the world
 * 
 * This method combines all these transformations to find where a point in the child's
 * local space would be in the world.
 * 
 * @param parent The parent transform
 * @param child The child transform
 * @param point The point to transform in the child's local coordinates
 * @return The transformed point in global coordinates
 */
Vector2 Transform::localToGlobalCoordinates(const Transform& parent, const Transform& child, const Vector2& point) {
    Vector2 localPoint = child.transformPoint(point);
    return parent.transformPoint(localPoint);
}

/**
 * @brief Transforms a point from global to local coordinates in a hierarchy.
 * 
 * Given a point in global space, transforms it through the inverse of the parent and child transforms
 * to get its position in the child's local space.
 * 
 * @param parent The parent transform
 * @param child The child transform
 * @param point The point to transform in global coordinates
 * @return The transformed point in the child's local coordinates
 */
Vector2 Transform::globalToLocalCoordinates(const Transform& parent, const Transform& child, const Vector2& point) {
    Vector2 globalPoint = parent.inverseTransformPoint(point);
    return child.inverseTransformPoint(globalPoint);
}

} // namespace scene_graph
