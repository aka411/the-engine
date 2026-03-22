#include <image_loader.h>
#include <platform/platform.h>
#include <cassert>


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


namespace TheEngine
{


	ImageLoader::ImageLoader(TheEngine::Platform::FileSystem& fileSystem) :
		m_fileSystem(fileSystem)
	{
		stbi_set_flip_vertically_on_load(true);
	}

	RenderingSystem::TextureCreateInfo ImageLoader::loadTextureFile(const TheEngine::Platform::Path& path)
	{
		//use stb to load file and create memory block
	
		using namespace TheEngine::RenderingSystem;

		TheEngine::Platform::File file = m_fileSystem.open(path);



		int width, height, channels;


		unsigned char* stbiRawPixels = stbi_load_from_memory(
			reinterpret_cast<const stbi_uc*>(file.data()),
			static_cast<int>(file.size()),
			&width, &height, &channels, STBI_rgb_alpha
		);

		if (!stbiRawPixels) {

			assert(false && stbi_failure_reason());

			return TheEngine::RenderingSystem::TextureCreateInfo{};
		}



		TheEngine::RenderingSystem::TextureCreateInfo textureCreateInfo;
		// --- Dimensions ---
		textureCreateInfo.width = width;
		textureCreateInfo.height = height;
		textureCreateInfo.depth = 1; // Always 1 for 2D static images
		textureCreateInfo.type = TextureType::TEXTURE_2D;

		textureCreateInfo.memoryBlock = TheEngine::Memory::MemoryBlock(
			reinterpret_cast<std::byte*>(stbiRawPixels),
			width * height * channels
		);

		// --- Format ---
		//Yeah need way to handle alphaless also?
		textureCreateInfo.textureSourcePixelFormat = TextureSourcePixelFormat::RGBA;
		textureCreateInfo.textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;
		textureCreateInfo.internalFormat = TextureInternalFormat::RGBA8; 



		stbi_image_free(stbiRawPixels);

		return textureCreateInfo;


	}


}