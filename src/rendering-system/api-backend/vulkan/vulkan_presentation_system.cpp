#include <assert.h>
#include <iostream>

#include <volk.h>

#include <rendering-system/api-backend/vulkan/vulkan_presentation_system.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/utils/vulkan_helper.h>
#include <rendering-system/api-backend/vulkan/utils/vulkan_image_barrier_helper.h>



namespace TheEngine::RenderingSystem::VulkanBackend
{







	void VulkanPresentationSystem::handleWindowResize()
	{



		m_vulkanSwapchainManager.recreateSwapchain(m_windowExtent.physicalWidth, m_windowExtent.physicalHeight);
		m_isWindowResized = false;

		//The calling order of other methods will take care of synchronisations


	}



	VulkanPresentationSystem::VulkanPresentationSystem(VulkanContext& vulkanContext, VulkanSwapchainManager& vulkanSwapchainManager, VulkanCommandBufferManager& vulkanCommandBufferManager) :
		m_vulkanSwapchainManager(vulkanSwapchainManager),
		m_vulkanContext(vulkanContext),
		m_vulkanCommandBufferManager(vulkanCommandBufferManager)

	{

		for (auto& frame : m_frames)
		{

			frame.vulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_vulkanCommandBufferManager.getCommandBuffer(TheEngine::RenderingSystem::VulkanBackend::QueueType::UNIFIED));
			frame.inFlightFence = createVkFence(m_vulkanContext.vkDevice);
			frame.imageAvailableSem = createVkSemaphore(m_vulkanContext.vkDevice);
			frame.renderFinishedSem = createVkSemaphore(m_vulkanContext.vkDevice);
		}

	}



	VulkanPresentationSystem::~VulkanPresentationSystem()
	{

		//TODO : Implement this later


	}

	void VulkanPresentationSystem::startFrame()
	{

		const VkDevice& logicalDevice = m_vulkanContext.vkDevice;


		if (vkWaitForFences(logicalDevice, 1, &m_frames[m_currentFrame].inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS)
		{
			assert(false && "Failed to wait for in-flight fence!");
		}


		// Reset the fence now that we know the previous frame is complete.
		vkResetFences(logicalDevice, 1, &m_frames[m_currentFrame].inFlightFence);


		//IMPORTANT : HANDLE SWAPCHAIN RECREATION ETC 
		// BEFORE AQUIRE AND QUEUE SUBMIT ELSE imageAvailableSem WILL POINT 
		// TO STALE DATA CAUSING READ VIOLATION OR CRASH IN DRIVER
		if (m_isWindowResized)
		{
			handleWindowResize();//----------------> (1)

		}


		VkResult acquireResult = vkAcquireNextImageKHR(
			logicalDevice, m_vulkanSwapchainManager.getVkSwapchainKHR(),//------>(2)
			std::numeric_limits<uint64_t>::max(),
			m_frames[m_currentFrame].imageAvailableSem,
			VK_NULL_HANDLE,
			&m_currentAcquiredImageIndex);

		if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}


		m_vulkanSwapchainManager.setCurrentImageAquiredIndex(m_currentAcquiredImageIndex);


		VulkanCommandBuffer& commandBuffer = *m_frames[m_currentFrame].vulkanCommandBuffer;


		commandBuffer.reset();





	}


	const int VulkanPresentationSystem::getCurrentFrameIndex() const
	{
		return m_currentFrame;
	}


	ICommandBuffer& VulkanPresentationSystem::getRenderCommandBuffer()
	{


		return static_cast<ICommandBuffer&>(*m_frames[m_currentFrame].vulkanCommandBuffer);

		
	}



	VulkanImageViewCombined VulkanPresentationSystem::getVulkanImageViewCombinedSwapchain(const int frameIndex)
	{


		return m_vulkanSwapchainManager.getVulkanImageViewCombinedSwapchain(m_currentFrame);

	}


	void VulkanPresentationSystem::setWindowSize(const WindowExtent& windowExtent)
	{

		m_isWindowResized = true;
		m_windowExtent = windowExtent;

	}

	void VulkanPresentationSystem::submitRenderCommandBuffer(ICommandBuffer& commandBuffer)
	{



		VulkanCommandBuffer& vulkanCommandBuffer = dynamic_cast<VulkanCommandBuffer&>(commandBuffer);


		vkCmdEndRendering(vulkanCommandBuffer.getVkCommandBuffer());



		vulkanCommandBuffer.insertImageBarrier(SWAP_CHAIN_IMAGE_TEXTURE_HANDLE, TextureLayout::COLOR_ATTACHMENT, TextureLayout::PRESENT_SRC);

		vulkanCommandBuffer.endCommandBuffer();






		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		//To wait for image to be available
		VkSemaphore waitSemaphores[] = { m_frames[m_currentFrame].imageAvailableSem };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vulkanCommandBuffer.getVkCommandBuffer();


		// To Signal Render Complete
		VkSemaphore signalSemaphores[] = { m_frames[m_currentFrame].renderFinishedSem };

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;


		//The fence added here is to be waited on before resetting command buffer
		if (vkQueueSubmit(vulkanCommandBuffer.getVulkanTargetQueue(), 1, &submitInfo, m_frames[m_currentFrame].inFlightFence) != VK_SUCCESS)
		{
			assert(false && "Failed to submit draw command buffer!");
		}






		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		// Wait for the render-finished semaphore before presenting.
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_vulkanSwapchainManager.getVkSwapchainKHR();
		presentInfo.pImageIndices = &m_currentAcquiredImageIndex;
		presentInfo.pResults = nullptr; // Optional




	
		VkResult result = vkQueuePresentKHR(vulkanCommandBuffer.getVulkanTargetQueue(), &presentInfo);


		if (result != VK_SUCCESS)
		{


			//if it fails due  to window resize i cant do anything till i get new dimensions ,
			//which the render system needs to give to presentation system
			std::cout << "Presentation System : Presentation failed, but ignored" << std::endl;
		}



	}




	void VulkanPresentationSystem::endFrame()
	{
		m_currentFrame = (m_currentFrame + 1) % NUMBER_OF_FRAMES_IN_FLIGHT;
	}


}