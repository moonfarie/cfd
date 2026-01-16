#include "pipeline.hpp"

#include <stdexcept>

#include "device.hpp"
#include "utilities.hpp"

namespace engine {

namespace render {

Pipeline::Pipeline(Device& device, const VkPipelineStateCreateInfo& pipeline_cfg,
                   const std::filesystem::path& vertex_shader_path,
                   const std::filesystem::path& fragment_shader_path)
    : device_(device) {
  create_pipeline(pipeline_cfg, vertex_shader_path, fragment_shader_path);
}

Pipeline::~Pipeline() {
  vkDestroyShaderModule(device_.handle(), vert_, nullptr);
  vkDestroyShaderModule(device_.handle(), frag_, nullptr);
  vkDestroyPipeline(device_.handle(), pipeline_, nullptr);
}

void Pipeline::create_pipeline(const VkPipelineStateCreateInfo& pipeline_cfg,
                               const std::filesystem::path& vertex_shader_path,
                               const std::filesystem::path& fragment_shader_path) {
  const auto vertex_shader_code = utilities::read_file_as_char_vec(vertex_shader_path);
  const auto fragment_shader_code = utilities::read_file_as_char_vec(fragment_shader_path);

  create_shader_module(vertex_shader_code, &vert_);
  create_shader_module(fragment_shader_code, &frag_);

  // Shader
  VkPipelineShaderStageCreateInfo shader_stage_create_info[2];

  shader_stage_create_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shader_stage_create_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shader_stage_create_info[0].module = vert_;
  shader_stage_create_info[0].pName = "main";
  shader_stage_create_info[0].flags = 0;
  shader_stage_create_info[0].pNext = nullptr;
  shader_stage_create_info[0].pSpecializationInfo = nullptr;

  shader_stage_create_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shader_stage_create_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  shader_stage_create_info[1].module = frag_;
  shader_stage_create_info[1].pName = "main";
  shader_stage_create_info[1].flags = 0;
  shader_stage_create_info[1].pNext = nullptr;
  shader_stage_create_info[1].pSpecializationInfo = nullptr;

  // Vertex Input
  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};

  vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
  vertex_input_state_create_info.vertexBindingDescriptionCount = 0;
  vertex_input_state_create_info.pVertexAttributeDescriptions = nullptr;
  vertex_input_state_create_info.pVertexBindingDescriptions = nullptr;

  // Viewport
  VkPipelineViewportStateCreateInfo viewport_state_create_info{};

  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  viewport_state_create_info.pViewports = &pipeline_cfg.viewport;
  viewport_state_create_info.scissorCount = 1;
  viewport_state_create_info.pScissors = &pipeline_cfg.scissor;

  // Color Blend
  VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};

  color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend_state_create_info.logicOpEnable = VK_FALSE;
  color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
  color_blend_state_create_info.attachmentCount = 1;
  color_blend_state_create_info.pAttachments = &pipeline_cfg.color_blend_attachment;
  color_blend_state_create_info.blendConstants[0] = 0.0F;
  color_blend_state_create_info.blendConstants[1] = 0.0F;
  color_blend_state_create_info.blendConstants[2] = 0.0F;
  color_blend_state_create_info.blendConstants[3] = 0.0F;

  // Graphics Pipeline
  VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};

  graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  graphics_pipeline_create_info.stageCount = 2;
  graphics_pipeline_create_info.pStages = shader_stage_create_info;
  graphics_pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
  graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
  graphics_pipeline_create_info.pInputAssemblyState = &pipeline_cfg.input_assembly;
  graphics_pipeline_create_info.pRasterizationState = &pipeline_cfg.rasterization;
  graphics_pipeline_create_info.pMultisampleState = &pipeline_cfg.multisample;
  graphics_pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
  graphics_pipeline_create_info.pDepthStencilState = &pipeline_cfg.depth_stencil;
  graphics_pipeline_create_info.pDynamicState = nullptr;
  graphics_pipeline_create_info.layout = pipeline_cfg.layout;
  graphics_pipeline_create_info.renderPass = pipeline_cfg.render_pass;
  graphics_pipeline_create_info.subpass = pipeline_cfg.subpass;
  graphics_pipeline_create_info.basePipelineIndex = -1;
  graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(device_.handle(), VK_NULL_HANDLE, 1, &graphics_pipeline_create_info,
                                nullptr, &pipeline_) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan graphics pipeline");
  }
}

Pipeline::VkPipelineStateCreateInfo Pipeline::create_default_config(uint32_t width,
                                                                    uint32_t height) {
  VkPipelineStateCreateInfo pipeline_state_create_info{};

  pipeline_state_create_info.viewport.x = 0.0F;
  pipeline_state_create_info.viewport.y = 0.0F;
  pipeline_state_create_info.viewport.width = static_cast<float>(width);
  pipeline_state_create_info.viewport.height = static_cast<float>(height);
  pipeline_state_create_info.viewport.minDepth = 0.0F;
  pipeline_state_create_info.viewport.maxDepth = 1.0F;

  pipeline_state_create_info.scissor.offset = {0, 0};
  pipeline_state_create_info.scissor.extent = {width, height};

  pipeline_state_create_info.input_assembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  pipeline_state_create_info.input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pipeline_state_create_info.input_assembly.primitiveRestartEnable = VK_FALSE;

  pipeline_state_create_info.rasterization.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  pipeline_state_create_info.rasterization.depthClampEnable = VK_FALSE;
  pipeline_state_create_info.rasterization.rasterizerDiscardEnable = VK_FALSE;
  pipeline_state_create_info.rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  pipeline_state_create_info.rasterization.lineWidth = 1.0F;
  pipeline_state_create_info.rasterization.cullMode = VK_CULL_MODE_NONE;
  pipeline_state_create_info.rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  pipeline_state_create_info.rasterization.depthBiasEnable = VK_FALSE;
  pipeline_state_create_info.rasterization.depthBiasConstantFactor = 0.0F;
  pipeline_state_create_info.rasterization.depthBiasClamp = 0.0F;
  pipeline_state_create_info.rasterization.depthBiasSlopeFactor = 0.0F;

  pipeline_state_create_info.multisample.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  pipeline_state_create_info.multisample.sampleShadingEnable = VK_FALSE;
  pipeline_state_create_info.multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  pipeline_state_create_info.multisample.minSampleShading = 1.0F;
  pipeline_state_create_info.multisample.pSampleMask = nullptr;
  pipeline_state_create_info.multisample.alphaToCoverageEnable = VK_FALSE;
  pipeline_state_create_info.multisample.alphaToOneEnable = VK_FALSE;

  pipeline_state_create_info.color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
      VK_COLOR_COMPONENT_A_BIT;
  pipeline_state_create_info.color_blend_attachment.blendEnable = VK_FALSE;
  pipeline_state_create_info.color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  pipeline_state_create_info.color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  pipeline_state_create_info.color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
  pipeline_state_create_info.color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  pipeline_state_create_info.color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  pipeline_state_create_info.color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

  pipeline_state_create_info.depth_stencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  pipeline_state_create_info.depth_stencil.depthTestEnable = VK_TRUE;
  pipeline_state_create_info.depth_stencil.depthWriteEnable = VK_TRUE;
  pipeline_state_create_info.depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
  pipeline_state_create_info.depth_stencil.depthBoundsTestEnable = VK_FALSE;
  pipeline_state_create_info.depth_stencil.minDepthBounds = 0.0F;
  pipeline_state_create_info.depth_stencil.maxDepthBounds = 1.0F;
  pipeline_state_create_info.depth_stencil.stencilTestEnable = VK_FALSE;
  pipeline_state_create_info.depth_stencil.front = {};
  pipeline_state_create_info.depth_stencil.back = {};

  return pipeline_state_create_info;
}

void Pipeline::create_shader_module(const std::vector<char>& code,
                                    VkShaderModule* shader_module) const {
  VkShaderModuleCreateInfo create_info{};

  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(device_.handle(), &create_info, nullptr, shader_module) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module");
  }
}

}  // namespace render

}  // namespace engine
