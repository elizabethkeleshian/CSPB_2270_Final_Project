#ifndef SCENE_GRAPH_TYPES_H
#define SCENE_GRAPH_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

namespace scene_graph { 

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
} // namespace scene_graph

#endif // SCENE_GRAPH_TYPES_H