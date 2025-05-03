#include "scene_graph/shape.h"
#include "scene_graph/types.h"

namespace scene_graph {

/**
 * @brief Constructor for the Shape base class
 *
 * The Shape class sits at the core of our rendering hierarchy, serving as the
 * base for all drawable objects in the scene. It extends Node to add visual
 * properties like color and abstract operations for rendering and hit
 * detection.
 *
 * @param name Unique identifier for the shape, used in the scene hierarchy
 */
Shape::Shape(std::string name) : Node(std::move(name)) {}

/**
 * @brief Sets the color of the shape
 *
 * Colors use RGBA format with values ranging from 0.0 to 1.0.
 * This affects how the shape appears when rendered.
 * For our car example, this lets us create distinct red and blue cars
 * using the same underlying geometry.
 *
 * @param color Vector4 containing RGBA values
 */
void Shape::setColor(const Vector4 &color) { color_ = color; }

/**
 * @brief Gets the current color of the shape
 *
 * @return RGBA color as Vector4 (each component in range 0.0-1.0)
 */
const Vector4 &Shape::getColor() const { return color_; }

} // namespace scene_graph