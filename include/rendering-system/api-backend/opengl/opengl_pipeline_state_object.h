#pragma once
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>
#include <glad/glad.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{



	class OpenglPipelineStateObject : public IGPUPipeline
	{



	private:

		//we have to create the vaos, etc
		GLuint m_vaoID;
	
	public:


		OpenglPipelineStateObject(PipelineDescriptor& pipelineDescriptor);

		virtual void bind() override;

	};




}

