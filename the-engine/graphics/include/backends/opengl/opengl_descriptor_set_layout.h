#pragma once
#include <vector>
#include "../../../include/igpu_descriptor_set.h"

//cleaned
namespace TheEngine::Graphics
{
	class OpenGLDescriptorSetLayout : public IGPUDescriptorSetLayout
	{
	private:
		const std::vector<DescriptorSetLayoutBinding> m_layoutBindings; // Store the bindings for this layout

	public:
		OpenGLDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& layoutBindings);
			
	
		~OpenGLDescriptorSetLayout();

	}
}