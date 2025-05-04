#ifndef SCENE_GRAPH_RECTANGLE_H
#define SCENE_GRAPH_RECTANGLE_H

#include "scene_graph/shape.h"

namespace scene_graph {

class Rectangle : public Shape {
public:
    static constexpr Vector2 DEFAULT_SIZE = Vector2(1.0F);
    Rectangle(const std::string& name, const Vector2& size = DEFAULT_SIZE);
    virtual ~Rectangle() = default;

    // Delete copy constructor and assignment operator
    Rectangle(const Rectangle&) = delete;
    Rectangle& operator=(const Rectangle&) = delete;

    // Delete move constructor and assignment operator
    Rectangle(Rectangle&&) = delete;
    Rectangle& operator=(Rectangle&&) = delete;

    // Size methods
    void setSize(const Vector2& size);
    const Vector2& getSize() const;

    // Override virtual methods from Shape
    void render() const override;
    bool containsPoint(const Vector2& point) const override;

private:
    Vector2 size_;
};

}  // namespace scene_graph

#endif  // SCENE_GRAPH_RECTANGLE_H