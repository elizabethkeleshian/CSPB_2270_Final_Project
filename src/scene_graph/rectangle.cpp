#include "scene_graph/rectangle.h"

namespace scene_graph {

/**
 * @brief Constructor for Rectangle shapes
 *
 * Rectangles are one of our primary building blocks for scene construction.
 * They're used for car bodies, buildings, and other geometric elements.
 *
 * @param name Unique identifier for this rectangle in the scene
 * @param size Vector2 containing width and height (defaults to 1x1 unit square)
 */
Rectangle::Rectangle(const std::string &name, const Vector2 &size)
    : Shape(name), size_(size) {}

/**
 * @brief Sets the size of the rectangle
 *
 * Updates the width and height, maintaining the rectangle's center point.
 * The transform's scale is separate from this size - the final visible
 * dimensions will be size * scale.
 *
 * @param size Vector2 containing width (x) and height (y)
 */
void Rectangle::setSize(const Vector2 &size) { size_ = size; }

/**
 * @brief Gets the current size of the rectangle
 *
 * @return Vector2 containing width (x) and height (y)
 */
const Vector2 &Rectangle::getSize() const { return size_; }

/**
 * @brief Renders the rectangle
 *
 * This is a placeholder in our architecture - actual rendering is
 * handled by the visualization system which calls this method.
 * This separation keeps rendering details out of our scene graph.
 */
void Rectangle::render() const {
  // Default implementation
}

/**
 * @brief Tests if a point is inside the rectangle
 *
 * Critical for mouse interaction and selection. We convert the world
 * point to the rectangle's local coordinate space and then check if
 * it's within the rectangle's bounds.
 *
 * @param point The point to test in world coordinates
 * @return true if the point is inside the rectangle, false otherwise
 */
bool Rectangle::containsPoint(const Vector2 &point) const {
  Vector2 localPoint = getLocalTransform().inverseTransformPoint(point);
  float halfWidth = size_.x / 2.0F;
  float halfHeight = size_.y / 2.0F;
  return abs(localPoint.x) <= halfWidth && abs(localPoint.y) <= halfHeight;
}
} // namespace scene_graph