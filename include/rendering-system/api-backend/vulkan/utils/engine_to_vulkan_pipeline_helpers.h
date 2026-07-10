#pragma once

#include <vector>
#include <volk.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanDescriptorSetManager;


	

	void getVkVertexlayout(const VertexLayout& vertexLayout,
		std::vector<VkVertexInputBindingDescription>& vertexBindings,
		std::vector<VkVertexInputAttributeDescription>& vertexAttributes);



	std::vector<VkDescriptorSetLayout> getVkDescriptorSetLayouts(std::vector<DescriptorSetLayout>& descriptorSetLayouts, VulkanDescriptorSetManager& vulkanDescriptorSetManager);

	VkPipelineRasterizationStateCreateInfo getVkPipelineRasterizationStateCreateInfo(const RasterizerState& rasterizerState);
	
	VkPipelineDepthStencilStateCreateInfo getVkPipelineDepthStencilStateCreateInfo(const DepthStencilState& depthStencilState);
	
	VkPipelineMultisampleStateCreateInfo getVkPipelineMultisampleStateCreateInfo(const MultisampleState& multisampleState);


}