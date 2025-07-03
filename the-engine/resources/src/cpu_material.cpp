#include "../include/cpu_material.h"

namespace TheEngine::Resource
{



	CPUMaterial::CPUMaterial(const std::vector<CPUTexture>&& textures) : m_textures(textures)
	{
	}

	CPUResourceType CPUMaterial::getType() const
	{
		return CPUResourceType::MATERIAL;
	}



}