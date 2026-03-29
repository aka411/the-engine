#pragma once
#include <memory>

namespace TheEngine::RenderingSystem
{

	class IBufferManager;
	class IPipelineManager;
	class ISamplerManager;
	class ITextureManager;
	class IShaderManager;
	class ITransferManager;
	class IFrameBufferManager;


	class IRenderDevice
	{

	protected:
	

		RenderingAPI m_renderingAPI;

		std::unique_ptr<IBufferManager> m_bufferManager;
		std::unique_ptr<IPipelineManager> m_pipelineManager;
		std::unique_ptr<ISamplerManager> m_samplerManager;
		std::unique_ptr<ITextureManager> m_textureManager;
		std::unique_ptr<IShaderManager> m_shaderManager;
		std::unique_ptr<ITransferManager> m_transferManager;
		std::unique_ptr<IFrameBufferManager> m_framebufferManager;



	public:

		IRenderDevice(const RenderingAPI renderingAPI) :
			m_renderingAPI(renderingAPI)
		{

		}

		virtual ~IRenderDevice()= default;


		//no need to make them virtual i guess
		virtual BufferManager& getBufferManager();


		virtual SamplerManager& getSamplerManager();
		virtual TextureManager& getTextureManager();
		virtual ShaderManager& getShaderManager();
		virtual TransferManager& getTransferManager();
		virtual PipelineManager& getPipelineManager();

		virtual FrameBufferManager& getFrameBufferManager();//do i need this method
	};





}