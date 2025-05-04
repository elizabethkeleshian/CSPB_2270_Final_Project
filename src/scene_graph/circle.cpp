#include "scene_graph/circle.h"

#include "types.h"

namespace scene_graph {

/**
 * @brief Constructor for Circle shapes
 *
 * Circles are ideal for wheels, buttons, and connection points in our
 * interactive scene editor. Unlike rectangles, they maintain their
 * circular appearance regardless of non-uniform scaling.
 *
 * @param name Unique identifier for this circle in the scene
 * @param radius Distance from center to edge (defaults to 0.5 units)
 */
Circle::Circle(const std::string& name, float radius) : Shape(name), radius_(radius) {
}

/**
 * @brief Sets the radius of the circle
 *
 * Adjusts the size of the circle while maintaining its center point.
 * Like with rectangles, the final visible radius is affected by the
 * transform's scale.
 *
 * @param radius New radius value (distance from center to edge)
 */
void Circle::setRadius(float radius) {
    radius_ = radius;
}

/**
 * @brief Gets the current radius of the circle
 *
 * @return Current radius value
 */
float Circle::getRadius() const {
    return radius_;
}

/**
 * @brief Renders the circle
 *
 * This is a placeholder in our architecture. The actual rendering
 * happens in the visualization::Renderer class, which approximates
 * the circle as a 32-segment polygon.
 */
void Circle::render() const {
    // Default implementation
}

/**
 * @brief Tests if a point is inside the circle
 *
 * Used for hit detection when selecting objects. We transform the
 * world point to the circle's local coordinate space and check if
 * the distance from origin is less than or equal to the radius.
 *
 * This makes wheels selectable in our car example, even when the
 * car is rotated or scaled.
 *
 * @param point The point to test in world coordinates
 * @return true if the point is inside the circle, false otherwise
 */
bool Circle::containsPoint(const Vector2& point) const {
    Vector2 localPoint = getLocalTransform().inverseTransformPoint(point);
    float distance = length(localPoint);
    return distance <= radius_;
}
}  // namespace scene_graph