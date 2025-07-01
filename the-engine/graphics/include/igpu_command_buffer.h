#pragma once

#include "igpu_pipeline_state.h"
#include "igpu_pipeline_layout.h"


//Need to address Index buffer 
//cleanup complete
namespace TheEngine::Graphics
{



    class IGPUCommandBuffer
    {
    public:
        virtual ~IGPUCommandBuffer() = default;


        // --- Lifecycle ---
        // For command buffer object lifecycle
        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Reset(bool releaseResources) = 0; // Maps to VkResetCommandBuffer/Pool


        // --- Render Pass Management ---
        virtual void BeginRenderPass(IGPURenderPass* renderPass, IGPUFramebuffer* framebuffer) = 0;//also consider clear values
        virtual void NextSubpass() = 0;
        virtual void EndRenderPass() = 0;


        // --- State Setting ---
                // Dynamic State commands
        virtual void SetViewport(const Viewport& viewport) = 0;

        
        // --- Resource Binding ---//

        virtual void BindPipelineState(IGPUPipelineState* pipelineState) = 0;
        virtual void BindDescriptorSets(const IGPUPipelineLayout* pipelineLayout,uint32_t firstSet, const std::vector<IGPUDescriptorSet*>& descriptorSets) = 0;
		
        //Buffer binding
        virtual void BindVertexBuffers(uint32_t firstBinding, const std::vector<IGPUBuffer*>& buffers, const std::vector<uint64_t>& offsets) = 0;
        virtual void BindIndexBuffer(IGPUBuffer* buffer, uint64_t offset, IndexType  indexType) = 0; // IndexType for 16-bit or 32-bit indices




        // --- Drawing Commands ---//
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
 

    };

}