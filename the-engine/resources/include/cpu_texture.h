#pragma once

#include "../../graphics/include/igpu_texture.h"
#include "cpu_resource.h"

namespace TheEngine::Resource
{
	class CPUTexture : public CPUResource
	{
	public:
		std::vector<char> data; // Raw bytes of pixel data (e.g., RGB, RGBA, compressed block data)
		int width;              // Width of the texture (at mip level 0)
		int height;             // Height of the texture (at mip level 0)
		int depth;              // Depth of the texture (for 3D textures, 1 for 2D)
		TheEngine::Graphics::TextureFormat format; // Pixel format (e.g., RGBA8_UNORM, DXT1)
		TheEngine::Graphics::TextureType type;     // Type of texture (e.g., TEXTURE_2D, TEXTURE_CUBE, TEXTURE_3D)
		uint32_t mipLevels;     // Number of mipmap levels present in 'data'
		uint32_t arrayLayers;   // Number of array layers (for texture arrays or cubemaps)
	public:
		CPUResourceType getType() const override { return CPUResourceType::TEXTURE; }
	};
}