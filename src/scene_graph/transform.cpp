#include "scene_graph/transform.h"

#include "types.h"

namespace scene_graph {

/**
 * @brief Default constructor for the Transform class.
 *
 * Creates our standard transform with neutral values:
 * - Position at origin (0, 0) - center of our scene
 * - Zero rotation - aligned with world axes
 * - Unit scale - original size of shapes
 *
 * We use this as the starting point for all scene objects.
 */
Transform::Transform()
    : position_(Vector2(0.0F, 0.0F)),
      rotation_(radians(0.0F)),
      scale_(Vector2(1.0F, 1.0F)),
      matrix_(Matrix4(1.0F)) {
    updateMatrix();
}

/**
 * @brief Copy constructor for the Transform class.
 *
 * Duplicates an existing transform, which we need for cloning objects
 * or creating related transforms with similar properties.
 *
 * We explicitly recalculate the matrix rather than just copying it to
 * ensure consistent behavior with our decomposition methods.
 *
 * @param original The transform to copy values from
 */
Transform::Transform(const Transform& original)
    : position_(original.position_),
      rotation_(original.rotation_),
      scale_(original.scale_),
      matrix_(original.matrix_) {
    updateMatrix();
}

/**
 * @brief Assignment operator for the Transform class.
 *
 * Used when replacing one transform with another. Common during
 * drag operations or when syncing transforms between objects.
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
 * @brief Sets the rotation of the transform.
 *
 * We work with degrees externally for user-friendliness, but store
 * radians internally for efficiency.
 *
 * Normalizes rotation to [0, 360) to avoid growing values when
 * objects rotate continuously in one direction.
 *
 * @param rotation The rotation in degrees to set
 */
void Transform::setRotation(float rotation) {
    // Normalize rotation to [0, 360)
    const float k360 = 360.0F;
    rotation = fmod(rotation, k360);
    if (rotation < 0.0F) {
        rotation += k360;
    }
    rotation_ = radians(rotation);
    updateMatrix();
}

/**
 * @brief Gets the current rotation of the transform.
 *
 * Converts internal radians to degrees and normalizes the result
 * to ensure consistent rotation values for the UI.
 *
 * @return The rotation in degrees in the range [0, 360)
 */
float Transform::getRotation() const {
    float rotationDegrees = degrees(rotation_);

    // Normalize to [0, 360)
    const float k360 = 360.0F;
    rotationDegrees = fmod(rotationDegrees, k360);
    if (rotationDegrees < 0.0F) {
        rotationDegrees += k360;
    }

    return rotationDegrees;
}

/**
 * @brief Updates the internal transformation matrix.
 *
 * Crucial for our scene graph - builds the transformation matrix from
 * position, rotation, and scale. We use the SRT order (Scale → Rotate →
 * Translate) which was chosen after testing various approaches with
 * hierarchical objects.
 *
 * This ordering ensures wheels rotate properly around their centers when
 * attached to moving cars, for example.
 */
void Transform::updateMatrix() {
    Matrix4 scaleMatrix = scale(Matrix4(1.0F), Vector3(scale_.x, scale_.y, 1.0f));
    Matrix4 rotationMatrix = rotate(Matrix4(1.0F), rotation_, Vector3(0.0F, 0.0F, 1.0F));
    Matrix4 translationMatrix = translate(Matrix4(1.0F), Vector3(position_.x, position_.y, 0.0F));
    matrix_ = translationMatrix * rotationMatrix * scaleMatrix;
}

/**
 * @brief Sets the transformation matrix and extracts components.
 *
 * Used when working with external matrix sources or when propagating
 * transforms through the scene graph hierarchy. This was tricky to get right!
 *
 * The decomposition process is sensitive to numerical precision, especially
 * for rotation extraction when scale approaches zero.
 *
 * @param matrix The 4x4 transformation matrix to set
 */
void Transform::setMatrix(const Matrix4& matrix) {
    matrix_ = matrix;

    // Extract translation directly
    position_.x = matrix_[3][0];
    position_.y = matrix_[3][1];

    // Extract scale - use length of the basis vectors
    scale_.x = glm::length(glm::vec2(matrix_[0][0], matrix_[0][1]));
    scale_.y = glm::length(glm::vec2(matrix_[1][0], matrix_[1][1]));

    // Extract rotation
    const float kMinScale = 0.0001F;
    if (scale_.x > kMinScale) {
        // Calculate rotation from the normalized x basis vector
        float cosTheta = matrix_[0][0] / scale_.x;
        float sinTheta = matrix_[0][1] / scale_.x;
        rotation_ = atan2(sinTheta, cosTheta);
    } else {
        // Avoid division by near-zero scale
        rotation_ = 0.0F;
    }
}

/**
 * @brief Calculates the inverse of this transform.
 *
 * Essential for converting between coordinate spaces, especially
 * for hit testing and input handling. When a user clicks on the canvas,
 * we need to convert screen coordinates to object space.
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
 * Core operation for rendering - converts a point in an object's
 * local coordinate system to world space. For example, converting
 * wheel vertices from wheel-local space to world space.
 *
 * @param point The point to transform in local coordinates
 * @return The transformed point in global coordinates
 */
Vector2 Transform::transformPoint(const Vector2& point) const {
    Vector4 homogeneous(point, 0.0F, 1.0F);
    Vector4 transformed = matrix_ * homogeneous;
    return {transformed};
}

/**
 * @brief Transforms a point from global to local coordinates.
 *
 * Useful for hit testing and selection. When a user clicks at a
 * world position, we need to check if that point is inside each object
 * by converting to the object's local space.
 *
 * @param point The point to transform in global coordinates
 * @return The transformed point in local coordinates
 */
Vector2 Transform::inverseTransformPoint(const Vector2& point) const {
    Matrix4 inverseMatrix = glm::inverse(matrix_);
    Vector4 homogeneous(point, 0.0F, 1.0F);
    Vector4 transformed = inverseMatrix * homogeneous;
    return {transformed};
}

/**
 * @brief Combines two transforms into a single transform.
 *
 * The resulting transform represents the composition of parent and child
 * transforms. When applied to a point, it's equivalent to applying the child
 * transform followed by the parent transform.
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
Transform Transform::interpolate(const Transform& start, const Transform& end, float factor) {
    Transform result;
    result.position_ = glm::mix(start.position_, end.position_, factor);
    result.rotation_ = scene_graph::wrapAngle(glm::mix(
        scene_graph::wrapAngle(start.rotation_), scene_graph::wrapAngle(end.rotation_), factor));
    result.scale_ = glm::mix(start.scale_, end.scale_, factor);
    result.updateMatrix();
    return result;
}

/**
 * @brief Transforms a point from local to global coordinates in a hierarchy.
 *
 * This method handles hierarchical transformations where objects can be nested
 * inside each other. For example, in a robot arm:
 * - The hand's position is relative to the lower arm
 * - The lower arm's position is relative to the upper arm
 * - The upper arm's position is relative to the robot base
 * - The robot base's position is relative to the world
 *
 * This method combines all these transformations to find where a point in the
 * child's local space would be in the world.
 *
 * @param parent The parent transform
 * @param child The child transform
 * @param point The point to transform in the child's local coordinates
 * @return The transformed point in global coordinates
 */
Vector2 Transform::localToGlobalCoordinates(const Transform& parent, const Transform& child,
                                            const Vector2& point) {
    Vector2 localPoint = child.transformPoint(point);
    return parent.transformPoint(localPoint);
}

/**
 * @brief Transforms a point from global to local coordinates in a hierarchy.
 *
 * Given a point in global space, transforms it through the inverse of the
 * parent and child transforms to get its position in the child's local space.
 *
 * @param parent The parent transform
 * @param child The child transform
 * @param point The point to transform in global coordinates
 * @return The transformed point in the child's local coordinates
 */
Vector2 Transform::globalToLocalCoordinates(const Transform& parent, const Transform& child,
                                            const Vector2& point) {
    Vector2 globalPoint = parent.inverseTransformPoint(point);
    return child.inverseTransformPoint(globalPoint);
}

}  // namespace scene_graph
