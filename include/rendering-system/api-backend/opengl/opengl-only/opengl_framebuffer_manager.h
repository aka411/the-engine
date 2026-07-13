#pragma once
#include <map>

#include <glad/glad.h>

#include <rendering-system/rhi/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/framebuffer.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	//need to map fbo attahment requirements to GLuintFBO


	class OpenglFramebufferManager 
	{


	private:

		std::map<RenderOutputConfigurationHash, GLuint> m_framebuffers;

	public:

		OpenglFramebufferManager();
		~OpenglFramebufferManager();


		GLuint getOrCreateFramebuffer(const RenderOutputConfiguration& renderOutputConfiguration);

	
		GLuint getNativeHandle(const RenderOutputConfiguration& renderOutputConfiguration);

	};





}