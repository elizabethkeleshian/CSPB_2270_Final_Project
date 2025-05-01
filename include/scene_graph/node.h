#ifndef SCENE_GRAPH_NODE_H
#define SCENE_GRAPH_NODE_H

#include <string>
#include <memory>
#include <vector>
#include "transform.h"

namespace scene_graph {

using namespace std;

class Node : public enable_shared_from_this<Node> {
public:
    Node(const string& name);
    virtual ~Node() = default;

    const string& getName() const { return name_; }
    void setName(const string& name) { name_ = name; }
    
    // Hierarchy operations
    weak_ptr<Node> getParent() const { return parent_; }
    const vector<shared_ptr<Node>>& getChildren() const { return children_; }
    void addChild(shared_ptr<Node> child);
    void removeChild(shared_ptr<Node> child);
    
    // Transform operations
    const Transform& getLocalTransform() const { return transform_; }
    Transform& getLocalTransform() { return transform_; }
    void setLocalTransform(const Transform& transform) { transform_ = transform; }
    

    Transform getGlobalTransform() const;
    
    void setPosition(const Vector2& position);
    Vector2 getPosition() const;
    void setRotation(float rotation);
    float getRotation() const;
    void setScale(const Vector2& scale);
    Vector2 getScale() const;

    bool hasParent(const shared_ptr<Node>& potentialParent) const;
    bool isOrphaned() const;

private:
    string name_;
    weak_ptr<Node> parent_;
    vector<shared_ptr<Node>> children_;
    Transform transform_;  // Local transform only
};

} // namespace scene_graph

#endif // SCENE_GRAPH_NODE_H