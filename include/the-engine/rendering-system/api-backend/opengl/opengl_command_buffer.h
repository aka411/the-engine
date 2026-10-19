#pragma once
#include <rendering-system/rhi/i_command_buffer.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{





    class OpenglCommandBuffer : public ICommandBuffer
    {


    private:



    public:

        OpenglCommandBuffer();


        virtual ~OpenglCommandBuffer() = default;


        // Recording Control
        virtual void beginCommandBuffer() = 0;

        virtual void beginRendering(const Framebuffer& framebuffer) = 0;
        virtual void endRendering() = 0;

        virtual void endCommandBuffer() = 0;


       
        virtual void reset() = 0; 


        // Pipeline & State
        virtual void bindPipeline(const PipelineHandle& pipelineHandle) = 0;
        virtual void setViewPort(float x, float y, float width, float height) = 0;




        // Resource Binding

        //virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) = 0;
        virtual void bindDescriptorSetWrite(uint32_t setIndex, DescriptorSetWrite descriptorSetWrite) = 0;


        virtual void bindVertexBuffer(int bindingPoint, const BufferHandle& bufferHandle) = 0;
        virtual void bindIndexBuffer(const BufferHandle& bufferHandle) = 0;


        virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) = 0;



        //Draw Commands
        virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) = 0;
        virtual void draw(uint32_t vertexCount, uint32_t firstVertex) = 0;



    };

}