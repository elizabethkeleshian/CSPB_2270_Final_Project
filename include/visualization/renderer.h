#ifndef VISUALIZATION_RENDERER_H
#define VISUALIZATION_RENDERER_H

#include "constants.h"
#include "font_manager.h"
#include "render_types.h"
#include "shader_manager.h"
#include "shape_renderer.h"
#include "text_renderer.h"
#include <memory>
#include <scene_graph/shape.h>
namespace visualization {

class Renderer {
public:
  Renderer();
  ~Renderer();

  // Delete copy and move operations
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&) = delete;
  Renderer &operator=(Renderer &&) = delete;

  // Initialization and cleanup
  bool initialize();
  void cleanup();

  // Headless mode for testing
  void setHeadlessMode(bool headless);
  bool isHeadlessMode() const;

  // Frame management
  void beginFrame();
  void endFrame();
  void setViewport(int width, int height);

  // Shape rendering (delegated to ShapeRenderer)
  void renderShape(const scene_graph::Shape &shape);
  void drawRectangle(float x, float y, float width, float height,
                     const Vector4 &color);
  void drawLine(float x1, float y1, float x2, float y2, const Vector4 &color,
                float thickness = 0.02f);

  // Text rendering (delegated to TextRenderer)
  void drawText(const std::string &text, float x, float y,
                const Vector4 &color);

  // Accessors for specialized renderers (for advanced usage)
  std::shared_ptr<ShapeRenderer> getShapeRenderer() const;
  std::shared_ptr<TextRenderer> getTextRenderer() const;

private:
  // Mode
  RenderMode mode_ = RenderMode::Normal;

  // Component managers and renderers
  std::shared_ptr<ShaderManager> shaderManager_;
  std::shared_ptr<FontManager> fontManager_;
  std::shared_ptr<TextRenderer> textRenderer_;
  std::shared_ptr<ShapeRenderer> shapeRenderer_;

  // Viewport dimensions
  int viewportWidth_ = constants::DEFAULT_WINDOW_WIDTH;
  int viewportHeight_ = constants::DEFAULT_WINDOW_HEIGHT;
};

} // namespace visualization

#endif // VISUALIZATION_RENDERER_H