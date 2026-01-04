#include "utils/file-decoders/image_decoder.h"
#include "stb/stb_image.h"
#include <cassert>

namespace TheEngine::Utils
{
	ImageDecoder::ImageDecoder(Platform& platform) :
		m_platform(platform)

	{




	}


	ImageData ImageDecoder::decodeImageFromFile(const std::string& pathToImageFile)
	{

		FileData fileData = m_platform.readFile(pathToImageFile);
		return decodeImageFromMemory(fileData.data.get(), fileData.size);

	}


	ImageData ImageDecoder::decodeImageFromMemory(const std::byte* imageData, size_t dataSize)
	{
        int width, height, channels;

        
        unsigned char* rawPixels = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc*>(imageData),
            static_cast<int>(dataSize),
            &width, &height, &channels, 0
        );

        if (!rawPixels)
        {
			assert(false && "Failed to decode image from memory.");
           // return {};
        }


        ImageData result;
        result.width = width;
        result.height = height;
        result.channels = channels;

         result.data = std::unique_ptr<std::byte[], void(*)(void*)>(
             reinterpret_cast<std::byte*>(rawPixels),
            [](void* ptr) { stbi_image_free(ptr); }
		);


        return result;

	}

}