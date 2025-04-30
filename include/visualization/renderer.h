#ifndef VISUALIZATION_RENDERER_H
#define VISUALIZATION_RENDERER_H

#include <memory>
#include <vector>
#include "../scene_graph/shape.h"

namespace visualization {

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialization and cleanup
    bool initialize();
    void cleanup();

    // Rendering methods
    void beginFrame();
    void endFrame();
    void renderShape(const scene_graph::Shape& shape);

    // Window management
    void setViewport(int width, int height);

private:
    // Private implementation details
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace visualization

#endif // VISUALIZATION_RENDERER_H 