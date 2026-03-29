#pragma once


namespace TheEngine::RenderingSystem::OpenGLBackend
{



	class OpenglRenderDevice : public IRenderDevice
	{

	private:


	public:

		OpenglRenderDevice();



		virtual IGPUBufferManager& getBufferManager() override;


		virtual IGPUSamplerManager& getSamplerManager() override;
		virtual IGPUTextureManager& getTextureManager() override;
		virtual IGPUShaderManager& getShaderManager() override;
		virtual IGPUTransferManager& getTransferManager() override;
		virtual IPipelineManager& getPipelineManager() override;

		virtual IFrameBufferManager& getFrameBufferManager() override;//do i need this method

	}




}