#include "../../../include/backends/opengl/opengl_framebuffer.h"

//cleaned but not good
namespace TheEngine::Graphics
{


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferCreateInfo& createInfo, std::vector< GLuint>& fbo)
		: m_openGLCreateInfo(createInfo), m_fbo(fbo)
	{
	}

    uint32_t OpenGLFramebuffer::GetHeight() const
	{
		return  m_openGLCreateInfo.height;
	}
    uint32_t OpenGLFramebuffer::GetWidth() const
    {
        return  m_openGLCreateInfo.width;
    }

	uint32_t OpenGLFramebuffer::GetLayers() const
	{
		return  m_openGLCreateInfo.layers; // Default to 1 layer, can be overridden if needed
	}

	const std::vector<FramebufferAttachment>& OpenGLFramebuffer::GetAttachments() const
	{
		return  m_openGLCreateInfo.attachments;
	}
}