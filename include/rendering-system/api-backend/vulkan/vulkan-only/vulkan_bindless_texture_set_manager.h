#pragma once
#include <volk.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	//OWNER : VulkanTextureStore

	class VulkanContext;

	class VulkanBindlessTextureSetManager
	{



	private:


		VkDescriptorPool m_bindlessImageVkDescriptorPool;
		VkDescriptorSet m_bindlessImageVkDescriptorSet;

		VulkanContext& m_vulkanContext;


		//track free slots etc


	public:

		VulkanBindlessTextureSetManager(VulkanContext& vulkanContext);

		VkDescriptorSet getBindlessImageVkDescriptorSet();


		void updateBindlessSlot(const uint64_t slotIndex, VkSampler sampler, VkImageView imageView);
		

	};








}