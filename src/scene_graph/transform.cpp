#include "scene_graph/transform.h"
#include "scene_graph/types.h"

namespace scene_graph {

/**
 * @brief Default constructor for the Transform class.
 * 
 * This constructor initializes the transform with default values for position, rotation, and scale.
 * It also updates the matrix of the transform.
 */
Transform::Transform() : 
    position_(Vector2(0.0f, 0.0f)), //
    rotation_(radians(0.0f)),
    scale_(Vector2(1.0f, 1.0f)) 
{
    updateMatrix();
}

/**
 * @brief Copy constructor for the Transform class.
 * 
 * This constructor initializes the transform with the values of another transform.
 * It also updates the matrix of the transform.
 * @param original The transform to copy.
 */
Transform::Transform(const Transform& original) 
    : position_(original.position_), rotation_(original.rotation_), scale_(original.scale_) {
    updateMatrix();
}

/**
 * @brief Assignment operator for the Transform class.
 * 
 * This operator assigns the values of another transform to the current transform.
 * It also updates the matrix of the transform.
 * @param original The transform to assign.
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
 * This destructor is defaulted.
 */
Transform::~Transform() = default;

/**
 * @brief Getter for the matrix of the transform.
 * 
 * This method returns the matrix of the transform.
 * @return The matrix of the transform.
 */
Matrix4 Transform::getMatrix() const {
    return matrix_;
}

/**
 * @brief Updates the matrix of the transform.
 * 
 */
void Transform::updateMatrix() {
    matrix_ = Matrix4(1.0f);
    // Implementation will be added later
}

/**
 * @brief Inverses the transform.
 * 
 * @return The inverse of the transform.
 */
Transform Transform::inverse() const {
    Transform result;
    // Implementation will be added later
    return result;
}

/**
 * @brief Transforms a point from local to global coordinates.
 * 
 * @param point The point to transform.
 * @return The transformed point.
 */
Vector2 Transform::transformPoint(const Vector2& point) const {
    return point;  // Implementation will be added later
}

/**
 * @brief Transforms a point from global to local coordinates.
 * 
 * @param point The point to transform.
 * @return The transformed point.
 */
Vector2 Transform::inverseTransformPoint(const Vector2& point) const {
    return point;  // Implementation will be added later
}

/**
 * @brief Combines two transforms.
 * 
 * @param parent The parent transform.
 * @param child The child transform.
 * @return The combined transform.
 */
Transform Transform::combine(const Transform& parent, const Transform& child) {
    Transform result;
    // Implementation will be added later
    return result;
}

/**
 * @brief Interpolates between two transforms.
 * 
 * @param transform1 The first transform.
 * @param transform2 The second transform.
 * @param factor The interpolation factor in the range [0, 1].
 * @return The interpolated transform.
 */
Transform Transform::interpolate(const Transform& transform1, const Transform& transform2, float factor) {
    Transform result;
    // Implementation will be added later
    return result;
}

/**
 * @brief Transforms a point from local to global coordinates.
 * 
 * @param parent The parent transform.
 * @param child The child transform.
 * @param point The point to transform. 
 * @return The transformed point.
 */

Vector2 Transform::globalToLocalCoordinates(const Transform& parent, const Transform& child, const Vector2& point) {
    return point;  // Implementation will be added later
}

} // namespace scene_graph
