#pragma once
#include <vector>


namespace TheEngine::Resource
{
	class CPUTexture : public ICPUResource
	{
	public:
		const std::vector<uint8_t> m_data; // Raw bytes of pixel data (e.g., RGB, RGBA, compressed block data)
		const  TheEngine::Graphics::TextureCreateInfo m_textureCreateInfo; // Metadata about the texture (format, dimensions, etc.)
		
	public:
		CPUTexture(const std::vector<uint8_t>& data,const TheEngine::Graphics::TextureCreateInfo& textureCreateInfo);
		CPUResourceType getType() const override;
	};
}