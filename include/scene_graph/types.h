#ifndef SCENE_GRAPH_TYPES_H
#define SCENE_GRAPH_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include <memory>

// Forward declarations for visualization namespace
namespace visualization {
    class Renderer;
    class Canvas;
}

namespace scene_graph { 

// Forward declarations
class Node;
class Shape;
class Rectangle;
class Circle;

// Vector types
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

// Matrix types
using Matrix4 = glm::mat4;

// Transformation functions
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::radians;
using glm::degrees;

// Using std namespace for shared_ptr
using std::shared_ptr;
using std::weak_ptr;

// Smart pointer type aliases
// using NodePtr = shared_ptr<Node>;
// using RendererPtr = shared_ptr<visualization::Renderer>;
// using CanvasPtr = shared_ptr<visualization::Canvas>;
// using ShapePtr = shared_ptr<Shape>;
// using RectanglePtr = shared_ptr<Rectangle>;
// using CirclePtr = shared_ptr<Circle>;

/**
 * @brief Wraps an angle to the range [-pi, pi] radians, ensuring shortest path interpolation.
 * 
 * This function takes an angle in radians and returns an equivalent angle
 * that is within the range [-pi, pi]. It shifts the angle by pi, wraps it
 * to [0, 2*pi], then shifts it back to [-pi, pi].
 * 
 * This wrapping is needed for interpolation because it ensures
 * that when interpolating between angles, the shortest path around the circle
 * is taken. For example:
 * - If interpolating from 3*pi/4 to -3*pi/4, it will go through pi/2
 * - If interpolating from -3*pi/4 to 3*pi/4, it will go through -pi/2
 * 
 * @param angle The angle in radians to wrap
 * @return The wrapped angle in the range [-pi, pi] radians
 */
float wrapAngle(float angle);

} // namespace scene_graph

#endif // SCENE_GRAPH_TYPES_H