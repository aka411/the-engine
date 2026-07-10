#include <utils/image_loader.h>
#include <platform/platform.h>
#include <cassert>


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>



namespace TheEngine
{


	ImageLoader::ImageLoader(TheEngine::Platform::FileSystem& fileSystem) :
		m_fileSystem(fileSystem)
	{
		stbi_set_flip_vertically_on_load(true);
	}


	TheEngine::RenderingSystem::TextureCreateInfo ImageLoader::loadTextureFile(const TheEngine::Platform::Path& path)
	{
		
	
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
	
		textureCreateInfo.desc.width = width;
		textureCreateInfo.desc.height = height;
		textureCreateInfo.desc.depth = 1; // Need more thought here
		textureCreateInfo.desc.type = TextureType::TEXTURE_2D;


		textureCreateInfo.memoryBlock = TheEngine::Memory::MemoryBlock(
			reinterpret_cast<std::byte*>(stbiRawPixels),
			width * height * channels
		);

		textureCreateInfo.desc.format = ResourceFormat::RGBA8_UNORM;//TODO : Don't hardcode
		textureCreateInfo.desc.usageFlags = TextureUsageFlags::IMAGE_USAGE_SAMPLED_BIT | TextureUsageFlags::IMAGE_USAGE_TRANSFER_DST_BIT;



		stbi_image_free(stbiRawPixels);

		return textureCreateInfo;


	}


}