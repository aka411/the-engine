#include <rendering-system/api-backend/opengl/opengl_render_device.h>
#include <iostream>
#include <rendering-system/api-backend/opengl/utils/opengl_debug.h>

#include <rendering-system/api-backend/opengl/opengl_buffer_manager.h>
#include <rendering-system/api-backend/opengl/opengl_pipeline_manager.h>
#include <rendering-system/api-backend/opengl/opengl_sampler_manager.h>
#include <rendering-system/api-backend/opengl/opengl_texture_manager.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	OpenglRenderDevice::OpenglRenderDevice() :
		IRenderDevice(RenderingAPI::OPENGL_4_6)

	{

		
	m_bufferManager = std::make_unique<OpenglBufferManager>();
	m_pipelineManager = std::make_unique<OpenglPipelineManager>();
	m_samplerManager = std::make_unique<OpenglSamplerManager>();
	m_textureManager = std::make_unique<OpenglTextureManager>();
	m_shaderManager = std::make_unique<OpenglShaderManager>();
	m_transferManager = std::make_unique<OpenglTransferManager>();


	m_framebufferManager = std::make_unique<OpenglFrameBufferManager>();




			GLint flags;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{

				glEnable(GL_DEBUG_OUTPUT);


				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);


				glDebugMessageCallback(DebugMessageCallback, NULL);


				glDebugMessageControl(
					GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
					0, NULL, GL_FALSE
				);

				std::cout << "OpenGL Debug Output successfully enabled." << std::endl;
			}
		
	}




	OpenglFramebufferManager& OpenglRenderDevice::getFramebufferManager()
	{
		return m_framebufferManager;
	}







}