#pragma once
#include <array>
#include <unordered_map>

#include <volk.h>

#include <rendering-system/rhi/data-structures/descriptor_set_layout.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{
	

	class VulkanContext;




	constexpr uint32_t NUMBER_OF_FRAMES_IN_FLIGHT = 2;//Need to move it to a central location like context maybe
	enum class DescriptorPoolType
	{
		STATIC_POOL,
		DYNAMIC_POOL
	};

	class VulkanDescriptorSetManager 
	{

	private:

		VulkanContext& m_vulkanContext;

		//State
		uint32_t m_currentFrameIndex = 0;

		//POOLS
		VkDescriptorPool m_staticVkDescriptorPool;
		std::array<VkDescriptorPool, NUMBER_OF_FRAMES_IN_FLIGHT> m_perFrameVkDescriptorPool;


		std::unordered_map<uint64_t,VkDescriptorSetLayout> m_cachedVkDescriptorSetLayouts;


	private:

		VkDescriptorSetLayout createVkDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout);

		void createVkDescriptorPool(VkDescriptorPoolCreateInfo& vkDescriptorPoolCreateInfo, VkDescriptorPool& vkDescriptorPool);

		void resetDynamicPool(const uint32_t frameIndex);

	public:

		VulkanDescriptorSetManager(VulkanContext& vulkanContext);



		VkDescriptorSetLayout getOrCreateVkDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout);
		VkDescriptorSet createVkDescriptorSet(const VkDescriptorSetLayout& vDescriptorSetLayout, DescriptorPoolType descriptorPoolType);


		//State Management
		void setCurrentFrame(const uint32_t frameIndex);
		
	};




}