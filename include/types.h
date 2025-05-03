#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// Forward declarations for scene_graph namespace
namespace scene_graph {
class Node;
class Shape;
class Rectangle;
class Circle;
class Transform;

//-----------------------------------------------------
// Type aliases for scene_graph specific types
//-----------------------------------------------------
using NodePtr = std::shared_ptr<Node>;
using ShapePtr = std::shared_ptr<Shape>;
using RectanglePtr = std::shared_ptr<Rectangle>;
using CirclePtr = std::shared_ptr<Circle>;

// Utility functions
float wrapAngle(float angle);
} // namespace scene_graph

// Forward declarations for visualization namespace
namespace visualization {
class Renderer;
class Canvas;
} // namespace visualization

//-----------------------------------------------------
// Type aliases - Mathematical types from GLM
//-----------------------------------------------------
/// 2D vector type
using Vector2 = glm::vec2;
/// 3D vector type
using Vector3 = glm::vec3;
/// 4D vector/color type
using Vector4 = glm::vec4;
/// 4x4 transformation matrix
using Matrix4 = glm::mat4;

//-----------------------------------------------------
// GLM transformation functions
//-----------------------------------------------------
using glm::degrees;
using glm::radians;
using glm::rotate;
using glm::scale;
using glm::translate;

//-----------------------------------------------------
// Smart pointer type aliases
//-----------------------------------------------------
using std::shared_ptr;
using std::weak_ptr;

#endif // TYPES_H