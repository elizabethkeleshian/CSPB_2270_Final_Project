#include "visualization/window.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace visualization {

Window::Window()
    : windowHandle_(nullptr), width_(WINDOW_WIDTH), height_(WINDOW_HEIGHT) {}

Window::~Window() { close(); }

bool Window::create(int width, int height, const std::string &title) {
  if (width <= 0 || height <= 0) {
    return false;
  }

  // If GLFW is not initialized, initialize it
  static bool glfwInitialized = false;
  if (!glfwInitialized) {
    if (glfwInit() == GLFW_FALSE) {
      std::cerr << "Failed to initialize GLFW \n";
      return false;
    }
    glfwInitialized = true;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  windowHandle_ =
      glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (windowHandle_ == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    return false;
  }

  glfwMakeContextCurrent(static_cast<GLFWwindow *>(windowHandle_));
  width_ = width;
  height_ = height;

  // mouse callback TODO: create separate function for this
  glfwSetWindowUserPointer(static_cast<GLFWwindow *>(windowHandle_), this);
  glfwSetCursorPosCallback(static_cast<GLFWwindow *>(windowHandle_),
                           [](GLFWwindow *window, double x, double y) {
                             auto *thisWindow = static_cast<Window *>(
                                 glfwGetWindowUserPointer(window));
                             if (thisWindow && thisWindow->mouseCallback_) {
                               thisWindow->mouseCallback_(x, y);
                             }
                           });

  // key callback TODO: create separate function for this
  glfwSetKeyCallback(
      static_cast<GLFWwindow *>(windowHandle_),
      [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *thisWindow =
            static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (thisWindow && thisWindow->keyCallback_) {
          thisWindow->keyCallback_(key, scancode, action, mods);
        }
      });

  return true;
}

void Window::close() {
  if (windowHandle_) {
    glfwDestroyWindow(static_cast<GLFWwindow *>(windowHandle_));
    windowHandle_ = nullptr;
  }
}

bool Window::shouldClose() const {
  if (windowHandle_ == nullptr) {
    return true; // Window already closed
  }
  return glfwWindowShouldClose(static_cast<GLFWwindow *>(windowHandle_)) !=
         GLFW_FALSE;
}

void Window::swapBuffers() {
  if (windowHandle_ != nullptr) {
    glfwSwapBuffers(static_cast<GLFWwindow *>(windowHandle_));
  }
}

void Window::pollEvents() { glfwPollEvents(); }

void Window::setMouseCallback(MouseCallback callback) {
  mouseCallback_ = std::move(callback);
}

void Window::setKeyCallback(KeyCallback callback) {
  keyCallback_ = std::move(callback);
}

int Window::getWidth() const { return width_; }

int Window::getHeight() const { return height_; }

} // namespace visualization