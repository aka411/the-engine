#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_resource_resolver.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_swapchain_manager.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{



	VulkanResourceResolver::VulkanResourceResolver(
		VulkanSwapchainManager& vulkanSwapchainManager,
		VulkanTextureStore& vulkanTextureStore,
		VulkanBufferManager& vulkanBufferManager,
		VulkanPipelineManager& vulkanPipelineManager) :

		m_vulkanSwapchainManager(vulkanSwapchainManager),
		m_vulkanTextureStore(vulkanTextureStore),
		m_vulkanBufferManager(vulkanBufferManager),
		m_vulkanPipelineManager(vulkanPipelineManager)

	{


	}



	const VulkanTexture VulkanResourceResolver::getVulkanTexture(const TextureHandle& textureHandle)
	{

		if (textureHandle == SWAP_CHAIN_IMAGE_TEXTURE_HANDLE)
		{
			//I need to come up with a better way
			VulkanImageViewCombined vulkanImageViewCombined = m_vulkanSwapchainManager.getVulkanImageViewCombinedSwapchain();
			return VulkanTexture{ .vkImage = vulkanImageViewCombined.image, .vkImageView = vulkanImageViewCombined.imageView};
		}

		return m_vulkanTextureStore.getVulkanTexture(textureHandle);
	}

	const VulkanBufferInfo VulkanResourceResolver::getVulkanBufferInfo(const BufferHandle& bufferHandle)
	{
		return m_vulkanBufferManager.getVulkanBufferInfo(bufferHandle);
	}

	const VulkanPipelineObject VulkanResourceResolver::getVulkanPipelineObject(const PipelineHandle& pipelineHandle)
	{
		return m_vulkanPipelineManager.getVulkanPipelineObject(pipelineHandle);
	}


	VkDescriptorSet VulkanResourceResolver::getBindlessImageVkDescriptorSet()
	{
		return m_vulkanTextureStore.getBindlessImageVkDescriptorSet();
	}

}