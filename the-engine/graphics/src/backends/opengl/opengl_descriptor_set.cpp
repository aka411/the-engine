
#include "../../../include/backends/opengl/opengl_descriptor_set.h"

//cleaned
namespace TheEngine::Graphics
{

	OpenGLDescriptorSet::OpenGLDescriptorSet()
	{

	}

	// Update the descriptors in the OpenGL descriptor set
	// This function will bind resources to the descriptor set based on the provided bindings
	// For OpenGL, this typically involves binding buffers, textures, and samplers to specific locations
	// in the shader program.
	void OpenGLDescriptorSet::UpdateDescriptors(const std::vector<DescriptorBinding>& bindings)
	{
		m_bindings = bindings;
		//just store the bindings in a vector

	}
	const std::vector<DescriptorBinding>& OpenGLDescriptorSet::GetBindings() const
	{
		 return m_bindings; 
	}
	OpenGLDescriptorSet::~OpenGLDescriptorSet()
	{

	}
}