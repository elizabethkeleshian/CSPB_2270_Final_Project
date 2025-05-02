#ifndef SCENE_GRAPH_NODE_H
#define SCENE_GRAPH_NODE_H

#include "scene_graph/transform.h"
#include "scene_graph/types.h"
#include <memory>
#include <vector>

namespace scene_graph {

class Node : public std::enable_shared_from_this<Node> {
public:
  Node(std::string name);
  virtual ~Node() = default;

  const std::string &getName() const { return name_; }
  void setName(const std::string &name) { name_ = name; }

  // Hierarchy operations
  std::weak_ptr<Node> getParent() const { return parent_; }
  const std::vector<std::shared_ptr<Node>> &getChildren() const {
    return children_;
  }
  void addChild(const std::shared_ptr<Node> &child);
  void removeChild(const std::shared_ptr<Node> &child);

  // Transform operations
  const Transform &getLocalTransform() const { return transform_; }
  Transform &getLocalTransform() { return transform_; }
  void setLocalTransform(const Transform &transform) { transform_ = transform; }

  Transform getGlobalTransform() const;

  void setPosition(const Vector2 &position);
  Vector2 getPosition() const;
  void setRotation(float rotation);
  float getRotation() const;
  void setScale(const Vector2 &scale);
  Vector2 getScale() const;

  bool hasParent(const std::shared_ptr<Node> &potentialParent) const;
  bool isOrphaned() const;

private:
  std::string name_;
  std::weak_ptr<Node> parent_;
  std::vector<std::shared_ptr<Node>> children_;
  Transform transform_; // Local transform only
};

} // namespace scene_graph

#endif // SCENE_GRAPH_NODE_H