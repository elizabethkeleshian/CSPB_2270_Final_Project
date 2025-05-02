#ifndef SCENE_GRAPH_CIRCLE_H
#define SCENE_GRAPH_CIRCLE_H

#include "scene_graph/shape.h"
#include "scene_graph/types.h"
#include <glm/glm.hpp>

namespace scene_graph {

class Circle : public Shape {

public:
  Circle(const std::string &name, float radius = DEFAULT_RADIUS);
  virtual ~Circle() = default;

  // Delete copy constructor and assignment operator
  Circle(const Circle &) = delete;
  Circle &operator=(const Circle &) = delete;

  // Delete move constructor and assignment operator
  Circle(Circle &&) = delete;
  Circle &operator=(Circle &&) = delete;

  // Radius methods
  void setRadius(float radius);
  float getRadius() const;

  // Override virtual methods from Shape
  void render() const override;
  bool containsPoint(const Vector2 &point) const override;

private:
  float radius_;
  static constexpr float DEFAULT_RADIUS = 0.5F;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_CIRCLE_H