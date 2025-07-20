#include "opengl_command_buffer.h"

//cleanup complete
namespace TheEngine::Graphics
{
 


	void OpenGLCommandBuffer::BeginRenderPass(IGPURenderPass* renderPass, IGPUFramebuffer* framebuffer)
	{

		m_currentRenderPass = static_cast<OpenGLRenderPass*>(renderPass);
		m_currentFramebuffer = static_cast<OpenGLFramebuffer*>(framebuffer);
		if (m_currentRenderPass && m_currentFramebuffer)
		{
			m_currentSubpassIndex = 0;
			return:
		}
		else
		{
			throw std::runtime_error("Invalid render pass or framebuffer");
		}
	}




	void OpenGLCommandBuffer::NextSubpass()
	{


	}

	void OpenGLCommandBuffer::EndRenderPass()
	{



	}










} // namespace TheEngine::Graphics