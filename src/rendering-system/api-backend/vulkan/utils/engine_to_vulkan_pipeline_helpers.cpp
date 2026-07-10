#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_pipeline_helpers.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_manager.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{







	void getVkVertexlayout(const VertexLayout& vertexLayout,
		std::vector<VkVertexInputBindingDescription>& vertexBindings,
		std::vector<VkVertexInputAttributeDescription>& vertexAttributes)
	{


	
		std::vector<VkVertexInputBindingDescription>& outBindings = vertexBindings;
		std::vector<VkVertexInputAttributeDescription>& outAttributes = vertexAttributes;

		//Map Bindings
		for (const auto& binding : vertexLayout.getBindings())
		{
			VkVertexInputBindingDescription bindingDesc{};
			bindingDesc.binding = binding.binding;
			bindingDesc.stride = binding.stride;
			bindingDesc.inputRate = binding.isInstanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;

			outBindings.push_back(bindingDesc);
		}

		//Map Attributes
		for (const auto& attr : vertexLayout.getAttributes())
		{
			VkVertexInputAttributeDescription attrDesc{};
			attrDesc.binding = attr.binding;
			attrDesc.location = attr.location;
			attrDesc.offset = attr.offset;


			attrDesc.format = resourceFormatToVkFormat(attr.format);

			outAttributes.push_back(attrDesc);
		}




	}






	std::vector<VkDescriptorSetLayout> getVkDescriptorSetLayouts(std::vector<DescriptorSetLayout>& descriptorSetLayouts, VulkanDescriptorSetManager& vulkanDescriptorSetManager)
	{
		std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
		vkDescriptorSetLayouts.reserve(descriptorSetLayouts.size());
		for (auto& descriptorSetLayout : descriptorSetLayouts)
		{
			vkDescriptorSetLayouts.push_back(vulkanDescriptorSetManager.getOrCreateVkDescriptorSetLayout(descriptorSetLayout));

		}
		return vkDescriptorSetLayouts;
	}







	VkPipelineRasterizationStateCreateInfo getVkPipelineRasterizationStateCreateInfo(const RasterizerState& rasterizerState)
	{
		const RasterizerState& state = rasterizerState;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = fillModeToVkPolygonMode(state.fillMode);
		rasterizer.lineWidth = state.lineWidth;
		rasterizer.cullMode = cullModeToVkCullModeFlags(state.cullMode);
		rasterizer.frontFace = frontFaceToVkFrontFace(state.frontFace);

		// Depth Bias

		rasterizer.depthBiasEnable = getVkBool( state.depthBiasEnable);
		rasterizer.depthBiasConstantFactor = state.depthBiasConstant;
		rasterizer.depthBiasSlopeFactor = state.depthBiasSlope;
		rasterizer.depthBiasClamp = state.depthBiasClamp;

		return rasterizer;
	}

	VkPipelineDepthStencilStateCreateInfo getVkPipelineDepthStencilStateCreateInfo(const DepthStencilState& depthStencilState)
	{
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

		// Depth
		depthStencil.depthTestEnable = depthStencilState.depthTestEnable ? VK_TRUE : VK_FALSE;
		depthStencil.depthWriteEnable = depthStencilState.depthWriteEnable ? VK_TRUE : VK_FALSE;
		depthStencil.depthCompareOp = compareOpToVkCompareOp(depthStencilState.depthCompareOp);

		// Bounds Test (Engine abstraction doesn't currently expose this, defaulting to false)
		depthStencil.depthBoundsTestEnable = VK_FALSE;

		// Stencil
		depthStencil.stencilTestEnable = depthStencilState.stencilTestEnable ? VK_TRUE : VK_FALSE;
		depthStencil.front = stencilFaceStateToVkStencilOpState(depthStencilState.stencilFront);
		depthStencil.back = stencilFaceStateToVkStencilOpState(depthStencilState.stencilBack);

		return depthStencil;
	}


	



	VkPipelineMultisampleStateCreateInfo getVkPipelineMultisampleStateCreateInfo(const MultisampleState& multisampleState)
	{
		
		VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo{};
		vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

		vkPipelineMultisampleStateCreateInfo.alphaToCoverageEnable = getVkBool(multisampleState.alphaToCoverageEnable);
		vkPipelineMultisampleStateCreateInfo.alphaToOneEnable = getVkBool(multisampleState.alphaToOneEnable);
		vkPipelineMultisampleStateCreateInfo.minSampleShading = multisampleState.minSampleShading;
		vkPipelineMultisampleStateCreateInfo.rasterizationSamples = getVkSampleCount(multisampleState.rasterizationSamples);
		vkPipelineMultisampleStateCreateInfo.sampleShadingEnable = getVkBool(multisampleState.sampleShadingEnable);
	

		return vkPipelineMultisampleStateCreateInfo;

	}





}