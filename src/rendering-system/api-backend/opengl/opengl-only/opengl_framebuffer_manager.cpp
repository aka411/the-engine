#include <rendering-system/api-backend/opengl/opengl-only/opengl_framebuffer_manager.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	static uint64_t RenderOutputConfigurationHash(const RenderOutputConfiguration& renderOutputConfiguration)
	{

	

	}
	

	OpenglFramebufferManager::OpenglFramebufferManager()
	{

	}


	OpenglFramebufferManager::~OpenglFramebufferManager()
	{

	}


	GLuint OpenglFramebufferManager::getOrCreateFramebuffer(const RenderOutputConfiguration& renderOutputConfiguration)
	{

		//check if we alread have

		if (m_framebuffers.find(RenderOutputConfigurationHash(renderOutputConfiguration)) != m_framebuffers.end())
		{
			return m_framebuffers.at(RenderOutputConfigurationHash(renderOutputConfiguration));
		}



		GLuint fbo;
		glCreateFramebuffers(1, &fbo);

		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex, 0);
		glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTex, 0);

		if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//failed
		}

		m_framebuffers[RenderOutputConfigurationHash(renderOutputConfiguration)] = fbo;

		return fbo;
	}



    GLuint OpenglFramebufferManager::getNativeHandle(const RenderOutputConfiguration& renderOutputConfiguration)
    {

		return m_framebuffers.at(RenderOutputConfigurationHash(renderOutputConfiguration));

    }



}