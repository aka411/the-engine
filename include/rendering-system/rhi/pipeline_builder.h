#pragma once
#include <string>

#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>

namespace TheEngine::RenderingSystem
{


	class PipelineBuilder
	{

	private:

		std::string m_pipelineName;
		PipelineStateCreateInfo m_pipelineStateCreateInfo;

	public:


		 PipelineBuilder() = default;

		 PipelineBuilder& setName(const std::string& name);

		 PipelineBuilder& setShader(const ShaderHandle shaderHandle);
		 PipelineBuilder& setVertexlayout(const VertexLayout vertexLayout);
		 PipelineBuilder& setPipelineConfig(const PipelineStateConfig pipelineConfig);

		 PipelineBuilder& setPipelineLayout(const PipelineLayout pipelineLayout);
		
		 PipelineBuilder& setRenderOutputConfiguration(const RenderOutputConfiguration renderOutputConfiguration);

		 const std::string& getPipelineName() const;
		 const PipelineStateCreateInfo& getPipelineStateCreateInfo() const;

	};



}