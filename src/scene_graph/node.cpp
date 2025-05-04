#include "scene_graph/node.h"
#include <algorithm>

namespace scene_graph {

/**
 * @brief Constructor for the Node class.
 *
 * @param name node name.
 */
Node::Node(std::string name) : name_(std::move(name)) {}

Node::~Node() {
  std::shared_ptr<Node> parentPtr = parent_.lock();

  if (parentPtr) {
    parentPtr->removeChild(shared_from_this());
  }

  children_.clear();
}

/**
 * @brief Add a child node.
 *
 * @param child The child node to add.
 */
void Node::addChild(const std::shared_ptr<Node> &child) {
  // Remove child from any existing parent
  std::shared_ptr<Node> currentParent = child->parent_.lock();
  if (currentParent) {
    // Remove from current parent
    currentParent->removeChild(child);
  }

  child->parent_ = shared_from_this();
  children_.push_back(child);
}

/**
 * @brief Remove child node.
 *
 * @param child node to remove.
 */
void Node::removeChild(const std::shared_ptr<Node> &child) {
  child->parent_.reset();
  children_.erase(std::remove(children_.begin(), children_.end(), child),
                  children_.end());
}

/**
 * @brief Get the global transform of the node.
 *
 * @return The global transform of the node.
 */
Transform Node::getGlobalTransform() const {
  shared_ptr<Node> parent = parent_.lock();
  if (parent) {
    // Combine parent's global transform with our local transform
    return Transform::combine(parent->getGlobalTransform(), transform_);
  }
  return transform_;
}

/**
 * @brief Set the position of the node.
 *
 * @param position The position of the node.
 */
void Node::setPosition(const Vector2 &position) {
  transform_.setPosition(position);
}

/**
 * @brief Get the position of the node.
 *
 * @return The position of the node.
 */
Vector2 Node::getPosition() const { return transform_.getPosition(); }

/**
 * @brief Set the rotation of the node.
 *
 * @param rotation The rotation of the node.
 */
void Node::setRotation(float rotation) { transform_.setRotation(rotation); }

/**
 * @brief Get the rotation of the node.
 *
 * @return The rotation of the node.
 */
float Node::getRotation() const { return transform_.getRotation(); }

/**
 * @brief Set the scale of the node.
 *
 * @param scale The scale of the node.
 */
void Node::setScale(const Vector2 &scale) { transform_.setScale(scale); }

/**
 * @brief Get the scale of the node.
 *
 * @return The scale of the node.
 */
Vector2 Node::getScale() const { return transform_.getScale(); }

bool Node::hasParent(const shared_ptr<Node> &potentialParent) const {
  return getParent().lock() == potentialParent;
}

bool Node::isOrphaned() const { return getParent().expired(); }

} // namespace scene_graph
