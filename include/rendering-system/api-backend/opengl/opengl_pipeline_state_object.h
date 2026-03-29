#pragma once
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>
#include <glad/glad.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{

	//currently under rework

	class OpenglPipelineStateObject : public IPipelineStateObject
	{



	private:

		//we have to create the vaos, etc
		GLuint m_vaoID;
		GLuint m_fbo;
	//A global state of RenderConfig to avoid resetting already set state

	public:


		OpenglPipelineStateObject(const RenderStateConfiguration& renderStateConfiguration);

		virtual void bind() override;

	};




}

