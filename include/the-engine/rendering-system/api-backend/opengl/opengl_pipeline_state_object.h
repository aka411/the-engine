#pragma once

#include <glad.h>
#include <rendering-system/rhi/gpu_pipeline_data_structures.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{



	class OpenglPipelineStateObject 
	{



	private:


		GLuint m_vaoID;
		GLuint m_fbo;


	public:


		OpenglPipelineStateObject(const PipelineStateConfiguration& pipelineStateConfiguration);

		

	};




}

