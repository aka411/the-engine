#pragma once

#include "opengl_render_device.h"
#include "opengl_framebuffer.h"
#include "opengl_render_pass.h"
#include "opengl_pipeline_state.h"

//cleanup complete
namespace TheEngine::Graphics
{

    class OpenGLCommandBuffer : public IGPUCommandBuffer
    {
    private:

       OpenGLPipelineState* m_currentPipeline = nullptr;
       OpenGLRenderPass* m_currentRenderPass = nullptr;
       OpenGLFramebuffer* m_currentFramebuffer = nullptr;

	   size_t m_currentSubpassIndex = 0;

    public:
  
         ~OpenGLCommandBuffer() = default;

		 OpenGLCommandBuffer() = default;

         // --- Render Pass Management ---
         virtual void BeginRenderPass(IGPURenderPass* renderPass, IGPUFramebuffer* framebuffer) ;//also consider clear values
         virtual void NextSubpass() ;
         virtual void EndRenderPass();



    };

} // namespace TheEngine::Graphics