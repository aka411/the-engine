#include "buffer_manager.h"
#include <utlis.h>









void BufferManager::allocateGlobalVertexBuffer()
{
	const size_t bufferSize = 1024 * 1024; // 1 MB buffer, is this enough?

	m_globalVertexBufferInfo.bufferSize =bufferSize;

	VkBufferCreateInfo bufferInfo {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;


	vmaCreateBuffer(m_vmaAllocator, &bufferInfo, &allocInfo, &m_globalVertexBufferInfo.bufferHandle, &m_globalVertexBufferInfo.vmaAllocation, nullptr);

	if(m_globalVertexBufferInfo.bufferHandle == VK_NULL_HANDLE)
	{
		logger("Failed to create vertex buffer");
		std::abort();
		return;
	}
	logger("Vertex buffer created successfully");



}




void BufferManager::allocateStagingBuffer()
{


	const size_t bufferSize = 1024 * 512; // 0.5 MB buffer, is this enough?

	m_stagingBufferInfo.bufferSize = bufferSize;

	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = bufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;


	vmaCreateBuffer(m_vmaAllocator, &bufferInfo, &allocInfo, &m_stagingBufferInfo.bufferHandle, &m_stagingBufferInfo.vmaAllocation, nullptr);

	if (m_stagingBufferInfo.bufferHandle == VK_NULL_HANDLE)
	{
		logger("Failed to create vertex buffer");
		std::abort();
		return;
	}
	logger("Staging buffer created successfully");



}

void BufferManager::allocateGlobalVertexSSBO()
{

	const size_t bufferSize = 1024 * 1024; // 1 MB buffer, is this enough?
	m_globalVertexSSBOInfo.bufferSize = bufferSize;

	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = bufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	

	vmaCreateBuffer(m_vmaAllocator, &bufferInfo, &allocInfo, &m_globalVertexSSBOInfo.bufferHandle, &m_globalVertexSSBOInfo.vmaAllocation, nullptr);

	if(m_globalVertexSSBOInfo.bufferHandle == VK_NULL_HANDLE)
	{
		logger("Failed to create vertex SSBO buffer");
		std::abort();
		return;
	}
	VkBufferDeviceAddressInfo addressInfo{};
	addressInfo.sType =  VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO ;
	addressInfo.buffer = m_globalVertexSSBOInfo.bufferHandle;

	m_globalVertexSSBOInfo.vulkanBDAAddress  = vkGetBufferDeviceAddress(m_vulkanDevice.getLogicalDeviceHandle(), &addressInfo);

	

}




BufferManager::BufferManager(VulkanDevice& vulkanDevice) :m_vulkanDevice(vulkanDevice), m_vmaAllocator(vulkanDevice.getVmaAllocator())
{


	

	//allocateGlobalVertexBuffer();
	allocateStagingBuffer();
	allocateGlobalVertexSSBO();

}

BufferAllocationInfo& BufferManager::getTransferBufferAllocationInfo()
{
	return m_stagingBufferInfo;
}

BufferAllocationInfo& BufferManager::getGlobalVertexSSBOInfo()
{
	
	return m_globalVertexSSBOInfo;


}

VmaAllocator& BufferManager::getVmaAllocator()
{
	return m_vmaAllocator;
}


BufferManager::~BufferManager()
{


	vmaDestroyBuffer(m_vmaAllocator, m_stagingBufferInfo.bufferHandle, m_stagingBufferInfo.vmaAllocation);
	//vmaDestroyBuffer(m_vmaAllocator, m_globalVertexBufferInfo.bufferHandle, m_globalVertexBufferInfo.vmaAllocation);
	vmaDestroyBuffer(m_vmaAllocator, m_globalVertexSSBOInfo.bufferHandle, m_globalVertexSSBOInfo.vmaAllocation);

	vmaDestroyAllocator(m_vmaAllocator);


}
