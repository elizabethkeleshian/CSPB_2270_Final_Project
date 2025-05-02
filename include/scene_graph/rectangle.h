#ifndef SCENE_GRAPH_RECTANGLE_H
#define SCENE_GRAPH_RECTANGLE_H

#include "scene_graph/shape.h"

namespace scene_graph {

class Rectangle : public Shape {
public:
    Rectangle(const std::string& name, const Vector2& size = Vector2(1.0f));
    virtual ~Rectangle() = default;

    // Size methods
    void setSize(const Vector2& size);
    const Vector2& getSize() const;

    // Override virtual methods from Shape
    void render() const override;
    bool containsPoint(const Vector2& point) const override;

private:
    Vector2 size_;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_RECTANGLE_H 