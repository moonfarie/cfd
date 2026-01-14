#pragma once

#include <cstdint>

#include "device.hpp"
#include "pipeline.hpp"
#include "window.hpp"

namespace cfd {

class App {
 public:
  App(uint32_t width, uint32_t height, const std::string& title);
  ~App() = default;

  void run() const;

 private:
  uint32_t width_{0};
  uint32_t height_{0};

  Window window_;
  render::Device device_{window_};
  render::Pipeline pipeline{width_, height_, device_, "shaders/shader.vert.spv",
                            "shaders/shader.frag.spv"};
};

}  // namespace cfd
