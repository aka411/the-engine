#pragma once
#include <rendering-system/rhi/i_texture_manager.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_texture_store.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanTransferManager;
	class VulkanTextureStore;




	class VulkanTextureManager : public ITextureManager
	{
	private:

		VulkanTextureStore& m_vulkanTextureStore;
		VulkanTransferManager& m_vulkanTransferManager;

	public:



		VulkanTextureManager(VulkanTextureStore& vulkanTextureStore,VulkanTransferManager& vulkanTransferManager);

		virtual ~VulkanTextureManager() override;

		virtual TextureHandle createNewTexture(TextureCreateInfo& textureCreateInfo) override;

		virtual void destroyTexture(const TextureHandle& textureHandle) override;



		/*NATIVE METHODS*/
		VkDescriptorSet getBindlessImageVkDescriptorSet();
		VulkanTexture getVulkanTexture(const TextureHandle& textureHandle);
	};



}