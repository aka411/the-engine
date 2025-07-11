#pragma once
#include <vector>

#include "../../core/include/types.h"
#include "cpu_texture.h"

namespace TheEngine::Resource
{
	class CPUMaterial : public ICPUResource
	{
	private:
		//Implicit binding points?
		std::vector<CPUTexture> m_textures;// consider pointer or reference
		
		
	public:
		CPUMaterial(const std::vector<CPUTexture>&& textures); 
		CPUResourceType getType() const override;
	};
}