#pragma once

#include <memory>
#include <vector>
#include "visualization/renderer.h"
#include <glm/glm.hpp>

// Forward declarations
namespace scene_graph {
    class Node;
    class Shape;
    // Vector2 is an alias, not a class
    using Vector2 = glm::vec2;
}

namespace visualization {

/**
 * @brief The Canvas class is responsible for rendering the scene graph.
 * 
 * This class manages the rendering of the scene graph, including adding
 * and removing shapes, setting the root node, and selecting nodes.
 */
class Canvas {
    public:
        Canvas();
        ~Canvas();

    // Initialize the canvas with a renderer
    bool initialize(std::shared_ptr<Renderer> renderer);

    // Set the root node of the canvas
    void setRoot(std::shared_ptr<scene_graph::Node> root);
    std::shared_ptr<scene_graph::Node> getRoot() const;

     // Add/remove shapes
    void addShape(std::shared_ptr<scene_graph::Shape> shape);
    void removeShape(std::shared_ptr<scene_graph::Shape> shape);

    // Canvas methods
    void render();
    void clear();
    void renderNode(std::shared_ptr<scene_graph::Node> node);
    std::shared_ptr<scene_graph::Node> hitTest(const scene_graph::Vector2& position) const;
    void selectNode(const std::shared_ptr<scene_graph::Node>& node);
    std::shared_ptr<scene_graph::Node> getSelectedNode() const;

    private:
        std::shared_ptr<Renderer> renderer_;
        std::shared_ptr<scene_graph::Node> root_;
        std::shared_ptr<scene_graph::Node> selectedNode_;
        std::vector<std::shared_ptr<scene_graph::Shape>> shapes_;

};

} // namespace visualization

