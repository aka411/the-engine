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

    public:
  
         ~OpenGLCommandBuffer();






    };

} // namespace TheEngine::Graphics