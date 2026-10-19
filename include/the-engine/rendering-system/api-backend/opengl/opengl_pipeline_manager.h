#pragma once
#include <vector>
#include <memory>

#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/engine_handles.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	class OpenglPipelineManager : public IPipelineManager
	{

	private:

		std::vector<PipelineStateConfiguration> m_openglPipelineCache;

	public:

		OpenglPipelineManager();
		virtual ~OpenglPipelineManager() override;


		virtual const PipelineHandle createPipelineStateObject(const PipelineStateConfiguration& pipelineStateConfiguration) override;





	};


}