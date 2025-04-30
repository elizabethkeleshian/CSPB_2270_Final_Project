#include "visualization/renderer.h"
#include <gtest/gtest.h>

namespace visualization {

class RendererTest : public ::testing::Test {
protected:
    void SetUp() override {
        renderer = std::make_unique<Renderer>();
    }

    std::unique_ptr<Renderer> renderer;
};

TEST_F(RendererTest, Initialize_Success) {
    EXPECT_TRUE(renderer->initialize());
}

TEST_F(RendererTest, SetViewport_UpdatesDimensions) {
    renderer->setViewport(800, 600);
    // TODO: Verify viewport dimensions
}

TEST_F(RendererTest, BeginFrame_ClearsBuffer) {
    renderer->beginFrame();
    // TODO: Verify buffer is cleared
}

TEST_F(RendererTest, EndFrame_SwapsBuffers) {
    renderer->endFrame();
    // TODO: Verify buffers are swapped
}

// Note: renderShape() will be tested with actual shapes
// in integration tests

} // namespace visualization 