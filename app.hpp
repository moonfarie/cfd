#pragma once

#include "device.hpp"
#include "pipeline.hpp"
#include "window.hpp"

namespace cfd {

class App {
 public:
  App(int width, int height, const std::string& title);
  ~App() = default;

  void run() const;

 private:
  Window window_;
  render::Device device_{window_};
  render::Pipeline pipeline{device_, "shaders/shader.vert.spv", "shaders/shader.frag.spv"};
};

}  // namespace cfd
