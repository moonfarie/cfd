#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace cfd {

class Window {
 public:
  Window(int width, int height, const std::string& title);
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  bool should_close() const;
  void poll_events() const;

 private:
  void init();

  int width_{0};
  int height_{0};
  std::string title_{""};
  GLFWwindow* window_{nullptr};
};

}  // namespace cfd
