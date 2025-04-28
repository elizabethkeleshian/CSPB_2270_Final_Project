#ifndef SCENE_GRAPH_TRANSFORM_H
#define SCENE_GRAPH_TRANSFORM_H

#include <glm/glm.hpp>
#include "types.h"

namespace scene_graph {

class Transform {
public:
    Transform();
    Transform(const Transform& other);
    Transform& operator=(const Transform& other);
    ~Transform();

    Matrix4 getMatrix() const;

    void setScale(const Vector2& scale) { scale_ = scale; updateMatrix(); }
    void setRotation(float rotation) { rotation_ = rotation; updateMatrix(); }
    void setPosition(const Vector2& position) { position_ = position; updateMatrix(); }

    const Vector2& getScale() const { return scale_; }
    float getRotation() const { return rotation_; }
    const Vector2& getPosition() const { return position_; }

    Transform inverse() const;

    Vector2 transformPoint(const Vector2& point) const;
    Vector2 inverseTransformPoint(const Vector2& point) const;

    static Transform combine(const Transform& parent, const Transform& child);
    static Transform interpolate(const Transform& a, const Transform& b, float t);
    static Vector2 localToGlobalCoordinates(const Transform& parent, const Transform& child, const Vector2& point);
    static Vector2 globalToLocalCoordinates(const Transform& parent, const Transform& child, const Vector2& point);
    
private:
    void updateMatrix();
    Vector2 position_;
    float rotation_;
    Vector2 scale_;
    Matrix4 matrix_;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_TRANSFORM_H
