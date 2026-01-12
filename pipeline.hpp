#pragma once

#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace cfd {

namespace render {

class Device;

class Pipeline {
 public:
  Pipeline(Device& device, const std::filesystem::path& vertex_shader_path,
           const std::filesystem::path& fragment_shader_path);
  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

 private:
  void create_pipeline(const std::filesystem::path& vertex_shader_path,
                       const std::filesystem::path& fragment_shader_path);

  void create_shader_module(const std::vector<char>& code, VkShaderModule* shader_module) const;

  Device& device_;
  VkPipeline pipeline_;
  VkShaderModule vert_;
  VkShaderModule frag_;
};

}  // namespace render

}  // namespace cfd
