#include "../../include/visualization/window.h"
#include <GLFW/glfw3.h>

namespace visualization {
    using namespace std;

    Window::Window(int width, int height, const string& title) : width_(width), height_(height), title_(title) {};

    bool Window::create(int width, int height, const string& title) {
        width_ = width;
        height_ = height;
        title_ = title;
        return true;
    }

    void Window::close() {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window_);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(window_);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::setMouseCallback(MouseCallback callback) {
        mouseCallback_ = callback;
    }

    void Window::setKeyCallback(KeyCallback callback) {
        keyCallback_ = callback;
    }

    int Window::getWidth() const {
        return width_;
    }

    int Window::getHeight() const {
        return height_;
    }
    
} // namespace visualization