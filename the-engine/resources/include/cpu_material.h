#pragma once
#include "cpu_resource.h"
#include <vector>

#include "../../core/include/types.h"
#include "cpu_texture.h"

namespace TheEngine::Resource
{
	class CPUMaterial : public CPUResource
	{
	private:
		//Implicit binding points?
		std::vector<CPUTexture> m_textures;// consider pointer or reference
		
		
	public:
		CPUMaterial(const std::vector<CPUTexture>&& textures); 
		CPUResourceType getType() const override;
	};
}