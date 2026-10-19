#pragma once
#include <vector>
#include <memory>

#include <volk.h>

#include<rendering-system/rhi/i_transfer_manager.h>
#include<rendering-system/api-backend/vulkan/vulkan_command_buffer.h>
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <rendering-system/utils/gpu-allocators/gpu_buffer_bump_suballocator.h>



namespace TheEngine::RenderingSystem::VulkanBackend
{
	
	class VulkanBufferManager;
	class VulkanContext;
	class VulkanCommandBufferManager;
	class VulkanCommandBuffer;





	class VulkanTransferManager : public ITransferManager
	{

	private:

		VulkanContext& m_vulkanContext;
		VulkanCommandBufferManager& m_vulkanCommandBufferManager;
		VulkanBufferManager& m_vulkanBufferManager;



		struct FencedCommandBuffer
		{
			std::unique_ptr<GPUBufferBumpSubAllocator> m_bufferSubAllocator;
			// Per-frame sync objects
			VkFence inFlightFence;           // CPU waits for GPU to finish 

			std::unique_ptr<VulkanCommandBuffer> vulkanCommandBuffer;

		};



		FencedCommandBuffer m_fencedCommandBuffer;
		FencedCommandBuffer m_fencedImageCommandBuffer;

		std::vector<BufferTransferRequest> m_bufferTransferRequests;
		std::vector<ImageTransferRequest> m_imageTransferRequests;
		std::vector<VkImage> m_vkImages;

	


		bool m_pendingSubmissions = false;// false only when both command buffer is signaled and requests vector is empty

	private:


		void processQueuedBufferTransferRequests(const bool isFlush = false);
		void processQueuedImageTransferRequests(const bool isFlush = false);

	public:

		VulkanTransferManager(VulkanContext& vulkanContext ,VulkanCommandBufferManager& vulkanCommandBufferManager, VulkanBufferManager& vulkanBufferManager);

		virtual ~VulkanTransferManager() override;

		virtual void transferToBuffer(BufferTransferRequest&& request) override;

		virtual void quickTransfer(const std::byte* srcDataPtr, const size_t bytesToCopy, const BufferHandle targetBufferHandle, const size_t targetOffset) override;


		//Vulkan only method
		void transferToImage(ImageTransferRequest&& imageTransferRequest, const VkImage& targetImage);

		//this require a explicit check on where there is any pending request or 
		virtual void flush() override;

	};





}
