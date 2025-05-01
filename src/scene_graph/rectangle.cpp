#include "../../include/scene_graph/rectangle.h"

namespace scene_graph {
    using namespace std;

    Rectangle::Rectangle(const string& name, const Vector2& size) : Shape(name), size_(size) {}

    void Rectangle::setSize(const Vector2& size) {
        size_ = size;
    }

    const Vector2& Rectangle::getSize() const {
        return size_;
    }

    void Rectangle::render() const {
        // Default implementation
    }
    
    bool Rectangle::containsPoint(const Vector2& point) const {
        Vector2 localPoint = getLocalTransform().inverseTransformPoint(point); 
        float halfWidth = size_.x / 2.0f;
        float halfHeight = size_.y / 2.0f;
        return abs(localPoint.x) <= halfWidth && abs(localPoint.y) <= halfHeight;
    }
} // namespace scene_graph