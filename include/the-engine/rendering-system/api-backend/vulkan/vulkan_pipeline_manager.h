#pragma once
#include <map>
#include <vector>
#include <string>

#include <volk.h>

#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/engine_handles.h>
#include <utils/data-structures/my_in_place_vector.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanContext;
	class VulkanDescriptorSetManager;
	class VulkanShaderManager;



	class VulkanPipelineObject
	{
		public:

		VkPipeline m_vkPipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_vkPipelineLayout = VK_NULL_HANDLE;

		//std::map<int, VkDescriptorSetLayout> m_vkDescriptorSetLayouts;

		//std::array<VkDescriptorSetLayout, 4> m_vkDescriptorSetLayouts = { VK_NULL_HANDLE ,VK_NULL_HANDLE ,VK_NULL_HANDLE ,VK_NULL_HANDLE };
		MyInPlaceVector<VkDescriptorSetLayout, 4> m_vkDescriptorSetLayouts;


		VkShaderStageFlags m_pushConstantStages;
		static constexpr uint32_t MAX_PUSH_CONSTANT_SIZE = 128;



	};






	class VulkanPipelineManager : public IPipelineManager
	{

	private:

		VulkanContext& m_vulkanContext;

		VulkanDescriptorSetManager& m_vulkanDescriptorSetManager;
		VulkanShaderManager& m_vulkanShaderManager;
 
		

		std::vector<VulkanPipelineObject> m_vulkanPipelineObjects;
		std::map<std::string, PipelineHandle> m_stringNameToPipelineHandle;



		VkPipeline createVkPipeline(VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo);

		//VkGraphicsPipelineCreateInfo getVkGraphicsPipelineCreateInfo(const PipelineStateCreateInfo& pipelineStateCreateInfo);

		PipelineHandle storeVulkanPipelineObject(VkPipeline vkPipeline, VkPipelineLayout vkPipelineLayout, std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts);

	public:

		VulkanPipelineManager(VulkanContext& vulkanContext, VulkanDescriptorSetManager& vulkanDescriptorSetManager, VulkanShaderManager& vulkanShaderManager);

		virtual ~VulkanPipelineManager() override;


		virtual const PipelineHandle createPipeline(const std::string& name, const PipelineStateCreateInfo& pipelineStateCreateInfo) override;
		virtual void destroyPipeline(const PipelineHandle& pipelineHandle) override;

		//GETTERS

		virtual const PipelineHandle getPipelineHandle(const std::string& pipelineName) const override;
		

		//Vulkan Specific Methods

		VkPipelineLayout createVkPipelineLayout(VkPipelineLayoutCreateInfo& VkPipelineLayoutCreateInfo);
		const PipelineHandle createVulkanPipelineObject(const std::string& pipelineName, const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo, const VkPipelineLayout vkPipelineLayout, const std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts);
		//const PipelineHandle storeVulkanPipelineObject(const std::string& pipelineName, VkPipeline vkPipeline);

		VulkanPipelineObject getVulkanPipelineObject(const std::string& pipelineName) const;
		VulkanPipelineObject getVulkanPipelineObject(const PipelineHandle& pipelineHandle) const;
	};




}