#pragma once
#include <vector>
#include <memory>
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	class OpenglPipelineManager : public IPipelineManager
	{

	private:

		std::vector<std::unique_ptr<IGPUPipeline>> m_gpuPipelines;

	public:

		OpenglPipelineManager();
		~OpenglPipelineManager();



		virtual PipelineHandle createPipeline(PipelineDescriptor& pipelineDescriptor) override;

		IGPUPipeline& getGPUPipelineState(const PipelineHandle pipelineHandle);

	};


}