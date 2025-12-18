#include "vulkan_renderer.h"
#include "volk.h"
#include <utlis.h>
#include <common_data_types.h>


void VulkanRenderer::createCommandPool()
{

	//create command pool

	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex = m_queueManager.getUnifiedQueueFamilyIndex();
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //allow command buffers to be individually reset
	commandPoolInfo.pNext = nullptr;

	if ((vkCreateCommandPool(m_vulkanDevice.getLogicalDeviceHandle(), &commandPoolInfo, nullptr, &m_commandPool) != VK_SUCCESS))
	{
		throw std::runtime_error("failed to create command pool");
	}

}

void VulkanRenderer::createCommandBuffers()
{



	//create command buffers
	VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
	commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT; //double buffering
	commandBufferAllocInfo.commandPool = m_commandPool;
	commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be submitted to a queue for execution, but cannot be called from other command buffers
	commandBufferAllocInfo.pNext = nullptr;

	if ((vkAllocateCommandBuffers(m_vulkanDevice.getLogicalDeviceHandle(), &commandBufferAllocInfo, m_commandBuffers) != VK_SUCCESS))
	{
		throw std::runtime_error("failed to allocate command buffers");
	}

	//create transfer command buffer

		//create command buffers
	VkCommandBufferAllocateInfo transferCommandBufferAllocInfo = {};
	transferCommandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	transferCommandBufferAllocInfo.commandBufferCount = 1; //double buffering
	transferCommandBufferAllocInfo.commandPool = m_commandPool;
	transferCommandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //can be submitted to a queue for execution, but cannot be called from other command buffers
	transferCommandBufferAllocInfo.pNext = nullptr;

	if ((vkAllocateCommandBuffers(m_vulkanDevice.getLogicalDeviceHandle(), &transferCommandBufferAllocInfo, &m_transferCommandBuffer) != VK_SUCCESS))
	{
		throw std::runtime_error("failed to allocate transfer command buffers");
	}



}

void VulkanRenderer::createAllSynchronisationPrimitives()
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = 0;

	if (vkCreateFence(m_vulkanDevice.getLogicalDeviceHandle(), &fenceInfo, nullptr, &m_transferFence) != VK_SUCCESS)
	{

		throw std::runtime_error("failed to create semaphores!");
	}


	auto createSemaphore = [](VkDevice logicalDevice, VkSemaphore& semaphoreHandle)
		{

			VkSemaphoreCreateInfo semaphoreCreateinfo{};
			semaphoreCreateinfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreCreateinfo.pNext = nullptr;
			semaphoreCreateinfo.flags = 0;

			if (vkCreateSemaphore(logicalDevice, &semaphoreCreateinfo,nullptr, &semaphoreHandle) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create semaphores!");
			}



		};

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;

		//If this flag was not set the very first time we tried to
		//render a frame it would have been set in not signaled state and would wait indefinitely
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // VERY IMPORTANT TO START IN SIGNALED STATE


		if (vkCreateFence(m_vulkanDevice.getLogicalDeviceHandle(), &fenceCreateInfo, nullptr, &m_perFrameSync[i].InFlightFence) != VK_SUCCESS)
		{

			throw std::runtime_error("failed to create fence for per frame synchronisation!");
		}


		createSemaphore(m_vulkanDevice.getLogicalDeviceHandle(), m_perFrameSync[i].ImageAvailableSem);
		createSemaphore(m_vulkanDevice.getLogicalDeviceHandle(), m_perFrameSync[i].RenderFinishedSem);




	}




}

void VulkanRenderer::transferDataToVertexSSBO(std::byte* byte, size_t size)
{



	//reset command buffer

	vkResetCommandBuffer(m_transferCommandBuffer, 0);




	BufferAllocationInfo transferBufferInfo = m_bufferManager.getTransferBufferAllocationInfo();

	//VkDeviceMemory transferBufferMemory = transferBufferInfo.vmaAllocation->GetMemory();

	void* mappedPointer = nullptr;


	vmaMapMemory(m_bufferManager.getVmaAllocator(), m_bufferManager.getTransferBufferAllocationInfo().vmaAllocation, &mappedPointer);
	//vkMapMemory(m_vulkanDevice.getLogicalDeviceHandle(), transferBufferMemory, 0, size, 0, &mappedPointer);

	if (mappedPointer == nullptr)
	{
		throw std::runtime_error("failed to map gpu memory to cpu");
	}

	memcpy(mappedPointer, byte, size);

	VkBuffer globalVertexSSBOBufferHandle = m_bufferManager.getGlobalVertexSSBOInfo().bufferHandle;
	VkBuffer stagingBufferHandle = transferBufferInfo.bufferHandle;

	vmaUnmapMemory(m_bufferManager.getVmaAllocator(), m_bufferManager.getTransferBufferAllocationInfo().vmaAllocation);

	VkCommandBufferBeginInfo beginInfo = {}; // Initialize all members to zero/default
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pNext = nullptr;

	vkBeginCommandBuffer(m_transferCommandBuffer, &beginInfo);


	//record a transfer

	// Define the single transfer region structure
	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;   // Starting byte offset in the source buffer
	copyRegion.dstOffset = 0;   // Starting byte offset in the destination buffer
	copyRegion.size = size;     // The size in bytes of the data block to copy


	vkCmdCopyBuffer(m_transferCommandBuffer, stagingBufferHandle, globalVertexSSBOBufferHandle, 1, &copyRegion);

	vkEndCommandBuffer(m_transferCommandBuffer);

	//submit command for excecution and out fence in

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_transferCommandBuffer;

	// Optional: Specify semaphores to wait on before execution
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr; // Array of VkPipelineStageFlags

	// Optional: Specify semaphores to signal after execution
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	//check fence or status and block
	VkResult result = vkQueueSubmit(m_queueManager.getUnifiedQueueHandle(), 1, &submitInfo, m_transferFence);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to transfer vertex data to SSBO");
	}

	//wait to complete

	VkResult transferResult = vkWaitForFences(m_vulkanDevice.getLogicalDeviceHandle(), 1, &m_transferFence, VK_TRUE, 1000000000);
	if (transferResult != VK_SUCCESS)
	{
		throw std::runtime_error("failed to wait for transfer fence");
	}

	logger("Transfer successful");

	//reset fence

	vkResetFences(m_vulkanDevice.getLogicalDeviceHandle(), 1, &m_transferFence);
}

VulkanRenderer::VulkanRenderer(vkb::Instance instance, VkSurfaceKHR surface) : m_vulkanDevice(instance, surface), m_queueManager(m_vulkanDevice), m_bufferManager(m_vulkanDevice), m_swapChainManager(m_vulkanDevice), m_descriptorSetManager(m_vulkanDevice), m_pipelineManager(m_vulkanDevice, m_descriptorSetManager)
{
	//assuming the order of initialisation of member variables is guranteed

	createCommandPool();

	createCommandBuffers();


	createAllSynchronisationPrimitives();


	std::vector<float> vertexData = getVertexData();

	transferDataToVertexSSBO(reinterpret_cast<std::byte*>(vertexData.data()), sizeof(float)*(vertexData.size()));









}

void VulkanRenderer::setViewPort(size_t width, size_t height)
{
	m_swapChainManager.setViewportDimensions(width, height);


}

void VulkanRenderer::renderFrame()
{
	m_swapChainManager.ensureReadyForDrawing();


	const VkDevice& logicalDevice = m_vulkanDevice.getLogicalDeviceHandle();


	if (vkWaitForFences(logicalDevice, 1, &m_perFrameSync[m_currentFrame].InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to wait for in-flight fence!");
	}

	// Reset the fence now that we know the previous frame is complete.
	vkResetFences(logicalDevice, 1, &m_perFrameSync[m_currentFrame].InFlightFence);

	// Acquire an image from the swap chain, signaling with the imageAvailable semaphore.
	uint32_t acquiredImageIndex;
	VkResult acquireResult = vkAcquireNextImageKHR(
		logicalDevice, m_swapChainManager.getSwapChain(), std::numeric_limits<uint64_t>::max(),
		m_perFrameSync[m_currentFrame].ImageAvailableSem, VK_NULL_HANDLE, &acquiredImageIndex);

	if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	VkCommandBuffer commandBuffer = m_commandBuffers[m_currentFrame];

	// Now that the command buffer is safe to use, reset it.
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkImageMemoryBarrier initialBarrier{};
	initialBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	// Transition from UNDEFINED (or PRESENT_SRC_KHR if it was just presented)
	initialBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	initialBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Wait on the ImageAvailable semaphore, which happens in the TOP_OF_PIPE stage
	// and allows the transition to proceed.
	initialBarrier.srcAccessMask = 0;
	initialBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	initialBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	initialBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	initialBarrier.image = m_swapChainManager.getSwapChainImage(acquiredImageIndex);

	initialBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	initialBarrier.subresourceRange.baseMipLevel = 0;
	initialBarrier.subresourceRange.levelCount = 1;
	initialBarrier.subresourceRange.baseArrayLayer = 0;
	initialBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(
		commandBuffer,
		// srcStageMask: Start of the pipeline (after semaphore signal)
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		// dstStageMask: Before the color attachment output stage starts
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		0, // dependencyFlags
		0, NULL, // memory barriers
		0, NULL, // buffer memory barriers
		1, &initialBarrier // image memory barriers
	);


	VkRenderingAttachmentInfoKHR colorAttachment{};
	colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colorAttachment.imageView = m_swapChainManager.getSwapChainImageView(acquiredImageIndex); // <-- Direct reference to the image view
	colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.clearValue.color =  {0.0f, 0.0f, 0.0f, 1.0f} ;

	//colorAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
	//colorAttachment.resolveImageView = m_swapChainManager.getSwapChainImageView(acquiredImageIndex);
	//colorAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	const ViewPortDimension viewPortDimensions = m_swapChainManager.getViewportDimensions();

	VkRect2D renderArea{};
	renderArea.extent = { (unsigned int)(viewPortDimensions.width),(unsigned int)(viewPortDimensions.height )};
	renderArea.offset = {0,0};


	// 3. Define the overall rendering information
	VkRenderingInfoKHR renderingInfo{};
	renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderingInfo.renderArea = renderArea; // Same as before
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &colorAttachment;
	renderingInfo.pDepthAttachment = nullptr;
	renderingInfo.pStencilAttachment = NULL;


	VkPipeline& pipeline = m_pipelineManager.getPipeline();
	VkPipelineLayout& pipelineLayout = m_pipelineManager.getPipelineLayout();









	vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);


	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)viewPortDimensions.width;
	viewport.height = (float)viewPortDimensions.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkExtent2D actualExtent = {
	static_cast<uint32_t>(viewPortDimensions.width),
	static_cast<uint32_t>(viewPortDimensions.height)
	};
	// Define the Scissor
	VkRect2D scissor{};
	scissor.offset = { 0, 0 }; // Start at the top-left corner
	scissor.extent = actualExtent; // Cover the full area




	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	PushConst pushConstants;
	pushConstants.modelMatrix = glm::mat4x4(1.0f);
	pushConstants.ssboOffset =  m_bufferManager.getGlobalVertexSSBOInfo().vulkanBDAAddress + 0;


	vkCmdPushConstants(
		commandBuffer,
		pipelineLayout,          // The layout associated with the bound pipeline
		VK_SHADER_STAGE_VERTEX_BIT, // Must match the stage flags from Step 1
		0,                       // Offset (0 for the first/only range)
		sizeof(pushConstants), // Size of the data to push
		&pushConstants               // Pointer to the actual data
	);

	uint32_t vertexCount = 3; // Total number of vertices in the SSBO for this draw call
	uint32_t instanceCount = 1;

	vkCmdDraw(
		commandBuffer,
		vertexCount,
		instanceCount,
		0, // firstVertex (start index in the SSBO)
		0  // firstInstance
	);



	vkCmdEndRenderingKHR(commandBuffer);

	VkImageMemoryBarrier imageBarrier{};
	imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageBarrier.dstAccessMask = 0; // No access needed after presentation transition
		imageBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		imageBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageBarrier.image = m_swapChainManager.getSwapChainImage(acquiredImageIndex);


		imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBarrier.subresourceRange.baseMipLevel = 0;
		imageBarrier.subresourceRange.levelCount = 1;
		imageBarrier.subresourceRange.baseArrayLayer = 0;
		imageBarrier.subresourceRange.layerCount = 1;



	vkCmdPipelineBarrier(
		commandBuffer,
		// srcStageMask: All color attachment output operations must finish
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		// dstStageMask: Wait for presentation engine operations (top of pipe is safe)
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
		0, // dependencyFlags
		0, NULL, // memory barriers
		0, NULL, // buffer memory barriers
		1, &imageBarrier // image memory barriers
	);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	// This semaphore signals that the image is ready for us.
	VkSemaphore waitSemaphores[] = { m_perFrameSync[m_currentFrame].ImageAvailableSem };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	// This semaphore will be signaled when the rendering is complete.
	VkSemaphore signalSemaphores[] = { m_perFrameSync[m_currentFrame].RenderFinishedSem };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_queueManager.getUnifiedQueueHandle(), 1, &submitInfo, m_perFrameSync[m_currentFrame].InFlightFence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	// REMOVED: vkQueueWaitIdle(m_mainQueueInfo.queue);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	// Wait for the render-finished semaphore before presenting.
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_swapChainManager.getSwapChain();
	presentInfo.pImageIndices = &acquiredImageIndex;
	presentInfo.pResults = nullptr; // Optional
	VkResult result = vkQueuePresentKHR(m_queueManager.getUnifiedQueueHandle(), &presentInfo);
	if (result != VK_SUCCESS)
	{
		//can fail when screen is resized
		//	throw std::runtime_error("failed to present swap chain image!");
	}
	//m_prevFrame = m_currentFrame;
	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;


}



std::vector<float> VulkanRenderer::getVertexData()
{

	//Triangle vertices

	std::vector<float> vertice = {
		//position             //color
		0.0f, 0.5f, 0.0f,0.0f,      1.0f, 0.0f, 0.0f,0.0f, //bottom vertex, red
		-0.5f, -0.5f, 0.0f,0.0f,    0.0f, 1.0f, 0.0f,0.0f, //right vertex, green
		0.5f,-0.5f, 0.0f,0.0f,       0.0f, 0.0f,1.0f,0.0f //left vertex, blue
	};

	return vertice;

}






VulkanRenderer::~VulkanRenderer()
{




	vkDeviceWaitIdle(m_vulkanDevice.getLogicalDeviceHandle());
	vkDestroyCommandPool(m_vulkanDevice.getLogicalDeviceHandle(), m_commandPool, nullptr);

}
