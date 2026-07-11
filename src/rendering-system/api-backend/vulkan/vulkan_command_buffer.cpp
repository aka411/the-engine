#include <assert.h>
#include <unordered_map>

#include <rendering-system/api-backend/vulkan/vulkan_command_buffer.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_resource_resolver.h>
#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/utils/framebuffer_to_vulkan_render_info.h>

#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	//Helpers

	// Barriers
	struct BarrierValue
	{
		VkPipelineStageFlags2 stageMask;
		VkAccessFlags2 accessMask;
		VkImageLayout layout;
	};
	const BarrierValue& getBarrierValue(TextureLayout textureLayout)
	{
		//TODO : ADD TRANSFER LAYOUT also
		static const std::unordered_map<TextureLayout, BarrierValue> table =
		{
			{ TextureLayout::COLOR_ATTACHMENT, {
				VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			}},
			{ TextureLayout::SHADER_READ_ONLY, {
				VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
				VK_ACCESS_2_SHADER_READ_BIT,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			}},
			{ TextureLayout::PRESENT_SRC, {
				VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,//VK_PIPELINE_STAGE_2_NONE
				VK_ACCESS_2_NONE,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			}},
			{ TextureLayout::UNDEFINED, {
				VK_PIPELINE_STAGE_2_NONE,
				//VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
				VK_ACCESS_2_NONE,
				VK_IMAGE_LAYOUT_UNDEFINED
			}}
			,
{ TextureLayout::DEPTH_STENCIL, {
				// Stage where depth testing happens
				VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,

				// We need to both read (for testing) and write (for updating) depth
				VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,

				// Even for D32, this is the standard optimal layout
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			}}
		};
		return table.at(textureLayout);
	}

	bool isDepthFormat(ResourceFormat format)
	{
		switch (format)
		{
		case ResourceFormat::D16_UNORM:
		case ResourceFormat::D32_FLOAT:
		case ResourceFormat::D24_S8_UINT:
		case ResourceFormat::D32_S8_FLOAT:
			return true;
		default:
			return false;
		}
	}

	bool hasStencilComponent(ResourceFormat format)
	{
		switch (format)
		{
		case ResourceFormat::D24_S8_UINT:
		case ResourceFormat::D32_S8_FLOAT:
			return true;
		default:
			return false;
		}
	}











	void VulkanCommandBuffer::prepareDrawState()
	{
		//m_vulkanDescriptorSetState.flushUpdates(m_v);

	}





	VulkanCommandBuffer::VulkanCommandBuffer(VulkanContext& vulkanContext, VkCommandBuffer& vkCommandBuffer, uint32_t vulkanQueueFamilyIndex, VkQueue vulkanTargetQueue, VulkanResourceResolver& vulkanResourceResolver, VulkanDescriptorSetManager& vulkanDescriptorSetManager) :
		ICommandBuffer(RenderingAPI::VULKAN_1_3),
		m_vulkanContext(vulkanContext),
		m_vkCommandBuffer(vkCommandBuffer),
		m_vulkanQueueFamilyIndex(vulkanQueueFamilyIndex),
		m_vulkanTargetQueue(vulkanTargetQueue),
		m_vulkanResourceResolver(vulkanResourceResolver),
		m_vulkanDescriptorSetManager(vulkanDescriptorSetManager)
	{



	}




	VulkanCommandBuffer::~VulkanCommandBuffer()
	{


	}




	// Recording Control
	void VulkanCommandBuffer::beginCommandBuffer()
	{
		vkResetCommandBuffer(m_vkCommandBuffer, 0);


		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;


		if (vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo) != VK_SUCCESS)
		{
			assert(false && "failed to begin recording command buffer!");
		}

		m_isRecording = true;

	}

	void VulkanCommandBuffer::beginRendering(const Framebuffer& framebuffer)
	{
		//This method requires attaching actual image
		VulkanRenderingInfo vulkanRenderingInfo;

		//m_vulkanResourceResolver.getVulkanTexture()
		toVulkanRenderingInfo(vulkanRenderingInfo, framebuffer, m_vulkanResourceResolver);

		vkCmdBeginRendering(m_vkCommandBuffer, &vulkanRenderingInfo.vkRenderingInfo);


	}

	void VulkanCommandBuffer::endRendering()
	{
		//vkCmdEndRenderingKHR(m_vkCommandBuffer);//when not in core like vulakn 1.2
		vkCmdEndRendering(m_vkCommandBuffer);
	}


	void VulkanCommandBuffer::endCommandBuffer()
	{
		if (vkEndCommandBuffer(m_vkCommandBuffer) != VK_SUCCESS)
		{
			assert(false && "failed to record command buffer!");
		}
		m_isRecording = false;
	}




	void VulkanCommandBuffer::reset()
	{
		vkResetCommandBuffer(m_vkCommandBuffer, 0);
	}



	// Pipeline & State
	void VulkanCommandBuffer::bindPipeline(const PipelineHandle& pipelineHandle)
	{
		m_vulkanPipelineObject = m_vulkanResourceResolver.getVulkanPipelineObject(pipelineHandle);

		vkCmdBindPipeline(m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_vulkanPipelineObject.m_vkPipeline);

		m_vulkanDescriptorSetState = VulkanDescriptorSetState(m_vkCommandBuffer, m_vulkanPipelineObject.m_vkPipelineLayout, m_vulkanResourceResolver.getBindlessImageVkDescriptorSet(), m_vulkanPipelineObject.m_vkDescriptorSetLayouts, m_vulkanDescriptorSetManager);


	}


	void VulkanCommandBuffer::setViewPort(float x, float y, float width, float height)
	{

		VkViewport viewport{};
		viewport.x = x;
		viewport.y = y;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(m_vkCommandBuffer, 0, 1, &viewport);

	}


	void VulkanCommandBuffer::setScissor(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height)
	{

		VkRect2D scissor{};
		scissor.offset = { x, y };
		scissor.extent = { width ,height };
		vkCmdSetScissor(m_vkCommandBuffer, 0, 1, &scissor);



	}




	// Resource Binding

	void VulkanCommandBuffer::bindUniformBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range)
	{
		VulkanBufferInfo info = m_vulkanResourceResolver.getVulkanBufferInfo(bufferHandle);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = info.vkBuffer;
		bufferInfo.offset = offset;
		bufferInfo.range = range;

		m_vulkanDescriptorSetState.bindBuffer(set, binding, bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	}



	void VulkanCommandBuffer::bindStorageBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range)
	{
		VulkanBufferInfo info = m_vulkanResourceResolver.getVulkanBufferInfo(bufferHandle);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = info.vkBuffer;
		bufferInfo.offset = offset;
		bufferInfo.range = range;

		m_vulkanDescriptorSetState.bindBuffer(set, binding, bufferInfo, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
	}



	void VulkanCommandBuffer::bindVertexBuffer(int bindingPoint, const BufferHandle& bufferHandle)
	{
		VulkanBufferInfo vulkanBufferInfo = m_vulkanResourceResolver.getVulkanBufferInfo(bufferHandle);
		VkBuffer vertexBuffers[] = { vulkanBufferInfo.vkBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(
			m_vkCommandBuffer,
			bindingPoint,              // firstBinding
			1,              // bindingCount
			vertexBuffers,
			offsets
		);
	}

	void  VulkanCommandBuffer::bindIndexBuffer(const BufferHandle& bufferHandle)
	{

		VulkanBufferInfo vulkanBufferInfo = m_vulkanResourceResolver.getVulkanBufferInfo(bufferHandle);

		vkCmdBindIndexBuffer(
			m_vkCommandBuffer,
			vulkanBufferInfo.vkBuffer,
			0,                        // offset
			VK_INDEX_TYPE_UINT32      // indexType //Dont hardcode?
		);
	}


	void  VulkanCommandBuffer::setPushConstants(const void* data, uint32_t size, uint32_t offset)
	{

		vkCmdPushConstants(
			m_vkCommandBuffer,
			m_vulkanPipelineObject.m_vkPipelineLayout,
			m_vulkanPipelineObject.m_pushConstantStages,
			offset,
			size,
			data
		);

	}



	void VulkanCommandBuffer::insertImageBarrier(const TextureHandle texturehandle, const TextureLayout srcLayout, const TextureLayout dstLayout)
	{

		auto& src = getBarrierValue(srcLayout);
		auto& dst = getBarrierValue(dstLayout);

		// Retrieve the actual Vulkan texture object
		const auto& texture = m_vulkanResourceResolver.getVulkanTexture(texturehandle);

		VkImageMemoryBarrier2 barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
		barrier.pNext = nullptr;

		// Source state
		barrier.srcStageMask = src.stageMask;
		barrier.srcAccessMask = src.accessMask;
		barrier.oldLayout = src.layout;

		// Destination state
		barrier.dstStageMask = dst.stageMask;
		barrier.dstAccessMask = dst.accessMask;
		barrier.newLayout = dst.layout;


		barrier.image = texture.vkImage;


		VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		if (isDepthFormat(texture.metaData.desc.format))
		{
			aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		
			if (hasStencilComponent(texture.metaData.desc.format))
			{
				aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

		barrier.subresourceRange.aspectMask = aspectMask;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

		//Queue family ownership transfer (For MVP we are using UNIFIED Queue)
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;


		VkDependencyInfo dependencyInfo{};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependencyInfo.imageMemoryBarrierCount = 1;
		dependencyInfo.pImageMemoryBarriers = &barrier;


		vkCmdPipelineBarrier2(m_vkCommandBuffer, &dependencyInfo);


	}












	//Draw Commands
	void  VulkanCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset)
	{


		m_vulkanDescriptorSetState.flushUpdates(m_vulkanContext.vkDevice);

		vkCmdDrawIndexed(
			m_vkCommandBuffer,
			indexCount,      // indexCount
			1,      // instanceCount
			firstIndex,      // firstIndex
			vertexOffset,      // vertexOffset (added to each index)
			0       // firstInstance
		);


	}

	void  VulkanCommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex)
	{

		vkCmdDraw(
			m_vkCommandBuffer,
			vertexCount,      // vertexCount
			1,      // instanceCount
			firstVertex,      // firstVertex
			0       // firstInstance
		);
	}






	//Native Methods
	VkCommandBuffer& VulkanCommandBuffer::getVkCommandBuffer()
	{
		return m_vkCommandBuffer;
	}


	uint32_t VulkanCommandBuffer::getVulkanQueueFamilyIndex() const
	{
		return m_vulkanQueueFamilyIndex;

	}


	VkQueue VulkanCommandBuffer::getVulkanTargetQueue() const
	{

		return m_vulkanTargetQueue;;
	}




}