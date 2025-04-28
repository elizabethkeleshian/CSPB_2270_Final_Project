#ifndef SCENE_GRAPH_NODE_H
#define SCENE_GRAPH_NODE_H

#include <string>
#include <memory>
#include <vector>

namespace scene_graph {

class Node {
public:
    Node(const std::string& name);
    virtual ~Node() = default;

    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

private:
    std::string name_;
};

} // namespace scene_graph

#endif // SCENE_GRAPH_NODE_H
