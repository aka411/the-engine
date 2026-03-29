//Include all opengl defs


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	OpenglRenderDevice::OpenglRenderDevice() :
		IRenderDevice(RenderingAPI::OPENGL_4_6)

		m_iGPUBufferManager(std::make_unique<OpenglBufferManager>()),
		m_iPipelineManager(std::make_unique<OpenglPipelineManager>()),
		m_iGPUSamplerManager(std::make_unique<OpenglSamplerManager>()),
		m_iGPUTextureManager(std::make_unique<OpenglTextureManager>()),
		m_iGPUShaderManager(std::make_unique<OpenglShaderManager>()),
		m_iGPUTransferManager(std::make_unique<OpenglTransferManager>()),
		m_iframebufferManager(std::make_unique<OpenglFrameBufferManager>())


	{





	}












}