#include <volk.h>
#include <vk_mem_alloc.h>

#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{
	static VkBufferUsageFlags getVkBufferUsageFlagBits(const uint32_t bufferUsage)
	{
		VkBufferUsageFlags flags = 0;

		if (bufferUsage & BufferUsage::TRANSFER_SRC_BIT)    flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		if (bufferUsage & BufferUsage::TRANSFER_DST_BIT)    flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if (bufferUsage & BufferUsage::UNIFORM_BUFFER_BIT)  flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (bufferUsage & BufferUsage::STORAGE_BUFFER_BIT)  flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if (bufferUsage & BufferUsage::VERTEX_BUFFER_BIT)   flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if (bufferUsage & BufferUsage::INDEX_BUFFER_BIT)    flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (bufferUsage & BufferUsage::INDIRECT_BUFFER_BIT) flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		if (bufferUsage & BufferUsage::DEVICE_ADDRESS_BIT)  flags |= VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

		return flags;
	}


	static VkMemoryPropertyFlags getVkMemoryPropertyFlags(const uint32_t memoryPropertyFlags)
	{
		VkMemoryPropertyFlags flags = 0;


		if (memoryPropertyFlags & (1 << MemoryPropertyFlags::DEVICE_LOCAL_BIT))
			flags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		if (memoryPropertyFlags & (1 << MemoryPropertyFlags::HOST_VISIBLE_BIT))
			flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		if (memoryPropertyFlags & (1 << MemoryPropertyFlags::HOST_COHERENT_BIT))
			flags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if (memoryPropertyFlags & (1 << MemoryPropertyFlags::HOST_CACHED_BIT))
			flags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

		return flags;
	}



	static VmaAllocationCreateInfo getVmaAllocationCreateInfo(const uint32_t engineFlags)
	{
		VmaAllocationCreateInfo allocInfo{};


		if (engineFlags & MemoryPropertyFlags::DEVICE_LOCAL_BIT)
		{

			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
		}
		else if (engineFlags & MemoryPropertyFlags::HOST_VISIBLE_BIT)
		{
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
		}
		else
		{
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		}

		//Mapping and Coherency
		if (engineFlags & MemoryPropertyFlags::HOST_VISIBLE_BIT)
		{
			
			allocInfo.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;

			// SEQUENTIAL_WRITE -> best for staging/uniforms (CPU -> GPU)
			// RANDOM_ACCESS    -> better if the CPU is reading back (GPU -> CPU)
			if (engineFlags & MemoryPropertyFlags::HOST_CACHED_BIT)
			{
				allocInfo.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
			}
			else
			{
				allocInfo.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
			}
		}

		return allocInfo;
	}






	VulkanBufferManager::VulkanBufferManager(VmaAllocator& vmaAllocator) :
		m_vmaAllocator(vmaAllocator)
	{

	}


	VulkanBufferManager::~VulkanBufferManager()
	{
		VmaAllocator allocator = m_vmaAllocator;
		for (const auto& bufferInfo : m_vulkanBuffers)
		{
			if (bufferInfo.vkBuffer != VK_NULL_HANDLE)
			{
				vmaDestroyBuffer(allocator, bufferInfo.vkBuffer, bufferInfo.vmaAllocation);
			}
		}
	

	}



	const BufferHandle VulkanBufferManager::createBuffer(const BufferCreateInfo& bufferCreateInfo)
	{


		VkBufferCreateInfo vkBufferInfo{};
		vkBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vkBufferInfo.size = bufferCreateInfo.size;
		vkBufferInfo.usage = getVkBufferUsageFlagBits(bufferCreateInfo.bufferUsage);
		vkBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//Headache


		
		VmaAllocationCreateInfo vmaAllocInfo = getVmaAllocationCreateInfo(bufferCreateInfo.memoryPropertyFlags);

		VkBuffer buffer;
		VmaAllocation allocation;
		VmaAllocationInfo allocationResult;
		
		
		VkResult result = vmaCreateBuffer(
			m_vmaAllocator,
			&vkBufferInfo,
			&vmaAllocInfo,
			&buffer,
			&allocation,
			&allocationResult
		);

		if (result != VK_SUCCESS) 
		{ 
			assert(false && "VulkanBackend : Buffer Allocation failed");
		}

	
		BufferMetadata meta{};
		meta.size = bufferCreateInfo.size;
		meta.bufferUsage = bufferCreateInfo.bufferUsage;
		meta.memoryPropertyFlags = bufferCreateInfo.memoryPropertyFlags;

	
		if (vmaAllocInfo.flags & VMA_ALLOCATION_CREATE_MAPPED_BIT)
		{
			meta.isMapped = true;
			meta.mappedPtr = allocationResult.pMappedData;
		}

	
	

		BufferHandle bufferHandle;
		bufferHandle.id = m_bufferRegistry.size();
		m_bufferRegistry.push_back(meta);


		VulkanBufferInfo vulkanBufferInfo;
		vulkanBufferInfo.vkBuffer = buffer;
		vulkanBufferInfo.vmaAllocation = allocation;

		m_vulkanBuffers.push_back(vulkanBufferInfo);



		return bufferHandle;

	}


	const VulkanBufferInfo VulkanBufferManager::getVulkanBufferInfo(const BufferHandle& bufferHandle) const
	{

		assert(bufferHandle.id <= m_vulkanBuffers.size() && "VulkanBufferManager : Invalid BufferHandle");

		return m_vulkanBuffers[bufferHandle.id];
	}


	void VulkanBufferManager::destroyBuffer(const BufferHandle& bufferHandle)
	{
		if (bufferHandle.id == 0 || bufferHandle.id > m_vulkanBuffers.size())
		{
			return;
		}

		size_t index = bufferHandle.id;
		auto& internalInfo = m_vulkanBuffers[index];

		if (internalInfo.vkBuffer != VK_NULL_HANDLE)
		{
			vmaDestroyBuffer(
				m_vmaAllocator,
				internalInfo.vkBuffer,
				internalInfo.vmaAllocation
			);

			// Scorch the entries so they can't be used again
			internalInfo.vkBuffer = VK_NULL_HANDLE;
			internalInfo.vmaAllocation = nullptr;
			//m_bufferRegistry[index] = {};
		}

	}


}