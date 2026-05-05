#pragma once
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <string>
#include <rendering-system/rhi/data-structures/vertex_layout.h>

namespace TheEngine::RenderingSystem
{


	class IPipelineBuilder
	{

	private:


	public:


		IPipelineBuilder() = default;

		virtual IPipelineBuilder& setName(const std::string& name) = 0;

		virtual IPipelineBuilder& setShader(const ShaderHandle shaderHandle) = 0;//or Id
		virtual IPipelineBuilder& setVertexlayout(const VertexLayout vertexLayout) = 0;//from USER or even JSON
		virtual IPipelineBuilder& setPipelineConfig(const PipelineStateConfig pipelineConfig) = 0;//we get this from pipeline manager

		virtual IPipelineBuilder& setPipelineLayout(const PipelineLayout pipelineLayout) = 0;//from USER or JSON
		
		virtual IPipelineBuilder& setRenderOutputConfiguration(const RenderOutputConfiguration renderOutputConfiguration) = 0;//from USER or JSON

		//Returns pipeline ID
		virtual PipelineHandle build() = 0;

	};



}