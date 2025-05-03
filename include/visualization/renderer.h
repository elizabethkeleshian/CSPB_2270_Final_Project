#ifndef VISUALIZATION_RENDERER_H
#define VISUALIZATION_RENDERER_H

#include "types.h"
#include <memory>
#include <scene_graph/shape.h>

namespace visualization {
class Renderer {
public:
  Renderer();
  ~Renderer();

  // delete move constructor and assignment operator
  Renderer(Renderer &&) = delete;
  Renderer &operator=(Renderer &&) = delete;

  // delete copy constructor and assignment operator
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;

  // Initialization and cleanup
  bool initialize();
  void cleanup();

  // Headless mode for testing
  void setHeadlessMode(bool headless);
  bool isHeadlessMode() const;

  // Rendering methods
  void beginFrame();
  void endFrame();
  void renderShape(const scene_graph::Shape &shape);

  // Text rendering
  void drawText(const std::string &text, float x, float y,
                const Vector4 &color);

  // Window management
  void setViewport(int width, int height);

  // Drawing methods
  void drawRectangle(float x, float y, float width, float height,
                     const Vector4 &color);
  void drawLine(float x1, float y1, float x2, float y2, const Vector4 &color,
                float thickness = 0.02f);

private:
  // Text rendering initialization
  bool initTextRendering();

  // Create fallback font when system fonts are not available
  void createFallbackFont();

  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace visualization

#endif // VISUALIZATION_RENDERER_H