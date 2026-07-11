#pragma once
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_texture_store.h>
#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_pipeline_manager.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	
	class VulkanBufferManager;
	class VulkanPipelineManager;
	class VulkanSwapchainManager;


	class VulkanResourceResolver
	{
	private:

		VulkanSwapchainManager& m_vulkanSwapchainManager;
		VulkanTextureStore& m_vulkanTextureStore;
		VulkanBufferManager& m_vulkanBufferManager;
		VulkanPipelineManager& m_vulkanPipelineManager;
		


	public:

		VulkanResourceResolver(VulkanSwapchainManager& vulkanSwapchainManager, VulkanTextureStore& vulkanTextureStore,
			VulkanBufferManager& m_vulkanBufferManager,
			VulkanPipelineManager& m_vulkanPipelineManager);



		const VulkanTexture getVulkanTexture(const TextureHandle& textureHandle);

		const VulkanBufferInfo getVulkanBufferInfo(const BufferHandle& bufferHandle);

		const VulkanPipelineObject getVulkanPipelineObject(const PipelineHandle& pipelineHandle);

		//Special method
		VkDescriptorSet getBindlessImageVkDescriptorSet();
	};







}