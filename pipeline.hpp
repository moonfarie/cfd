#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>
#include <filesystem>
#include <vector>

namespace engine {

namespace render {

class Device;

class Pipeline {
 public:
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

  Pipeline(Device& device, const VkPipelineStateCreateInfo& pipeline_cfg,
           const std::filesystem::path& vertex_shader_path,
           const std::filesystem::path& fragment_shader_path);
  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

  static VkPipelineStateCreateInfo create_default_config(uint32_t width, uint32_t height);

 private:
  void create_pipeline(const VkPipelineStateCreateInfo& pipeline_cfg,
                       const std::filesystem::path& vertex_shader_path,
                       const std::filesystem::path& fragment_shader_path);
  void create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module) const;

  Device& device_;
  VkPipeline pipeline_;
  VkShaderModule vert_;
  VkShaderModule frag_;
};

}  // namespace render

}  // namespace engine
