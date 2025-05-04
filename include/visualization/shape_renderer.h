// visualization/shape_renderer.h
#ifndef VISUALIZATION_SHAPE_RENDERER_H
#define VISUALIZATION_SHAPE_RENDERER_H

#include <scene_graph/circle.h>
#include <scene_graph/rectangle.h>
#include <scene_graph/shape.h>

#include <memory>

#include "render_types.h"
#include "shader_manager.h"

namespace visualization {

class ShapeRenderer {
public:
    ShapeRenderer(std::shared_ptr<ShaderManager> shaderManager);
    ~ShapeRenderer();

    // Delete copy and move operations
    ShapeRenderer(const ShapeRenderer&) = delete;
    ShapeRenderer& operator=(const ShapeRenderer&) = delete;
    ShapeRenderer(ShapeRenderer&&) = delete;
    ShapeRenderer& operator=(ShapeRenderer&&) = delete;

    // Initialization
    bool initialize(RenderMode mode = RenderMode::Normal);
    void cleanup();

    // Shape rendering
    void renderShape(const scene_graph::Shape& shape);

    // Basic primitive rendering
    void drawRectangle(float x, float y, float width, float height, const Vector4& color);
    void drawLine(float x1, float y1, float x2, float y2, const Vector4& color,
                  float thickness = 0.02f);

    // Information
    bool isInitialized() const;
    void setViewport(int width, int height);

private:
    // Shader manager reference
    std::shared_ptr<ShaderManager> shaderManager_;

    // Implementation details
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace visualization

#endif  // VISUALIZATION_SHAPE_RENDERER_H