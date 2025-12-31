#pragma once
#include <memory>
#include <string>
#include "platform.h"
#include <cstddef>

namespace TheEngine::Utils
{
	struct ImageData
	{
		int width = 0;
		int height = 0;
		int channels = 0;

		std::unique_ptr < std::byte[], void(*)(void*) > data = { nullptr, nullptr };
	};

	class ImageDecoder
	{
	private:

		Platform& m_platform;

	public:

		ImageDecoder(Platform& paltform);

		ImageData decodeImageFromFile(const std::string& pathToImageFile);

		
		ImageData decodeImageFromMemory(const std::byte* imageData, size_t dataSize);


	};




}