#pragma once
#include <vector>
#include "../../igpu_descriptor_set.h"
#include "opengl_descriptor_set_layout.h"

//cleaned
namespace TheEngine::Graphics
{
	class OpenGLDescriptorSet : public IGPUDescriptorSet
	{
	private:
		
		std::vector<DescriptorBinding> m_bindings;
	public:

		OpenGLDescriptorSet();

		virtual void UpdateDescriptors(const std::vector<DescriptorBinding>& bindings) override;

		const std::vector<DescriptorBinding>& GetBindings() const;

		~OpenGLDescriptorSet() override ; // Good practice to use 'override' // need more research
	};
}