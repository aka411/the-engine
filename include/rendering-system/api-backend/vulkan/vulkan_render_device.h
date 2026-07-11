#pragma once
#include <volk.h>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>

#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_texture_store.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanSwapchainManager;
	class VulkanTextureStore;
	class VulkanResourceResolver;

	class VulkanPresentationSystem;
	class VulkanDescriptorSetManager;
	class VulkanCommandBufferManager;
	class VulkanQueueManager;


	class VulkanRenderDevice : public IRenderDevice
	{

	private:



		VkSurfaceKHR m_surface;


		vkb::Instance m_instance;
		vkb::PhysicalDevice m_physicalDevice;
		vkb::Device m_logicalDevice;

		VmaAllocator m_vmaAllocator;

		VulkanContext m_vulkanContext;




	private:

		//VULKAN MANAGERS
		std::unique_ptr <VulkanSwapchainManager> m_vulkanSwapchainManager;


		std::unique_ptr<VulkanDescriptorSetManager> m_vulkanDescriptorSetManager;
		
		//This store was made to solve a dependecy issue with VulkanResourceResolver
		std::unique_ptr<VulkanTextureStore> m_vulkanTextureStore;
		std::unique_ptr<VulkanResourceResolver> m_vulkanResourceResolver;

		std::unique_ptr<VulkanQueueManager> m_vulkanQueueManager;

		//std::unique_ptr<VulkanPresentationSystem> m_vulkanPresentationSystem;

		std::unique_ptr<VulkanCommandBufferManager> m_vulkanCommandBufferManager;


	public:

		VulkanRenderDevice(VkSurfaceKHR surface, vkb::Instance instance);
		virtual ~VulkanRenderDevice() override;


		virtual void beginFrame(const uint32_t frameIndex) override;


		//Vulkan Specific Methods

		const VkDevice& getLogicalDeviceHandle() const { return m_logicalDevice.device; }
		const VkPhysicalDevice& getPhysicalDeviceHandle() const { return m_physicalDevice.physical_device; }
		const VkInstance& getInstance() const { return m_instance.instance; }
		const VkSurfaceKHR& getSurface() const { return m_surface; }
		

		VulkanContext getVulkanContext() { return m_vulkanContext; };
		VmaAllocator& getVMAAllocator() { return m_vmaAllocator; }

	};

}