#include "visualization/window.h"
#include <gtest/gtest.h>

namespace visualization {

class WindowTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = std::make_unique<Window>();
    }

    void TearDown() override {
        window->close();
    }

    std::unique_ptr<Window> window;
};

TEST_F(WindowTest, Create_Success) {
    EXPECT_TRUE(window->create(800, 600, "Test Window"));
}

TEST_F(WindowTest, Create_InvalidDimensions) {
    EXPECT_FALSE(window->create(0, 0, "Invalid Window"));
}

TEST_F(WindowTest, GetDimensions) {
    window->create(800, 600, "Test Window");
    EXPECT_EQ(window->getWidth(), 800);
    EXPECT_EQ(window->getHeight(), 600);
}

TEST_F(WindowTest, MouseCallback) {
    bool callbackCalled = false;
    window->setMouseCallback([&](double x, double y) {
        callbackCalled = true;
    });
    // TODO: Simulate mouse event and verify callback
}

TEST_F(WindowTest, KeyCallback) {
    bool callbackCalled = false;
    window->setKeyCallback([&](int key, int action) {
        callbackCalled = true;
    });
    // TODO: Simulate key event and verify callback
}

TEST_F(WindowTest, ShouldClose_InitiallyFalse) {
    window->create(800, 600, "Test Window");
    EXPECT_FALSE(window->shouldClose());
}

} // namespace visualization 