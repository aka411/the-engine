#pragma once
#include <vector>

#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/data-structures/gpu_buffer_data_types.h>
#include <volk.h>
#include <vk_mem_alloc.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanRenderDevice;
	

	struct VulkanBufferInfo
	{
		VkBuffer vkBuffer = VK_NULL_HANDLE;
		VmaAllocation vmaAllocation = nullptr;

	};


	class VulkanBufferManager : public IBufferManager
	{

	private :

		std::vector<VulkanBufferInfo> m_vulkanBuffers;

		VmaAllocator& m_vmaAllocator;

	public :
	
		VulkanBufferManager(VmaAllocator& vmaAllocator);
		~VulkanBufferManager();

		virtual const BufferHandle createBuffer(const BufferCreateInfo& bufferCreateInfo) override;

		const VulkanBufferInfo getVulkanBufferInfo(const BufferHandle& bufferHandle) const;

		virtual void destroyBuffer(const BufferHandle& bufferHandle) override;

	};






}