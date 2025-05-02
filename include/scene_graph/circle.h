#ifndef SCENE_GRAPH_CIRCLE_H
#define SCENE_GRAPH_CIRCLE_H

#include "scene_graph/shape.h"

namespace scene_graph {

class Circle : public Shape {

public:
    Circle(const std::string& name, float radius = 0.5f);
    virtual ~Circle() = default;

    // Radius methods
    void setRadius(float radius);
    float getRadius() const;

    // Override virtual methods from Shape
    void render() const override;
    bool containsPoint(const Vector2& point) const override;

private:
    float radius_;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_CIRCLE_H 