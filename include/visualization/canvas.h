#pragma once

#include <memory>
#include <vector>

#include "scene_graph/node.h"
#include "scene_graph/shape.h"
#include "types.h"
#include "visualization/renderer.h"

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

    // delete move constructor and assignment operator
    Canvas(Canvas&&) = delete;
    Canvas& operator=(Canvas&&) = delete;

    // delete copy constructor and assignment operator
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    // Initialize the canvas with a renderer
    bool initialize(const std::shared_ptr<Renderer>& renderer);

    // Set the root node of the canvas
    void setRoot(const std::shared_ptr<scene_graph::Node>& root);
    std::shared_ptr<scene_graph::Node> getRoot() const;

    // Add/remove shapes
    void addShape(const std::shared_ptr<scene_graph::Shape>& shape);
    void removeShape(const std::shared_ptr<scene_graph::Shape>& shape);

    // Canvas methods
    void render();
    void clear();
    void renderNode(const std::shared_ptr<scene_graph::Node>& node);
    [[nodiscard]] std::shared_ptr<scene_graph::Node> hitTest(const Vector2& position) const;
    void selectNode(const std::shared_ptr<scene_graph::Node>& node);
    [[nodiscard]] std::shared_ptr<scene_graph::Node> getSelectedNode() const;
    [[nodiscard]] std::shared_ptr<scene_graph::Node> hitTestRecursive(
        const std::shared_ptr<scene_graph::Node>& node, const Vector2& position) const;

private:
    std::shared_ptr<Renderer> renderer_;
    std::shared_ptr<scene_graph::Node> root_;
    std::shared_ptr<scene_graph::Node> selectedNode_;
    std::vector<std::shared_ptr<scene_graph::Shape>> shapes_;
};

}  // namespace visualization
