#include "window.hpp"

#include <cassert>
#include <stdexcept>

namespace cfd {

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title) {
  init();
}

Window::~Window() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
  }

  glfwTerminate();
}

bool Window::should_close() const {
  assert(window_ != nullptr);
  return (glfwWindowShouldClose(window_) != 0);
}

void Window::poll_events() const { glfwPollEvents(); }

void Window::create_surface(VkInstance instance, VkSurfaceKHR* surface) const {
  if (glfwCreateWindowSurface(instance, window_, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create GLFW window surface");
  }
}

void Window::init() {
  if (glfwInit() == GLFW_FALSE) {
    throw std::runtime_error("Failed to initialize the GLFW library");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  if (window_ == nullptr) {
    throw std::runtime_error("Failed to create GLFW window");
  }
}

}  // namespace cfd
