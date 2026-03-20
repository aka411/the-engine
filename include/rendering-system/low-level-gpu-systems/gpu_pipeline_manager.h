#pragma once
#include <vector>
#include <memory>
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>


namespace TheEngine::RenderingSystem
{


	class GPUPipelineManager
	{

	private:

		std::vector<std::unique_ptr<IGPUPipeline>> m_gpuPipelines;

	public:

		GPUPipelineManager();
		~GPUPipelineManager();

		PipelineHandle createPipeline(PipelineDescriptor& pipelineDescriptor);

		IGPUPipeline& getGPUPipelineState(const PipelineHandle pipelineHandle);

	};


}