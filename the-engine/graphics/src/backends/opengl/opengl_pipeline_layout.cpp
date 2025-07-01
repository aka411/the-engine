#include "../../the-engine/the-engine/graphics/include/backends/opengl/opengl_pipeline_layout.h"




namespace TheEngine::Graphics
{








	OpenGLPipelineLayout::OpenGLPipelineLayout(const std::vector<OpenGLDescriptorSetLayout*>& descriptorSetLayouts)
		: m_descriptorSetLayouts(descriptorSetLayouts)
	{
	}

	OpenGLPipelineLayout::~OpenGLPipelineLayout()
	{
	}

	const std::vector<IGPUDescriptorSetLayout*>& TheEngine::Graphics::OpenGLPipelineLayout::GetDescriptorSetLayouts() const
	{
		return m_descriptorSetLayouts;
	}

	const std::vector<PushConstantRange>& OpenGLPipelineLayout::GetPushConstantRanges() const
	{
		return m_pushConstantRanges;
	}

}