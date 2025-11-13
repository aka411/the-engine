#pragma once 
#include "volk.h"	

#include <vulkan_device.h>
#include "vk_mem_alloc.h"


struct BufferAllocationInfo
{
	VkBuffer bufferHandle = VK_NULL_HANDLE;
	VmaAllocation vmaAllocation;//

	size_t bufferSize = 0;
	VkDeviceAddress vulkanBDAAddress = 0;
};



class BufferManager
{
private:
	VulkanDevice& m_vulkanDevice;
	VmaAllocator& m_vmaAllocator;


	
	BufferAllocationInfo m_globalVertexBufferInfo;
	BufferAllocationInfo m_stagingBufferInfo;

	BufferAllocationInfo m_globalVertexSSBOInfo;




	void allocateGlobalVertexBuffer();
	void allocateStagingBuffer();

	void allocateGlobalVertexSSBO();


public:

	BufferManager(VulkanDevice& vulkanDevice);

	BufferAllocationInfo& getTransferBufferAllocationInfo();
	BufferAllocationInfo& getGlobalVertexSSBOInfo();

	VmaAllocator& getVmaAllocator();

	~BufferManager();
};