#include "../include/cpu_texture.h"

namespace TheEngine::Resource
{

	CPUTexture::CPUTexture(const std::vector<uint8_t>& data, const TheEngine::Graphics::TextureCreateInfo& textureCreateInfo)
		:  m_data(data), m_textureCreateInfo(textureCreateInfo)
	{
	}

	CPUResourceType CPUTexture::getType() const
	{
		return CPUResourceType::TEXTURE;
	}


}