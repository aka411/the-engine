#pragma once
#include <volk.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_state.h>
#include <rendering-system/api-backend/vulkan/vulkan_pipeline_manager.h>




namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanResourceResolver;
	class VulkanContext;

	class VulkanCommandBuffer : public ICommandBuffer
	{

	private:

		VulkanContext& m_vulkanContext;

		VkCommandBuffer m_vkCommandBuffer = VK_NULL_HANDLE;

		uint32_t m_vulkanQueueFamilyIndex;
		VkQueue m_vulkanTargetQueue;





		VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;

		VulkanResourceResolver& m_vulkanResourceResolver;
		VulkanDescriptorSetManager& m_vulkanDescriptorSetManager;

		//State Tracking 
		VulkanPipelineObject  m_vulkanPipelineObject;
		VulkanDescriptorSetState m_vulkanDescriptorSetState;


		void prepareDrawState();

	public:


		VulkanCommandBuffer(VulkanContext& vulkanContext, VkCommandBuffer& vkCommandBuffer, uint32_t vulkanQueueFamilyIndex, VkQueue vulkanTargetQueue, VulkanResourceResolver& vulkanResourceResolver, VulkanDescriptorSetManager& vulkanDescriptorSetManager);

		virtual ~VulkanCommandBuffer() override;



		VulkanCommandBuffer(VulkanCommandBuffer&&) noexcept = default;
		VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) noexcept = default;






		// Recording Control
		virtual void beginCommandBuffer() override;

		virtual void beginRendering(const Framebuffer& framebuffer) override;

		virtual void endRendering() override;

		virtual void endCommandBuffer() override;//end Recording



		virtual void reset() override;


		// Pipeline & State
		virtual void bindPipeline(const PipelineHandle& pipelineHandle) override;
		virtual void setViewPort(float x, float y, float width, float height) override;
		virtual void setScissor(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) override;



		// Resource Binding
		virtual void bindUniformBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range) override;
		virtual void bindStorageBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range) override;

		virtual void bindVertexBuffer(int bindingPoint, const BufferHandle& bufferHandle) override;
		virtual void bindIndexBuffer(const BufferHandle& bufferHandle) override;


		virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) override;


		// Barriers
		virtual void insertImageBarrier(const TextureHandle texturehandle, const TextureLayout srcLayout, const TextureLayout dstLayout) override;





		//Draw Commands
		virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) override;
		virtual void draw(uint32_t vertexCount, uint32_t firstVertex) override;





		//Native Methods

		VkCommandBuffer& getVkCommandBuffer();

		uint32_t getVulkanQueueFamilyIndex() const;
		VkQueue getVulkanTargetQueue() const;

	};






}

