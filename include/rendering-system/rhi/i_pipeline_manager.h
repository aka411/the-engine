#pragma once
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <string>



namespace TheEngine::RenderingSystem
{
	

	class IPipelineManager
	{

	private:




		
	public:
		
		IPipelineManager() = default;
		virtual ~IPipelineManager() = default;


		
		virtual const PipelineHandle createPipeline(const std::string& name, const PipelineStateCreateInfo& pipelineStateCreateInfo) = 0;



		virtual void destroyPipeline(const PipelineHandle& pipelineHandle) = 0;


		//GETTERS
		virtual const PipelineHandle getPipelineHandle(const std::string& pipelineName) const = 0;

	};



}