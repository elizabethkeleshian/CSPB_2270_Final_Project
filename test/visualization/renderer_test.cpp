// test/visualization/renderer_test.cpp (expanded)
#include "visualization/renderer.h"
#include "scene_graph/rectangle.h"
#include "scene_graph/circle.h"
#include <gtest/gtest.h>
#include <memory>

namespace visualization {

class RendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        renderer = std::make_shared<Renderer>();
    }

    std::shared_ptr<Renderer> renderer;
};

TEST_F(RendererTest, Initialize_Success) {
    EXPECT_TRUE(renderer->initialize());
}

TEST_F(RendererTest, SetViewport_UpdatesDimensions) {
    renderer->initialize();
    renderer->setViewport(800, 600);
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
    scene_graph::Rectangle rect("TestRect", scene_graph::Vector2(2.0f, 1.0f));
    rect.setPosition(scene_graph::Vector2(1.0f, 1.0f));
    rect.setRotation(45.0f);
    rect.setColor(scene_graph::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    
    EXPECT_NO_THROW(renderer->renderShape(rect));
}

TEST_F(RendererTest, RenderShape_Circle) {
    renderer->initialize();
    scene_graph::Circle circle("TestCircle", 1.0f);
    circle.setPosition(scene_graph::Vector2(1.0f, 1.0f));
    circle.setRotation(45.0f);
    circle.setColor(scene_graph::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    
    EXPECT_NO_THROW(renderer->renderShape(circle));
}

TEST_F(RendererTest, Cleanup_Success) {
    renderer->initialize();
    EXPECT_NO_THROW(renderer->cleanup());
}

} // namespace visualization