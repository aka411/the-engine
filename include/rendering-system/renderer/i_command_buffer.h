#pragma once

namespace TheEngine::RenderingSystem
{


    class ICommandBuffer
    {



    private:

        const RenderingAPI m_renderingAPI;
        //Handles etc is api dependant
        bool m_isRecording = false;

    public:

        CommandBuffer(const GraphicsAPI& graphicsAPI) :
            m_renderingAPI(renderingAPI)
        {

        };

        virtual ~CommandBuffer() = default;




        // Recording Control
        virtual void beginCommandBuffer() = 0;//Start Recording

        virtual void beginRendering(const IFrameBuffer& frameBuffer) = 0;
        virtual void endRendering() = 0;

        virtual void endCommandBuffer() = 0;//end Recording


       // virtual void execute() = 0; //no need
        virtual void reset() = 0; //might need this maybe


        // Pipeline & State
        virtual void bindPipeline(const IPipelineStateObject& pipelineStateObject) = 0;
        virtual void setViewPort(float x, float y, float width, float height) = 0;




        // Resource Binding

        //virtual void bindDescriptorSet(uint32_t setIndex, DescriptorSetHandle set) = 0;
        virtual void bindDescriptorSetWrite(uint32_t setIndex, DescriptorSetHandle set) = 0;
        //Replaced by above one method
        //virtual void bindSSBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;
       //virtual void bindUBO(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;

        virtual void bindVertexBuffer(int bindingPoint, GPUBufferInfo gpuBufferInfo) = 0;
        virtual void bindIndexBuffer(GPUBufferInfo gpuBufferInfo) = 0;


        virtual void setPushConstants(const void* data, uint32_t size, uint32_t offset = 0) = 0;



        //Draw Commands
        virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex, int32_t vertexOffset) = 0;
        virtual void draw(uint32_t vertexCount, uint32_t firstVertex) = 0;




    }

}