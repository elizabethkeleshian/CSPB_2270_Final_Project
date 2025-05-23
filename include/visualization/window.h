#ifndef VISUALIZATION_WINDOW_H
#define VISUALIZATION_WINDOW_H

#include <functional>
#include <string>

namespace visualization {

class Window {
public:
    using MouseCallback = std::function<void(double, double)>;
    using KeyCallback = std::function<void(int, int, int, int)>;
    using MouseButtonCallback = std::function<void(int, int, int)>;
    using ScrollCallback = std::function<void(double, double)>;

    Window();
    ~Window();

    // delete move constructor and assignment operator
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    // delete copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Window management
    bool create(int width, int height, const std::string& title);
    void close();
    [[nodiscard]] bool shouldClose() const;
    void swapBuffers();
    static void pollEvents();

    // Input callbacks
    void setMouseCallback(MouseCallback callback);
    void setKeyCallback(KeyCallback callback);
    void setMouseButtonCallback(MouseButtonCallback callback);
    void setScrollCallback(ScrollCallback callback);

    // Getters
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    void* getWindowHandle() const {
        return windowHandle_;
    }

private:
    void* windowHandle_;
    int width_;
    int height_;
    MouseCallback mouseCallback_;
    KeyCallback keyCallback_;
    MouseButtonCallback mouseButtonCallback_;
    ScrollCallback scrollCallback_;
};

}  // namespace visualization

#endif  // VISUALIZATION_WINDOW_H