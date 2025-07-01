#pragma once
#include <vector>
#include "../../igpu_pipeline_layout.h"
#include "../../igpu_descriptor_set_layout.h"
#include "opengl_descriptor_set_layout.h"

namespace TheEngine::Graphics
{
	class OpenGLPipelineLayout : public IGPUPipelineLayout
	{
	private:
		std::vector<OpenGLDescriptorSetLayout*> m_descriptorSetLayouts; // Store the descriptor set layouts
		
	public:

		OpenGLPipelineLayout(const std::vector<OpenGLDescriptorSetLayout*>& descriptorSetLayouts);
	
		~OpenGLPipelineLayout() override;

		const std::vector<OpenGLDescriptorSetLayout>& GetDescriptorSetLayouts() const ;
		

	};
}