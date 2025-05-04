#include "visualization/window.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include "constants.h"

namespace visualization {

Window::Window()
    : windowHandle_(nullptr),
      width_(constants::DEFAULT_WINDOW_WIDTH),
      height_(constants::DEFAULT_WINDOW_HEIGHT) {
}

Window::~Window() {
    close();
}

bool Window::create(int width, int height, const std::string& title) {
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

    windowHandle_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (windowHandle_ == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }

    glfwMakeContextCurrent(static_cast<GLFWwindow*>(windowHandle_));
    width_ = width;
    height_ = height;

    // mouse callback TODO: create separate function for this
    glfwSetWindowUserPointer(static_cast<GLFWwindow*>(windowHandle_), this);
    glfwSetCursorPosCallback(
        static_cast<GLFWwindow*>(windowHandle_), [](GLFWwindow* window, double x, double y) {
            auto* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (thisWindow && thisWindow->mouseCallback_) {
                thisWindow->mouseCallback_(x, y);
            }
        });

    // key callback TODO: create separate function for this
    glfwSetKeyCallback(static_cast<GLFWwindow*>(windowHandle_),
                       [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                           auto* thisWindow =
                               static_cast<Window*>(glfwGetWindowUserPointer(window));
                           if (thisWindow && thisWindow->keyCallback_) {
                               thisWindow->keyCallback_(key, scancode, action, mods);
                           }
                       });

    // mouse button callback TODO: create separate function for this
    glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(windowHandle_),
                               [](GLFWwindow* window, int button, int action, int mods) {
                                   auto* thisWindow =
                                       static_cast<Window*>(glfwGetWindowUserPointer(window));
                                   if (thisWindow && thisWindow->mouseButtonCallback_) {
                                       thisWindow->mouseButtonCallback_(button, action, mods);
                                   }
                               });

    // scroll callback TODO: create separate function for this
    glfwSetScrollCallback(static_cast<GLFWwindow*>(windowHandle_),
                          [](GLFWwindow* window, double xoffset, double yoffset) {
                              auto* thisWindow =
                                  static_cast<Window*>(glfwGetWindowUserPointer(window));
                              if (thisWindow && thisWindow->scrollCallback_) {
                                  thisWindow->scrollCallback_(xoffset, yoffset);
                              }
                          });

    return true;
}

void Window::close() {
    if (windowHandle_) {
        glfwDestroyWindow(static_cast<GLFWwindow*>(windowHandle_));
        windowHandle_ = nullptr;
    }

    // Only terminate GLFW if no other windows exist
    // (in a real app, might need a ref counter)
    glfwTerminate();
}

bool Window::shouldClose() const {
    if (windowHandle_ == nullptr) {
        return true;  // Window already closed
    }
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(windowHandle_)) != GLFW_FALSE;
}

void Window::swapBuffers() {
    if (windowHandle_ != nullptr) {
        glfwSwapBuffers(static_cast<GLFWwindow*>(windowHandle_));
    }
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::setMouseCallback(MouseCallback callback) {
    mouseCallback_ = std::move(callback);
}

void Window::setKeyCallback(KeyCallback callback) {
    keyCallback_ = std::move(callback);
}

void Window::setMouseButtonCallback(MouseButtonCallback callback) {
    mouseButtonCallback_ = std::move(callback);
}

void Window::setScrollCallback(ScrollCallback callback) {
    scrollCallback_ = std::move(callback);
}

int Window::getWidth() const {
    return width_;
}

int Window::getHeight() const {
    return height_;
}

}  // namespace visualization