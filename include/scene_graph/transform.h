#ifndef SCENE_GRAPH_TRANSFORM_H
#define SCENE_GRAPH_TRANSFORM_H

#include <glm/glm.hpp>
#include "types.h"

namespace scene_graph {

/**
 * @brief A class that represents a transform in 2D space.
 * 
 * This class provides a way to store and manipulate the position, rotation, and scale of an object in 2D space.
 * It also provides methods for combining and interpolating transforms, as well as converting between local and global coordinates.
 */
class Transform {
public:
    Transform();
    Transform(const Transform& other);
    Transform& operator=(const Transform& other);
    ~Transform();

    Matrix4 getMatrix() const;

    void setScale(const Vector2& scale) { scale_ = scale; updateMatrix(); }
    void setRotation(float rotation) { rotation_ = radians(rotation); updateMatrix(); }
    void setPosition(const Vector2& position) { position_ = position; updateMatrix(); }

    void setMatrix(const Matrix4& matrix);
    
    const Vector2& getScale() const { return scale_; }
    float getRotation() const { return degrees(rotation_); }
    const Vector2& getPosition() const { return position_; }

    Transform inverse() const; // Returns the inverse of the transform.

    Vector2 transformPoint(const Vector2& point) const; // Transforms a point from local to global coordinates.
    Vector2 inverseTransformPoint(const Vector2& point) const; // Transforms a point from global to local coordinates.

    static Transform combine(const Transform& parent, const Transform& child); // Combines two transforms.
    static Transform interpolate(const Transform& a, const Transform& b, float t); // Interpolates between two transforms.
    static Vector2 localToGlobalCoordinates(const Transform& parent, const Transform& child, const Vector2& point); // Transforms a point from local to global coordinates.
    static Vector2 globalToLocalCoordinates(const Transform& parent, const Transform& child, const Vector2& point); // Transforms a point from global to local coordinates.
    
private:
    void updateMatrix(); // Updates the matrix of the transform.
    Vector2 position_; // The position of the transform.
    float rotation_; // The rotation of the transform.
    Vector2 scale_; // The scale of the transform.
    Matrix4 matrix_; // The matrix of the transform.
};

} // namespace scene_graph

#endif // SCENE_GRAPH_TRANSFORM_H
