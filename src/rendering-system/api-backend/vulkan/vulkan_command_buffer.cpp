

namespace TheEngine::RenderingSystem::VulkanBackend
{


	VulkanCommandBuffer::VulkanCommandBuffer()

	{

	}




    virtual VulkanCommandBuffer::~VulkanCommandBuffer() override
    {


    }




    // Recording Control
    virtual void VulkanCommandBuffer::beginCommandBuffer() override
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
    }

    virtual void VulkanCommandBuffer::beginRendering(const IFrameBuffer& frameBuffer) override
    {

    }

    virtual void VulkanCommandBuffer::endRendering() override
    {

    }


    virtual void VulkanCommandBuffer::endCommandBuffer() override
    {

    }



    // virtual void execute() = 0; //no need
    virtual void VulkanCommandBuffer::reset() override
    {

    }



    // Pipeline & State
    virtual void VulkanCommandBuffer::bindPipeline(const IPipelineStateObject& pipelineStateObject) override
    {



        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }

    virtual void VulkanCommandBuffer::setViewPort(float x, float y, float width, float height) override
    {

        VkViewport viewport{};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);



        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


    }





    // Resource Binding

    //virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) = 0;
    virtual void VulkanCommandBuffer::bindDescriptorSetWrite(uint32_t setIndex, DescriptorSetHandle set) override
    {

    }



    virtual void VulkanCommandBuffer::bindVertexBuffer(int bindingPoint, GPUBufferInfo gpuBufferInfo) override
    {

    }

    virtual void VulkanCommandBuffer::bindIndexBuffer(GPUBufferInfo gpuBufferInfo) override
    {

    }



    virtual void VulkanCommandBuffer::setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) override
    {

    }




    //Draw Commands
    virtual void VulkanCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) override
    {

    }




    virtual void VulkanCommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex) override
    {

    }




}