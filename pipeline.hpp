#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace cfd {

namespace render {

class Device;

struct VkPipelineStateCreateInfo {
  VkViewport viewport;
  VkRect2D scissor;

  VkPipelineInputAssemblyStateCreateInfo input_assembly;
  VkPipelineRasterizationStateCreateInfo rasterization;
  VkPipelineMultisampleStateCreateInfo multisample;
  VkPipelineColorBlendAttachmentState color_blend_attachment;
  VkPipelineDepthStencilStateCreateInfo depth_stencil;

  VkPipelineLayout layout{nullptr};
  VkRenderPass render_pass{nullptr};
  uint32_t subpass{0};
};

class Pipeline {
 public:
  Pipeline(uint32_t width, uint32_t height, Device& device,
           const std::filesystem::path& vertex_shader_path,
           const std::filesystem::path& fragment_shader_path);
  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

 private:
  void create_pipeline(uint32_t width, uint32_t height,
                       const std::filesystem::path& vertex_shader_path,
                       const std::filesystem::path& fragment_shader_path);
  void init_default_config(uint32_t width, uint32_t height);
  void create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module) const;

  Device& device_;
  VkPipeline pipeline_;
  VkShaderModule vert_;
  VkShaderModule frag_;
  VkPipelineStateCreateInfo pipeline_state_create_info_{};
};

}  // namespace render

}  // namespace cfd
