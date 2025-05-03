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

  // Rendering methods
  void beginFrame();
  void endFrame();
  void renderShape(const scene_graph::Shape &shape);

  // Text rendering
  void drawText(const std::string &text, float x, float y,
                const Vector4 &color);

  // Window management
  void setViewport(int width, int height);

private:
  // Text rendering initialization
  bool initTextRendering();

  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace visualization

#endif // VISUALIZATION_RENDERER_H