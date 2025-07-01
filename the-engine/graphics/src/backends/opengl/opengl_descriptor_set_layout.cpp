#include "../../../include/backends/opengl/opengl_descriptor_set_layout.h"

//cleaned
namespace TheEngine::Graphics
{

	OpenGLDescriptorSetLayout::OpenGLDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& layoutBindings)
		:m_layoutBindings(layoutBindings)
	{

	}

	OpenGLDescriptorSetLayout::~OpenGLDescriptorSetLayout()
	{
	}


}