


namespace TheEngine::RenderingSystem
{





	Framebuffer::FrameBuffer(uint32_t width, uint32_t height) :
		m_width(width), m_height(height)
	{

	}



	// The "Swap" mechanism
	void Framebuffer::setColorAttachment(uint32_t index, const FramebufferAttachment& attachment)
	{

	}
	void Framebuffer::setDepthStencilAttachment(const FramebufferAttachment& attachment)
	{

	}


	uint32_t Framebuffer::getWidth() const
	{
		return m_width;
	}

	uint32_t Framebuffer::getHeight() const
	{
		return m_height;
	}


};



}