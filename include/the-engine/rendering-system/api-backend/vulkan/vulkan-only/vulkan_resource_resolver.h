#pragma once
#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_pipeline_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_texture_system_data_types.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	
	class VulkanBufferManager;
	class VulkanPipelineManager;
	class VulkanSwapchainManager;
	class VulkanTextureManager;

	class VulkanResourceResolver
	{
	private:

		VulkanSwapchainManager& m_vulkanSwapchainManager;
		VulkanTextureManager& m_vulkanTextureManager;
		VulkanBufferManager& m_vulkanBufferManager;
		VulkanPipelineManager& m_vulkanPipelineManager;
		


	public:

		VulkanResourceResolver(
			VulkanSwapchainManager& vulkanSwapchainManager, 
			VulkanTextureManager& vulkanTextureManager,
			VulkanBufferManager& m_vulkanBufferManager,
			VulkanPipelineManager& m_vulkanPipelineManager);



		const VulkanTexture getVulkanTexture(const TextureHandle& textureHandle);

		const VulkanBufferInfo getVulkanBufferInfo(const BufferHandle& bufferHandle);

		const VulkanPipelineObject getVulkanPipelineObject(const PipelineHandle& pipelineHandle);

		//Special method
		VkDescriptorSet getBindlessImageVkDescriptorSet();
	};







}