#include "../../include/scene_graph/shape.h"

namespace scene_graph {
    using namespace std;

    Shape::Shape(const string& name) : Node(name) {}

    void Shape::setColor(const Vector4& color) {
        color_ = color;
    }   

    const Vector4& Shape::getColor() const {
        return color_;
    }

    
} // namespace scene_graph