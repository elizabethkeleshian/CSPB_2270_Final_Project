#ifndef SCENE_GRAPH_SHAPE_H
#define SCENE_GRAPH_SHAPE_H

#include "scene_graph/node.h"
#include <glm/glm.hpp>

namespace scene_graph {
    
// Use the glm type directly
using Vector2 = glm::vec2;
using Vector4 = glm::vec4;

class Shape : public Node {
public:
    Shape(const std::string& name);
    virtual ~Shape() = default;

    // Color methods
    void setColor(const Vector4& color);
    const Vector4& getColor() const;

    // Pure virtual methods to be implemented by derived classes
    virtual void render() const = 0;
    virtual bool containsPoint(const Vector2& point) const = 0;

protected:
    Vector4 color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // Default white
};

} // namespace scene_graph

#endif // SCENE_GRAPH_SHAPE_H 