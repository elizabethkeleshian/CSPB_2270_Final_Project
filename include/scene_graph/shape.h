#ifndef SCENE_GRAPH_SHAPE_H
#define SCENE_GRAPH_SHAPE_H

#include "node.h"
#include "types.h"

namespace scene_graph {

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