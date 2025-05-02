#include "../../include/scene_graph/circle.h"

namespace scene_graph {
    using namespace std;

    Circle::Circle(const string& name, float radius) : Shape(name), radius_(radius) {}

    void Circle::setRadius(float radius) {
        radius_ = radius;
    }

    float Circle::getRadius() const {
        return radius_;
    }

    void Circle::render() const {
        // Default implementation
    }

    bool Circle::containsPoint(const Vector2& point) const {
        Vector2 localPoint = getLocalTransform().inverseTransformPoint(point); 
        float distance = length(localPoint);
        return distance <= radius_;
    }
} // namespace scene_graph  