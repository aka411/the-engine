#pragma once
#include <rendering-system/engine_handles.h>
#include <cstdint>
#include <engine/engine_core_data_types.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>


namespace TheEngine::RenderingSystem
{
    class Framebuffer;

    class ICommandBuffer
    {



    protected:

        RenderingAPI m_renderingAPI;

        bool m_isRecording = false;

    public:

        ICommandBuffer(const RenderingAPI& renderingAPI) :
            m_renderingAPI(renderingAPI)
        {

        };

  
        virtual ~ICommandBuffer() = default;

        const bool isRecording() const
        {
            return m_isRecording;
        }





        // Recording Control
        virtual void beginCommandBuffer() = 0;//Start Recording
   
        virtual void beginRendering(const Framebuffer& framebuffer) = 0;
        virtual void endRendering() = 0;

        virtual void endCommandBuffer() = 0;//end Recording


        // virtual void execute() = 0; //no need
        virtual void reset() = 0; //might need this maybe


        // Pipeline & State
      
        virtual void bindPipeline(const PipelineHandle& pipelineHandle) = 0;
        virtual void setViewPort(float x, float y, float width, float height) = 0;
        virtual void setScissor(const int32_t x, const int32_t y, const uint32_t width, const uint32_t height) = 0;



        // Resource Binding

        virtual void bindUniformBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range) = 0;
        virtual void bindStorageBuffer(uint32_t set, uint32_t binding, const BufferHandle& bufferHandle, uint64_t offset, uint64_t range) = 0;


        virtual void bindVertexBuffer(int bindingPoint, const BufferHandle& bufferHandle) = 0;
        virtual void bindIndexBuffer(const BufferHandle& bufferHandle) = 0;


        virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        // Barriers
        virtual void insertImageBarrier(const TextureHandle texturehandle, const TextureLayout srcLayout, const TextureLayout dstLayout) = 0;




        //Draw Commands
        virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) = 0;
        virtual void draw(uint32_t vertexCount, uint32_t firstVertex) = 0;




    };

}