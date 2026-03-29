
namespace TheEngine::RenderingSystem
{



	virtual BufferManager& IRenderDevice::getBufferManager() override
	{
		return m_bufferManager;
	}


	virtual SamplerManager& IRenderDevice::getSamplerManager() override
	{
		return m_samplerManager;
	}

	virtual TextureManager& IRenderDevice::getTextureManager() override
	{
		return m_textureManager;
	}

	virtual ShaderManager& IRenderDevice::getShaderManager() override
	{
		return m_shaderManager;
	}

	virtual TransferManager& IRenderDevice::getTransferManager() override
	{
		return m_transferManager;
	}

	virtual PipelineManager& IRenderDevice::getPipelineManager() override
	{
		return m_pipelineManager;
	}

	virtual FrameBufferManager& IRenderDevice::getFrameBufferManager() override
	{
		return m_framebufferManager;
	}


}