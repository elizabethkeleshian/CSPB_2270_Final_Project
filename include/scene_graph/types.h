#ifndef SCENE_GRAPH_TYPES_H
#define SCENE_GRAPH_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace scene_graph {

// Forward declarations
class Node;
class Shape;
class Rectangle;
class Circle;

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

using NodePtr = shared_ptr<Node>;
using ShapePtr = shared_ptr<Shape>;
using RectanglePtr = shared_ptr<Rectangle>;
using CirclePtr = shared_ptr<Circle>;

// Utility functions
float wrapAngle(float angle);

} // namespace scene_graph

// Forward declarations for visualization namespace
namespace visualization {
class Renderer;
class Canvas;
} // namespace visualization

#endif // SCENE_GRAPH_TYPES_H