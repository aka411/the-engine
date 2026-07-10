#include <rendering-system/rhi/pipeline_builder.h>




namespace TheEngine::RenderingSystem
{




	PipelineBuilder& PipelineBuilder::setName(const std::string& name)
	{

		m_pipelineName = name;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::setShader(const ShaderHandle shaderHandle)
	{
		m_pipelineStateCreateInfo.shaderHandle = shaderHandle;
		return *this;
	}

	PipelineBuilder& PipelineBuilder::setVertexlayout(const VertexLayout vertexLayout)
	{

		m_pipelineStateCreateInfo.vertexLayout = vertexLayout;

		return *this;

	}

	PipelineBuilder& PipelineBuilder::setPipelineConfig(const PipelineStateConfig pipelineStateConfig)
	{

		m_pipelineStateCreateInfo.pipelineStateConfig = pipelineStateConfig;

		return *this;
	}


	PipelineBuilder& PipelineBuilder::setPipelineLayout(const PipelineLayout pipelineLayout)
	{

		m_pipelineStateCreateInfo.pipelineLayout = pipelineLayout;


		return *this;
	}



	PipelineBuilder& PipelineBuilder::setRenderOutputConfiguration(const RenderOutputConfiguration renderOutputConfiguration)
	{

		m_pipelineStateCreateInfo.renderOutputConfiguration = renderOutputConfiguration;

		return *this;
	}



	const std::string& PipelineBuilder::getPipelineName() const
	{
		return m_pipelineName;
	}


	const PipelineStateCreateInfo& PipelineBuilder::getPipelineStateCreateInfo() const
	{
		return m_pipelineStateCreateInfo;
	}


}