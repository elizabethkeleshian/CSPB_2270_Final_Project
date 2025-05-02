#include "scene_graph/shape.h"
#include "scene_graph/types.h"

namespace scene_graph {

Shape::Shape(std::string name) : Node(std::move(name)) {}

void Shape::setColor(const Vector4 &color) { color_ = color; }

const Vector4 &Shape::getColor() const { return color_; }

} // namespace scene_graph