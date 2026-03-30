#pragma once
#include <map>

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	//need to map fbo attahment requirements to GLuintFBO


	class OpenglFrameBufferManager 
	{


	private:

		std::map<RenderOutputConfigurationHash, GLuint> m_frabuffers;

	public:

		OpenglFrameBufferManager();
		~OpenglFrameBufferManager();

		//I was thinking of letting this manager set the state and fbo
		void setState(const Framebuffer& framebuffer);//API specific method


	
		GLuint getNativeHandle(const RenderOutputConfiguration& renderOutputConfiguration);

	};





}