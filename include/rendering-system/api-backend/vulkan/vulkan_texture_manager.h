#pragma once
#include <rendering-system/rhi/i_texture_manager.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/api-backend/vulkan/vulkan_texture_system_data_types.h>



namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanTransferManager;
	class VulkanSamplerManager;
	class VulkanContext;


	class VulkanTextureManager : public ITextureManager
	{

	private:

		VulkanContext& m_vulkanContext;

		VkDescriptorPool m_bindlessImageVkDescriptorPool;
		VkDescriptorSet m_bindlessImageVkDescriptorSet;


	private:

		VulkanSamplerManager& m_vulkanSamplerManager;
		VulkanTransferManager* m_vulkanTransferManager{ nullptr };

		std::vector<VulkanTexture> m_vulkanTextures;

	protected:

		friend class VulkanRenderDevice;
		void setVulkanTransferManager(VulkanTransferManager& vulkanTransferManager);

	private:

		void updateBindlessSlot(const uint64_t slotIndex, VkSampler sampler, VkImageView imageView);
		size_t storeVulkanTexture(const VulkanTexture vulkanTexture);

	public:



		VulkanTextureManager(VulkanContext& vulkanContext,VulkanSamplerManager& vulkanSamplerManager);

		virtual ~VulkanTextureManager() override;


		virtual TextureHandle createTexture(const TextureCreateInfo& textureCreateInfo) override;
		virtual TextureHandle createTexture(const TextureCreateInfo& textureCreateInfo, TheEngine::Memory::MemoryBlock&& initialData) override;

		virtual void destroyTexture(const TextureHandle& textureHandle) override;


		/*NATIVE METHODS*/
		VkDescriptorSet getBindlessImageVkDescriptorSet();
		VulkanTexture getVulkanTexture(const TextureHandle& textureHandle);
	};



}