#ifndef VISUALIZATION_WINDOW_H
#define VISUALIZATION_WINDOW_H

#include <functional>
#include <string>

namespace visualization {

class Window {
public:
  using MouseCallback = std::function<void(double, double)>;
  using KeyCallback = std::function<void(int, int)>;

  Window();
  ~Window();

  // delete move constructor and assignment operator
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;

  // delete copy constructor and assignment operator
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  // Window management
  bool create(int width, int height, const std::string &title);
  void close();
  bool shouldClose() const;
  void swapBuffers();
  void pollEvents();

  // Input callbacks
  void setMouseCallback(MouseCallback callback);
  void setKeyCallback(KeyCallback callback);

  // Getters
  int getWidth() const;
  int getHeight() const;

private:
  void *windowHandle_;
  int width_;
  int height_;
  MouseCallback mouseCallback_;
  KeyCallback keyCallback_;
};

} // namespace visualization

#endif // VISUALIZATION_WINDOW_H