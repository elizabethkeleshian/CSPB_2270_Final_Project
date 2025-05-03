#include "visualization/window.h"
#include <gtest/gtest.h>

namespace visualization {

class WindowTest : public ::testing::Test {
protected:
  void SetUp() override { window = std::make_unique<Window>(); }

  void TearDown() override { window->close(); }

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
  window->setMouseCallback([&](double x, double y) { callbackCalled = true; });
  // TODO: Simulate mouse event and verify callback but how???
}

TEST_F(WindowTest, KeyCallback) {
  bool callbackCalled = false;
  window->setKeyCallback([&](int key, int scancode, int action, int mods) {
    callbackCalled = true;
  });
  // TODO: Simulate key event and verify callback but how???
}

TEST_F(WindowTest, ShouldClose_InitiallyFalse) {
  window->create(800, 600, "Test Window");
  EXPECT_FALSE(window->shouldClose());
}

// Test window destruction
TEST_F(WindowTest, Destruction_ClosesWindow) {
  auto window = std::make_unique<Window>();
  window->create(800, 600, "Test Window");
  // Window should be closed when destroyed
}

// Test poll events
TEST_F(WindowTest, PollEvents_NoExceptions) {
  window->create(800, 600, "Test Window");
  EXPECT_NO_THROW(window->pollEvents());
}

// Test swap buffers
TEST_F(WindowTest, SwapBuffers_NoExceptions) {
  window->create(800, 600, "Test Window");
  EXPECT_NO_THROW(window->swapBuffers());
}

// Test setting should close
TEST_F(WindowTest, ShouldClose_SettingFlag) {
  window->create(800, 600, "Test Window");
  // Need a way to set the shouldClose flag, perhaps add a method to Window
  // window->setShouldClose(true);
  // EXPECT_TRUE(window->shouldClose());
}
} // namespace visualization