#pragma once

#include <memory>
#include <vector>

#include "device.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

namespace engine {

class App {
 public:
  App(int width, int height, const std::string& title);
  ~App();

  App(const App&) = delete;
  App& operator=(const App&) = delete;

  void run() const;

 private:
  void create_pipeline_layout();
  void create_pipeline();
  void create_command_buffers();

  void draw_frame();

  Window window_;
  render::Device device_{window_};
  render::SwapChain swap_chain_{device_, window_.extent()};
  VkPipelineLayout pipeline_layout_;
  std::unique_ptr<render::Pipeline> pipeline_;
  std::vector<VkCommandBuffer> command_buffers_;
};

}  // namespace engine
