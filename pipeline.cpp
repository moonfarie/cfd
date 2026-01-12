#include "pipeline.hpp"

#include <iostream>
#include <stdexcept>

#include "device.hpp"
#include "utilities.hpp"

namespace cfd {

namespace render {

Pipeline::Pipeline(Device& device, const std::filesystem::path& vertex_shader_path,
                   const std::filesystem::path& fragment_shader_path)
    : device_(device) {
  create_pipeline(vertex_shader_path, fragment_shader_path);
}

Pipeline::~Pipeline() {
  //
}

void Pipeline::create_pipeline(const std::filesystem::path& vertex_shader_path,
                               const std::filesystem::path& fragment_shader_path) {
  const auto vertex_shader_code = utilities::read_file_as_char_vec(vertex_shader_path);
  const auto fragment_shader_code = utilities::read_file_as_char_vec(fragment_shader_path);

  std::cout << "vertex_shader_code size: " << vertex_shader_code.size()
            << "\nfragment_shader_code size: " << fragment_shader_code.size() << '\n';
}

void Pipeline::create_shader_module(const std::vector<char>& code,
                                    VkShaderModule* shader_module) const {
  VkShaderModuleCreateInfo create_info;

  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(device_.handle(), &create_info, nullptr, shader_module) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module");
  }
}

}  // namespace render

}  // namespace cfd
