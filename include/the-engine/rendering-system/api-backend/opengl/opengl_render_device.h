#pragma once
#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/api-backend/opengl/opengl-only/opengl_framebuffer_manager.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{


	class OpenglRenderDevice : public IRenderDevice
	{

	private:

		OpenglFramebufferManager m_framebufferManager;

	public:

		OpenglRenderDevice();



		virtual IBufferManager& getBufferManager() override;


		virtual ISamplerManager& getSamplerManager() override;
		virtual ITextureManager& getTextureManager() override;
		virtual IShaderManager& getShaderManager() override;
		virtual ITransferManager& getTransferManager() override;
		virtual IPipelineManager& getPipelineManager() override;



		//OPENGL SPECIFIC METHODS
		OpenglFramebufferManager& getFramebufferManager();

	};




}