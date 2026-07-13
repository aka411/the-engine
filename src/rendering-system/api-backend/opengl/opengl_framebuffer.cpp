#include <cstdint>

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	void OpenglFramebufferManager::setState(const Framebuffer& framebuffer)
	{


		if (index >= m_colorAttachments.size()) m_colorAttachments.resize(index + 1);
		m_colorAttachments[index] = attachment;


		glNamedFramebufferTexture(m_fboHandle, GL_COLOR_ATTACHMENT0 + index,
			(GLuint)attachment.texture.apiHandle, attachment.mipLevel);


		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < m_colorAttachments.size(); ++i) 
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glNamedFramebufferDrawBuffers(m_fboHandle, (GLsizei)drawBuffers.size(), drawBuffers.data());
	}

	void OpenglFramebuffer::setDepthStencilAttachment(const FramebufferAttachment& attachment)
	{
		m_depthStencilAttachment = attachment;
		m_hasDepthStencil = true;
		glNamedFramebufferTexture(m_fboHandle, GL_DEPTH_STENCIL_ATTACHMENT,
			(GLuint)attachment.texture.apiHandle, attachment.mipLevel);
	}



	OpenglFramebuffer::OpenglFramebuffer(uint32_t width, uint32_t height) :
		IFramebuffer(width, height)
	{
		glCreateFramebuffers(1, &m_fboHandle);
	}



	virtual OpenglFramebuffer::~OpenglFrameBuffer() override
	{
		if (m_fboHandle) glDeleteFramebuffers(1, &m_fboHandle);
	}


	virtual void OpenglFramebuffer::setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) override
	{

		if (index >= m_colorAttachments.size())
		{
			m_colorAttachments.resize(index + 1);
		}
		m_colorAttachments[index] = attachment;


		glNamedFramebufferTexture(m_fboHandle, GL_COLOR_ATTACHMENT0 + index,
			(GLuint)attachment.texture.apiHandle, attachment.mipLevel);


		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < m_colorAttachments.size(); ++i) 
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glNamedFramebufferDrawBuffers(m_fboHandle, (GLsizei)drawBuffers.size(), drawBuffers.data());
	}

	virtual void OpenglFramebuffer::setDepthStencilAttachment(const FramebufferAttachment& attachment) override
	{
		m_depthStencilAttachment = attachment;
		m_hasDepthStencil = true;
		glNamedFramebufferTexture(m_fboHandle, GL_DEPTH_STENCIL_ATTACHMENT,
			(GLuint)attachment.texture.apiHandle, attachment.mipLevel);
	}




	virtual uint32_t OpenglFramebuffer::getNativeHandle() const override
	{
		return m_fboHandle;
	}



}