#include <cassert>

#include <rendering-system/api-backend/vulkan/vulkan_transfer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer_manager.h>
#include <rendering-system/utils/gpu-allocators/gpu_buffer_bump_suballocator.h>
#include <rendering-system/api-backend/vulkan/utils/vulkan_helper.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{

	
	void VulkanTransferManager::processQueuedBufferTransferRequests(const bool isFlush)
	{


		//Flush exit condition -> (1) All requests completed AND (2) CommandBuffer Free
		//No flush exit condition -> (1) CommandBuffer in use (2) allocator OOM 




		auto submitCommandBuffer = [&]()
			{
				m_fencedCommandBuffer.vulkanCommandBuffer->endCommandBuffer();

				VkSubmitInfo submitInfo{};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &m_fencedCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer();

				// No need to wait on anything
				submitInfo.waitSemaphoreCount = 0;
				submitInfo.pWaitSemaphores = nullptr;
				submitInfo.pWaitDstStageMask = nullptr; 

				//No need to signal anything else on GPU
				submitInfo.signalSemaphoreCount = 0;
				submitInfo.pSignalSemaphores = nullptr;

				vkResetFences(m_vulkanContext.vkDevice, 1, &m_fencedCommandBuffer.inFlightFence);
				VkResult submitResult = vkQueueSubmit(m_fencedCommandBuffer.vulkanCommandBuffer->getVulkanTargetQueue(), 1, &submitInfo, m_fencedCommandBuffer.inFlightFence);
				if (!(submitResult == VK_SUCCESS))
				{
					assert(false && "Failed to submit transfer command buffer");
				}

		
				//m_fencedCommandBuffer.m_bufferSubAllocator->reset();
				return;
			};


		const int ALIGNEMENT_REQUIRED = 256;

		auto allocateAndRecord = [&](BufferTransferRequest& inRequest) ->bool
			{


				GPUSubAllocationInfo gpuSubAllocationInfo = m_fencedCommandBuffer.m_bufferSubAllocator->allocateAligned(inRequest.memoryblock.getSize(), ALIGNEMENT_REQUIRED);
				BufferMetadata stagingBufferMetadata = m_vulkanBufferManager.getBufferMetadata(m_fencedCommandBuffer.m_bufferSubAllocator->getBufferHandle());


				if (gpuSubAllocationInfo.isAllocationSuccessful)
				{
					//Allocation was a success



					void* stagingBufferPtr = (std::byte*)(stagingBufferMetadata.mappedPtr) + gpuSubAllocationInfo.offset;
					memcpy(stagingBufferPtr, inRequest.memoryblock.getData(), inRequest.memoryblock.getSize());

					if (!m_fencedCommandBuffer.vulkanCommandBuffer->isRecording())
					{
						m_fencedCommandBuffer.vulkanCommandBuffer->beginCommandBuffer();
					}


					//Copy Command
					VkBufferCopy copyRegion = {};
					copyRegion.srcOffset = gpuSubAllocationInfo.offset; 
					copyRegion.dstOffset = inRequest.targetOffset;   
					copyRegion.size = inRequest.memoryblock.getSize(); 

					VkBuffer vkStagingBuffer = m_vulkanBufferManager.getVulkanBufferInfo(m_fencedCommandBuffer.m_bufferSubAllocator->getBufferHandle()).vkBuffer;
					VkBuffer vkTargetBuffer = m_vulkanBufferManager.getVulkanBufferInfo(inRequest.targetBufferHandle).vkBuffer;



					vkCmdCopyBuffer(m_fencedCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer(), vkStagingBuffer, vkTargetBuffer, 1, &copyRegion);



					VkBufferMemoryBarrier2 bufferBarrier = {};
					bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
					bufferBarrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
					bufferBarrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;

					// TODO : Fine grain this later
					bufferBarrier.dstStageMask = VK_PIPELINE_STAGE_2_VERTEX_ATTRIBUTE_INPUT_BIT |
						VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT |
						VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT |
						VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

					bufferBarrier.dstAccessMask = VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT |
						VK_ACCESS_2_INDEX_READ_BIT |
						VK_ACCESS_2_UNIFORM_READ_BIT |
						VK_ACCESS_2_SHADER_READ_BIT;



					bufferBarrier.buffer = vkTargetBuffer;
					bufferBarrier.offset = inRequest.targetOffset;
					bufferBarrier.size = inRequest.memoryblock.getSize();
					bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

					VkDependencyInfo dependencyInfo = {};
					dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
					dependencyInfo.bufferMemoryBarrierCount = 1;
					dependencyInfo.pBufferMemoryBarriers = &bufferBarrier;

					vkCmdPipelineBarrier2(m_fencedCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer(), &dependencyInfo);
					//Dont submit

					return true;

				}

				return false;
			};




		for (int i = m_bufferTransferRequests.size() -1; i >= 0 ; --i)
		{

			auto& request = m_bufferTransferRequests[i];


			//IF flush, hard wait on fence
			VkResult fenceStatusVkResult = vkGetFenceStatus(m_vulkanContext.vkDevice, m_fencedCommandBuffer.inFlightFence);

			if (isFlush && fenceStatusVkResult != VK_SUCCESS)
			{
				//hard wait
				vkWaitForFences(m_vulkanContext.vkDevice, 1,&m_fencedCommandBuffer.inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
				fenceStatusVkResult = vkGetFenceStatus(m_vulkanContext.vkDevice, m_fencedCommandBuffer.inFlightFence);
			}



			if (!(fenceStatusVkResult == VK_SUCCESS))
			{
				//Not free
			
				if (isFlush)
				{
					assert(false && "Command Buffer not getting free for flushing all transfer requests");
				}
				return;
			}
			else
			{
				//Free
				// 
				//const int ALIGNEMENT_REQUIRED = 256;

				//GPUSubAllocationInfo gpuSubAllocationInfo = m_fencedCommandBuffer.m_bufferSubAllocator->allocateAligned(request.memoryblock.getSize(), ALIGNEMENT_REQUIRED);
				BufferMetadata stagingBufferMetadata = m_vulkanBufferManager.getBufferMetadata(m_fencedCommandBuffer.m_bufferSubAllocator->getBufferHandle());


				if (allocateAndRecord(request))
				{
					m_bufferTransferRequests.pop_back();
					if (m_bufferTransferRequests.size() == 0)
					{
						submitCommandBuffer();
						m_fencedCommandBuffer.m_bufferSubAllocator->reset();


						if (!isFlush)
						{
							break;
						}
					}
			
					continue;
				}
				else
				{

					//Check reason for Allocation failure

					if (ALIGNEMENT_REQUIRED + request.memoryblock.getSize() <= stagingBufferMetadata.size)
					{
						//Staging buffer size is enough to accomodate request

						//submit till now

						submitCommandBuffer();

						//m_bufferTransferRequests.push_back(std::move(request));
						m_fencedCommandBuffer.m_bufferSubAllocator->reset();
						continue;
					}
					else
					{
						//Need bigger staging buffer to accomodate this trasnfer request
						//TODO : Implement later
						//1.Allocate new temp buffer
						//2.submit recorded command with this new request also
						//3.return
						/*if (!m_fencedCommandBuffer.vulkanCommandBuffer.isRecording())
						{
							m_fencedCommandBuffer.vulkanCommandBuffer.beginCommandBuffer();
						}
						*/
						assert(false && "Request size greater than size of staging buffer available");
						return;
					}


				}




			}
		}


		if (isFlush)
		{
			vkWaitForFences(m_vulkanContext.vkDevice, 1, &m_fencedCommandBuffer.inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());

		}


	}





	void VulkanTransferManager::processQueuedImageTransferRequests(const bool isFlush)
	{



		auto submitCommandBuffer = [&]()
			{
				m_fencedImageCommandBuffer.vulkanCommandBuffer->endCommandBuffer();

				VkSubmitInfo submitInfo{};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &m_fencedImageCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer();

				
				submitInfo.waitSemaphoreCount = 0;
				submitInfo.pWaitSemaphores = nullptr;
				submitInfo.pWaitDstStageMask = nullptr;

				
				submitInfo.signalSemaphoreCount = 0;
				submitInfo.pSignalSemaphores = nullptr;

				vkResetFences(m_vulkanContext.vkDevice, 1, &m_fencedImageCommandBuffer.inFlightFence);

				VkResult submitResult = vkQueueSubmit(m_fencedImageCommandBuffer.vulkanCommandBuffer->getVulkanTargetQueue(), 1, &submitInfo, m_fencedImageCommandBuffer.inFlightFence);
				if (!(submitResult == VK_SUCCESS))
				{
					assert(false && "Failed to submit transfer command buffer");
				}


				//	m_fencedCommandBuffer.m_bufferSubAllocator->reset();
				return;
			};


		const int ALIGNEMENT_REQUIRED = 256;  //+ need to handle NPOT

		auto allocateAndRecord = [&](ImageTransferRequest& inRequest, VkImage& vkImage) ->bool
			{


				GPUSubAllocationInfo gpuSubAllocationInfo = m_fencedImageCommandBuffer.m_bufferSubAllocator->allocateAligned(inRequest.memoryBlock.getSize(), ALIGNEMENT_REQUIRED);
				BufferMetadata stagingBufferMetadata = m_vulkanBufferManager.getBufferMetadata(m_fencedCommandBuffer.m_bufferSubAllocator->getBufferHandle());


				if (gpuSubAllocationInfo.isAllocationSuccessful)
				{
					//Allocation was a success



					void* stagingBufferPtr = (std::byte*)(stagingBufferMetadata.mappedPtr) + gpuSubAllocationInfo.offset;
					memcpy(stagingBufferPtr, inRequest.memoryBlock.getData(), inRequest.memoryBlock.getSize());

					if (!m_fencedImageCommandBuffer.vulkanCommandBuffer->isRecording())
					{
						m_fencedImageCommandBuffer.vulkanCommandBuffer->beginCommandBuffer();
					}



					//Transition: UNDEFINED -> TRANSFER_DST_OPTIMAL
					VkImageMemoryBarrier barrier{};
					barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

					barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

					barrier.srcAccessMask = 0;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

					barrier.image = vkImage;

					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					barrier.subresourceRange.baseMipLevel = 0;
					barrier.subresourceRange.baseArrayLayer = inRequest.destinationBaseArrayLayer;
					barrier.subresourceRange.layerCount = inRequest.destinationLayerCount;
					barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;


					vkCmdPipelineBarrier(m_fencedImageCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						0, 0, nullptr, 0, nullptr, 1, &barrier);

					
					VkBufferImageCopy region{};
					region.bufferOffset = 0;
					region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					region.imageSubresource.mipLevel = inRequest.destinationMipLevel;
					region.imageSubresource.baseArrayLayer = inRequest.destinationBaseArrayLayer;
					region.imageSubresource.layerCount = inRequest.destinationLayerCount;



					region.imageExtent = { inRequest.textureDescription.width, inRequest.textureDescription.height, 1 };



					VkBuffer stagingBufferVk = m_vulkanBufferManager.getVulkanBufferInfo(m_fencedImageCommandBuffer.m_bufferSubAllocator->getBufferHandle()).vkBuffer;

					vkCmdCopyBufferToImage(m_fencedImageCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer(), stagingBufferVk, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

					//Transition: TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(m_fencedImageCommandBuffer.vulkanCommandBuffer->getVkCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						0, 0, nullptr, 0, nullptr, 1, &barrier);
					//Dont submit

					return true;

				}

				return false;
			};




		for (int i = m_imageTransferRequests.size() - 1; i >= 0;--i)
		{

			auto& request = m_imageTransferRequests[i];
			auto& vkImage = m_vkImages[i];

			//IF flush, hard wait on fence
			VkResult fenceStatusVkResult = vkGetFenceStatus(m_vulkanContext.vkDevice, m_fencedImageCommandBuffer.inFlightFence);

			if (isFlush && fenceStatusVkResult != VK_SUCCESS)
			{
				//hard wait
				vkWaitForFences(m_vulkanContext.vkDevice, 1, &m_fencedImageCommandBuffer.inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
				fenceStatusVkResult = vkGetFenceStatus(m_vulkanContext.vkDevice, m_fencedImageCommandBuffer.inFlightFence);
			}



			if (!(fenceStatusVkResult == VK_SUCCESS))
			{
				//Not free

				if (isFlush)
				{
					assert(false && "Command Buffer not getting free for flushing all transfer requests");
				}
				return;
			}
			else
			{
				//Free
				// 
				//const int ALIGNEMENT_REQUIRED = 256;

				//GPUSubAllocationInfo gpuSubAllocationInfo = m_fencedCommandBuffer.m_bufferSubAllocator->allocateAligned(request.memoryblock.getSize(), ALIGNEMENT_REQUIRED);
				BufferMetadata stagingBufferMetadata = m_vulkanBufferManager.getBufferMetadata(m_fencedImageCommandBuffer.m_bufferSubAllocator->getBufferHandle());


				if (allocateAndRecord(request, vkImage))
				{
					m_imageTransferRequests.pop_back();
					m_vkImages.pop_back();

					if (m_imageTransferRequests.size() == 0)
					{
						submitCommandBuffer();
						m_fencedImageCommandBuffer.m_bufferSubAllocator->reset();


						if (!isFlush)
						{
							break;
						}
					}

					continue;
				}
				else
				{

					//Check reason for Allocation failure

					//Need pading etc also
					if (ALIGNEMENT_REQUIRED + request.memoryBlock.getSize() <= stagingBufferMetadata.size)
					{
						//Staging buffer size is enough to accomodate request

						//submit till now

						submitCommandBuffer();

						//m_bufferTransferRequests.push_back(std::move(request));
						m_fencedImageCommandBuffer.m_bufferSubAllocator->reset();
						continue;
					}
					else
					{
						//Need bigger staging buffer to accomodate this trasnfer request
						//TODO : Implement later
						//1.Allocate new temp buffer
						//2.submit recorded command with this new request also
						//3.return
						/*if (!m_fencedCommandBuffer.vulkanCommandBuffer.isRecording())
						{
							m_fencedCommandBuffer.vulkanCommandBuffer.beginCommandBuffer();
						}
						*/
						assert(false && "Request size greater than size of staging buffer available");
						return;
					}


				}




			}
		}


		if (isFlush)
		{
			vkWaitForFences(m_vulkanContext.vkDevice, 1, &m_fencedImageCommandBuffer.inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
			
		}



	}






	VulkanTransferManager::VulkanTransferManager(VulkanContext& vulkanContext, VulkanCommandBufferManager& vulkanCommandBufferManager, VulkanBufferManager& vulkanBufferManager) :
		ITransferManager(vulkanBufferManager),
		m_vulkanContext(vulkanContext),
		m_vulkanCommandBufferManager(vulkanCommandBufferManager),
		m_vulkanBufferManager(vulkanBufferManager)

	{


		const size_t stagingBufferSize = 1024 * 1024 * 50;//50 MiB 

		BufferCreateInfo bufferCreateInfo;
		bufferCreateInfo.bufferUsage = BufferUsage::TRANSFER_SRC_BIT;
		bufferCreateInfo.memoryPropertyFlags = MemoryPropertyFlags::HOST_VISIBLE_BIT | MemoryPropertyFlags::HOST_COHERENT_BIT;
		bufferCreateInfo.size = stagingBufferSize;


		BufferHandle bufferStagingBufferHandle = m_vulkanBufferManager.createBuffer(bufferCreateInfo);

		// Initialize the Sub-Allocators with the newly created buffers
		m_fencedCommandBuffer.m_bufferSubAllocator = std::make_unique<GPUBufferBumpSubAllocator>(
			bufferStagingBufferHandle,
			stagingBufferSize
		);

		m_fencedCommandBuffer.vulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_vulkanCommandBufferManager.getCommandBuffer(TheEngine::RenderingSystem::VulkanBackend::QueueType::UNIFIED));
		m_fencedCommandBuffer.inFlightFence = createVkFence(m_vulkanContext.vkDevice);




		//For Image Transfer Processing

		BufferHandle imageStagingBufferHandle = m_vulkanBufferManager.createBuffer(bufferCreateInfo);

		//Initialize the Sub-Allocators with the newly created buffers

		m_fencedImageCommandBuffer.m_bufferSubAllocator = std::make_unique<GPUBufferBumpSubAllocator>(
			imageStagingBufferHandle,
			stagingBufferSize
		);

		m_fencedImageCommandBuffer.vulkanCommandBuffer = std::make_unique <VulkanCommandBuffer>(m_vulkanCommandBufferManager.getCommandBuffer(TheEngine::RenderingSystem::VulkanBackend::QueueType::UNIFIED));
		m_fencedImageCommandBuffer.inFlightFence = createVkFence(m_vulkanContext.vkDevice);

	}


	VulkanTransferManager::~VulkanTransferManager()
	{



	}



	void VulkanTransferManager::transferToBuffer(BufferTransferRequest&& request)
	{

		//FIRST CHECK IF TARGET BUFFER IS MAPPED 



		BufferMetadata targetBufferMetadata = m_vulkanBufferManager.getBufferMetadata(request.targetBufferHandle);

		if (targetBufferMetadata.isMapped && targetBufferMetadata.mappedPtr != nullptr)
		{

			size_t requestedEnd = request.targetOffset + request.memoryblock.getSize();

			assert(requestedEnd <= targetBufferMetadata.size && "Out of bounds: Offset + Data Size exceeds buffer capacity");

			void* targetPtr = (std::byte*)(targetBufferMetadata.mappedPtr) + request.targetOffset;
			memcpy(targetPtr, request.memoryblock.getData(), request.memoryblock.getSize());

			if (!(targetBufferMetadata.memoryPropertyFlags & MemoryPropertyFlags::HOST_COHERENT_BIT))
			{
			
				VulkanBufferInfo vulkanBufferInfo = m_vulkanBufferManager.getVulkanBufferInfo(request.targetBufferHandle);

				vmaFlushAllocation(m_vulkanContext.vmaAllocator, vulkanBufferInfo.vmaAllocation, request.targetOffset, targetBufferMetadata.size);
				

			}


			return;
		}


		//store in queued requests
		m_bufferTransferRequests.push_back(std::move(request));

		processQueuedBufferTransferRequests();

	}



	void VulkanTransferManager::quickTransfer(const std::byte* srcDataPtr, const size_t bytesToCopy, const BufferHandle targetBufferHandle, const size_t targetOffset)
	{
		BufferMetadata bufferMetadata = m_vulkanBufferManager.getBufferMetadata(targetBufferHandle);

		if (bufferMetadata.isMapped && bufferMetadata.mappedPtr != nullptr)
		{

			size_t requestedEnd = targetOffset + bytesToCopy;

			assert(requestedEnd <= bufferMetadata.size && "Out of bounds: Offset + Data Size exceeds buffer capacity");

			void* targetPtr = (std::byte*)(bufferMetadata.mappedPtr) + targetOffset;
			memcpy(targetPtr, srcDataPtr, bytesToCopy);



			if (!(bufferMetadata.memoryPropertyFlags & MemoryPropertyFlags::HOST_COHERENT_BIT))
			{

				VulkanBufferInfo vulkanBufferInfo = m_vulkanBufferManager.getVulkanBufferInfo(targetBufferHandle);
				vmaFlushAllocation(m_vulkanContext.vmaAllocator, vulkanBufferInfo.vmaAllocation, targetOffset, bytesToCopy);
			}

			return;
		}

		assert(false && "Tried to do a quick transfer on non mapped buffer");
		//else create Buffer request and do it
	}



	//Vulkan only method
	void VulkanTransferManager::transferToImage(ImageTransferRequest&& imageTransferRequest, const VkImage& targetImage)
	{
		
		m_imageTransferRequests.push_back(std::move(imageTransferRequest));
		m_vkImages.push_back(targetImage);
		processQueuedImageTransferRequests();

	}

	void VulkanTransferManager::flush()
	{

		processQueuedBufferTransferRequests(true);
		processQueuedImageTransferRequests(true);
	}


}
