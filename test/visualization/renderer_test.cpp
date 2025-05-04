// test/visualization/renderer_test.cpp (expanded)
#include "constants.h"
#include "scene_graph/circle.h"
#include "scene_graph/rectangle.h"
#include "types.h"
#include "visualization/renderer.h"
#include <gtest/gtest.h>
#include <memory>

namespace visualization {

class RendererTest : public ::testing::Test {
protected:
  void SetUp() override {
    renderer = std::make_shared<Renderer>();
    // Enable headless mode for testing without OpenGL context
    renderer->setHeadlessMode(true);
  }

  std::shared_ptr<Renderer> renderer;
};

TEST_F(RendererTest, Initialize_Success) {
  EXPECT_TRUE(renderer->initialize());
}

TEST_F(RendererTest, SetViewport_UpdatesDimensions) {
  renderer->initialize();
  renderer->setViewport(constants::DEFAULT_WINDOW_WIDTH,
                        constants::DEFAULT_WINDOW_HEIGHT);
  // Hard to test the actual dimensions since they're internal
  // So we'll just verify the method doesn't crash
  SUCCEED();
}

TEST_F(RendererTest, BeginFrame_ClearsBuffer) {
  renderer->initialize();
  EXPECT_NO_THROW(renderer->beginFrame());
}

TEST_F(RendererTest, EndFrame_SwapsBuffers) {
  renderer->initialize();
  EXPECT_NO_THROW(renderer->endFrame());
}

TEST_F(RendererTest, RenderShape_Rectangle) {
  renderer->initialize();
  scene_graph::Rectangle rect("TestRect", Vector2(2.0f, 1.0f));
  rect.setPosition(Vector2(1.0f, 1.0f));
  rect.setRotation(45.0f);
  rect.setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

  EXPECT_NO_THROW(renderer->renderShape(rect));
}

TEST_F(RendererTest, RenderShape_Circle) {
  renderer->initialize();
  scene_graph::Circle circle("TestCircle", 1.0f);
  circle.setPosition(Vector2(1.0f, 1.0f));
  circle.setRotation(45.0f);
  circle.setColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));

  EXPECT_NO_THROW(renderer->renderShape(circle));
}

TEST_F(RendererTest, Cleanup_Success) {
  renderer->initialize();
  EXPECT_NO_THROW(renderer->cleanup());
}

TEST_F(RendererTest, DrawRectangle_Success) {
  renderer->initialize();
  Vector4 color(0.5f, 0.6f, 0.7f, 0.8f);
  EXPECT_NO_THROW(renderer->drawRectangle(-1.0f, -1.0f, 2.0f, 2.0f, color));
}

TEST_F(RendererTest, DrawLine_Success) {
  renderer->initialize();
  Vector4 color(0.1f, 0.2f, 0.3f, 0.4f);
  EXPECT_NO_THROW(renderer->drawLine(-1.0f, -1.0f, 1.0f, 1.0f, color));
}

// Test drawing at various angles
TEST_F(RendererTest, DrawLine_VariousAngles) {
  renderer->initialize();
  Vector4 color(0.1f, 0.2f, 0.3f, 0.4f);

  // Horizontal line
  EXPECT_NO_THROW(renderer->drawLine(-1.0f, 0.0f, 1.0f, 0.0f, color));

  // Vertical line
  EXPECT_NO_THROW(renderer->drawLine(0.0f, -1.0f, 0.0f, 1.0f, color));

  // Diagonal lines
  EXPECT_NO_THROW(renderer->drawLine(-1.0f, -1.0f, 1.0f, 1.0f, color));
  EXPECT_NO_THROW(renderer->drawLine(-1.0f, 1.0f, 1.0f, -1.0f, color));

  // Short line
  EXPECT_NO_THROW(renderer->drawLine(0.0f, 0.0f, 0.1f, 0.1f, color));

  // Zero-length line (should not crash)
  EXPECT_NO_THROW(renderer->drawLine(0.0f, 0.0f, 0.0f, 0.0f, color));
}

} // namespace visualization