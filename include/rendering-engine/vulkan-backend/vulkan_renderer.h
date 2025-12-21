#pragma once 
#include <vulkan_device.h>
#include <queue_manager.h>
#include <buffer_manager.h>
#include <swap_chain_manager.h>
#include <pipeline_manager.h>



struct PerFrameSync
{
	VkFence InFlightFence = VK_NULL_HANDLE;//for command buffer

	VkSemaphore ImageAvailableSem = VK_NULL_HANDLE;
	/*vkAcquireNextImageKHR	Signal	The CPU passes this semaphore to this function. 
	The Presentation Engine (part of the OS/Windowing System) signals it when 
	the swapchain image is available to be rendered to.*/


	VkSemaphore RenderFinishedSem = VK_NULL_HANDLE;
/*The  Presentation Engine is made to wait for this to signal so it presents only after this is signaled*/



};



class VulkanRenderer
{
private:

	VulkanDevice m_vulkanDevice;
	QueueManager m_queueManager;


	BufferManager m_bufferManager;
	SwapChainManager m_swapChainManager;
	PipelineManager m_pipelineManager;
	DescriptorSetManager m_descriptorSetManager;


	static const int MAX_FRAMES_IN_FLIGHT = 2;


	VkCommandPool m_commandPool = VK_NULL_HANDLE;



	VkCommandBuffer m_transferCommandBuffer = VK_NULL_HANDLE;//need synchronisation for this 
	VkFence m_transferFence = VK_NULL_HANDLE;

	//double buffering
	VkCommandBuffer m_commandBuffers[MAX_FRAMES_IN_FLIGHT] = { VK_NULL_HANDLE, VK_NULL_HANDLE };//need synchronisation for this 

	PerFrameSync m_perFrameSync[MAX_FRAMES_IN_FLIGHT];
	
	unsigned int  m_currentFrame = 0;


	void createCommandPool();
	void createCommandBuffers();

	void createAllSynchronisationPrimitives();

	void transferDataToVertexSSBO(std::byte* byte, size_t size);


	std::vector<float> getVertexData();

public:

	VulkanRenderer(vkb::Instance instance, VkSurfaceKHR surface);
	void setViewPort(size_t width, size_t height);
	void renderFrame();

	~VulkanRenderer();
};