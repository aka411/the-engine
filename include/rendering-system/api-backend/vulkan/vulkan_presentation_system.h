#pragma once
#include <rendering-system/engine_handles.h>
#include <volk.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_swapchain_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_texture_system_data_types.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer.h>
#include <memory>
#include <rendering-system/rhi/i_presentation_system.h>


namespace TheEngine::RenderingSystem
{
	class ICommandBuffer;
}


namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanCommandBufferManager;
	class VulkanContext;







	struct Frame 
	{

		//VulkanImageViewCombined swapChainImage;//we get this from swapchain manager

		// Per-frame sync objects
		VkFence inFlightFence;           // CPU waits for GPU to finish this frame, for command buffer
		VkSemaphore imageAvailableSem;   // GPU waits for swapchain image to be ready
		VkSemaphore renderFinishedSem;   // GPU waits for rendering to finish before presenting

		
		std::unique_ptr<VulkanCommandBuffer> vulkanCommandBuffer;
	};

	class VulkanPresentationSystem : public IPresentationSystem
	{

	private:

		static const int NUMBER_OF_FRAMES_IN_FLIGHT = 2;

		VulkanContext& m_vulkanContext;

		VulkanSwapchainManager& m_vulkanSwapchainManager;


		VulkanCommandBufferManager& m_vulkanCommandBufferManager;

		//Needs a state machine
		// N Fence per N commandBuffer 
		//N CommandBuffer per N Frame if so

		//N semaphores per N frame for queue Submit
		//N semaphores for queue submit to present


		Frame m_frames[NUMBER_OF_FRAMES_IN_FLIGHT];

		int m_currentFrame = 0;

		uint32_t m_currentAcquiredImageIndex = 0;

		bool m_isWindowResized = false;

		WindowExtent m_windowExtent;


		void handleWindowResize();

	public:

		VulkanPresentationSystem(VulkanContext& vulkanContext, VulkanSwapchainManager& vulkanSwapchainManager, VulkanCommandBufferManager& vulkanCommandBufferManager);

		virtual ~VulkanPresentationSystem() override;

		virtual void startFrame() override;

		virtual const int getCurrentFrameIndex() const override;

		virtual ICommandBuffer& getRenderCommandBuffer() override;


	    //Vulkan Specific Method
		VulkanImageViewCombined getVulkanImageViewCombinedSwapchain(const int frameIndex);


		virtual void setWindowSize(const WindowExtent& windowExtent) override;

		//Implcity know which frame its from 
		virtual void submitRenderCommandBuffer(ICommandBuffer& commandBuffer) override;

		virtual void endFrame() override;
	};









}