#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanCommandBuffer : public ICommandBuffer
	{

	private:

        VkCommandBuffer m_commandBuffer

	public:


		VulkanCommandBuffer();

        virtual ~VulkanCommandBuffer() override;


        // Recording Control
        virtual void beginCommandBuffer() override;

        virtual void beginRendering(const IFrameBuffer& frameBuffer) override;
        virtual void endRendering() override;

        virtual void endCommandBuffer() override;//end Recording


        // virtual void execute() = 0; //no need
        virtual void reset() override; //might need this maybe


        // Pipeline & State
        virtual void bindPipeline(const IPipelineStateObject& pipelineStateObject) override;
        virtual void setViewPort(float x, float y, float width, float height) override;




        // Resource Binding

        //virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) = 0;
        virtual void bindDescriptorSetWrite(uint32_t setIndex, DescriptorSetHandle set) override;


        virtual void bindVertexBuffer(int bindingPoint, GPUBufferInfo gpuBufferInfo) override;
        virtual void bindIndexBuffer(GPUBufferInfo gpuBufferInfo) override;


        virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) override;



        //Draw Commands
        virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) override;
        virtual void draw(uint32_t vertexCount, uint32_t firstVertex) override;








	};






}

