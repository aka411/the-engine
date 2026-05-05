#pragma once
#include <memory>
#include <engine/engine_core_data_types.h>


namespace TheEngine::RenderingSystem
{

	class IBufferManager;
	class IPipelineManager;
	class ISamplerManager;
	class ITextureManager;
	class IShaderManager;
	class ITransferManager;
	class IPresentationSystem;

	class IPipelineBuilder;

	class IRenderDevice
	{

	protected:
	
		std::string GPU_NAME;

		const RenderingAPI m_renderingAPI;

		std::unique_ptr<IBufferManager> m_bufferManager;
		std::unique_ptr<ITransferManager> m_transferManager;

		std::unique_ptr<IPipelineManager> m_pipelineManager;
		std::unique_ptr<ISamplerManager> m_samplerManager;
		std::unique_ptr<ITextureManager> m_textureManager;
		std::unique_ptr<IShaderManager> m_shaderManager;
	

		std::unique_ptr<IPresentationSystem> m_presentationSystem;


	public:

		IRenderDevice(const RenderingAPI renderingAPI);

		virtual ~IRenderDevice();


		//no need to make them virtual i guess
		virtual IBufferManager& getBufferManager();


		virtual ISamplerManager& getSamplerManager();
		virtual ITextureManager& getTextureManager();
		virtual IShaderManager& getShaderManager();
		virtual ITransferManager& getTransferManager();
		virtual IPipelineManager& getPipelineManager();


		virtual IPresentationSystem& getPresentationSystem();

		virtual void beginFrame(const uint32_t frameIndex) = 0;

		virtual std::unique_ptr<IPipelineBuilder> getPipelineBuilder() = 0;
	};





}