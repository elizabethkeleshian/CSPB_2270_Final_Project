#ifndef SCENE_GRAPH_TRANSFORM_H
#define SCENE_GRAPH_TRANSFORM_H

#include "types.h"

namespace scene_graph {

/**
 * @brief A class that represents a transform in 2D space.
 *
 * This class provides a way to store and manipulate the position, rotation, and
 * scale of an object in 2D space. It also provides methods for combining and
 * interpolating transforms, as well as converting between local and global
 * coordinates.
 */
class Transform {
public:
  Transform();
  Transform(const Transform &original);
  Transform &operator=(const Transform &original);
  ~Transform() = default;
  Transform(Transform &&) = default;

  // delete move assignment operator
  Transform &operator=(Transform &&) = default;

  [[nodiscard]] const Matrix4 &getMatrix() const { return matrix_; }

  void setScale(const Vector2 &scale) {
    scale_ = scale;
    updateMatrix();
  }

  void setPosition(const Vector2 &position) {
    position_ = position;
    updateMatrix();
  }
  void setRotation(float rotation);

  void setMatrix(const Matrix4 &matrix);

  [[nodiscard]] const Vector2 &getScale() const { return scale_; }
  // float getRotation() const { return degrees(rotation_); }
  [[nodiscard]] float getRotation() const;
  [[nodiscard]] const Vector2 &getPosition() const { return position_; }

  [[nodiscard]] Transform inverse() const;

  [[nodiscard]] Vector2 transformPoint(const Vector2 &point) const;
  [[nodiscard]] Vector2 inverseTransformPoint(const Vector2 &point) const;

  static Transform combine(const Transform &parent, const Transform &child);
  static Transform interpolate(const Transform &start, const Transform &end,
                               float factor);
  static Vector2 localToGlobalCoordinates(const Transform &parent,
                                          const Transform &child,
                                          const Vector2 &point);
  static Vector2 globalToLocalCoordinates(const Transform &parent,
                                          const Transform &child,
                                          const Vector2 &point);

private:
  void updateMatrix();
  Vector2 position_;
  float rotation_;
  Vector2 scale_;
  Matrix4 matrix_;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_TRANSFORM_H
