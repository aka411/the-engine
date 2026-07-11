#pragma once
#include <vector>

#include <volk.h>
#include <vk_mem_alloc.h>// seems like vk_mem_alloc.h has to come after volk.h

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_bindless_texture_set_manager.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/engine_handles.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanContext;
	class VulkanSamplerManager;


	struct VulkanTexture
	{
		TextureMetadata metaData;

		VkImage vkImage;
		VkImageView vkImageView;
		VmaAllocation vmaAllocation;

	};


	

	class VulkanTextureStore
	{


	private:

		VulkanContext& m_vulkanContext;
		VulkanBindlessTextureSetManager m_vulkanBindlessTextureSetManager;

		VulkanSamplerManager& m_vulkanSamplerManager;
		std::vector<VulkanTexture> m_vulkanTextures;



		TextureHandle storeVulkanTexture(VulkanTexture& vulkanTexture, std::vector<TextureMetadata>& textureRegistry);

	public:

		VulkanTextureStore(VulkanContext& vulkanContext, VulkanSamplerManager& vulkanSamplerManager);

		TextureHandle createNewTexture(TextureCreateInfo& textureCreateInfo, std::vector<TextureMetadata>& textureRegistry);
		void destroyTexture(const TextureHandle& textureHandle, std::vector<TextureMetadata>& textureRegistry);

		
		VulkanTexture getVulkanTexture(const TextureHandle textureHandle);
		VkDescriptorSet getBindlessImageVkDescriptorSet();

	};






}