#include "app.hpp"

#include <stdexcept>

namespace engine {

App::App(int width, int height, const std::string& title) : window_(width, height, title) {
  create_pipeline_layout();
  create_pipeline();
}

App::~App() { vkDestroyPipelineLayout(device_.handle(), pipeline_layout_, nullptr); }

void App::run() const {
  while (not window_.should_close()) {
    window_.poll_events();
  }
}

void App::create_pipeline_layout() {
  VkPipelineLayoutCreateInfo create_info{};

  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.setLayoutCount = 0;
  create_info.pSetLayouts = nullptr;
  create_info.pushConstantRangeCount = 0;
  create_info.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(device_.handle(), &create_info, nullptr, &pipeline_layout_) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan pipeline layout");
  }
}

void App::create_pipeline() {
  auto pipeline_cfg =
      render::Pipeline::create_default_config(swap_chain_.width(), swap_chain_.height());

  pipeline_cfg.layout = pipeline_layout_;
  pipeline_cfg.render_pass = swap_chain_.render_pass();

  pipeline_ = std::make_unique<render::Pipeline>(device_, pipeline_cfg, "shaders/shader.vert.spv",
                                                 "shaders/shader.frag.spv");
}

void App::create_command_buffers() {
  //
}

void App::draw_frame() {
  //
}

}  // namespace engine
