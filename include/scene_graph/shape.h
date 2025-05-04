#ifndef SCENE_GRAPH_SHAPE_H
#define SCENE_GRAPH_SHAPE_H

#include <string>

#include "scene_graph/node.h"
#include "types.h"

namespace scene_graph {

class Shape : public Node {
public:
    Shape(std::string name);
    virtual ~Shape() = default;

    // Delete copy constructor and assignment operator
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    // Delete move constructor and assignment operator
    Shape(Shape&&) = delete;
    Shape& operator=(Shape&&) = delete;

    // Color methods
    void setColor(const Vector4& color);
    const Vector4& getColor() const;

    // Pure virtual methods to be implemented by derived classes
    virtual void render() const = 0;
    virtual bool containsPoint(const Vector2& point) const = 0;

private:
    Vector4 color_ = Vector4(1.0F, 1.0F, 1.0F, 1.0F);  // Default white
};

}  // namespace scene_graph

#endif  // SCENE_GRAPH_SHAPE_H