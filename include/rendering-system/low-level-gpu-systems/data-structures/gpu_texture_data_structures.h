#pragma once
#include <cstdint>
#include <cstddef>
#include "gpu_sampler_data_structures.h"
#include <functional>
#include <memory>
#include <memory-management/memory_block.h>


namespace TheEngine::RenderingSystem
{


	/*****  TEXTURE SYSTEM *******/




	enum class TextureSourcePixelFormat
	{
		UNKNOWN,
		R,       
		RG,       
		RGB,    
		RGBA,   
		BGRA    
	};



	// TextureSourceComponentType: Defines the bit size/type of each component in the CPU data.
	enum class TextureSourceComponentType
	{
		UNKNOWN,
		UNSIGNED_BYTE, // GL_UNSIGNED_BYTE (8-bit per channel)
		FLOAT,         // GL_FLOAT (32-bit float per channel)
		HALF_FLOAT     // GL_HALF_FLOAT (16-bit float per channel)
	};




	enum class TextureType
	{
		TEXTURE_2D,         
		TEXTURE_3D,       
		TEXTURE_CUBE_MAP,   
		TEXTURE_2D_ARRAY,   
		UNKNOWN
	};


	//TODO : Maybe consider RGB then precision enum as 8 bit, 16 bit, 32 bit, and float as a separate enum, this way we can have more flexibility in the future if we want to add more formats like compressed formats etc.
	// TextureInternalFormat: Defines how the texture data is stored on the GPU, including the number of channels and their bit depth.
	enum class TextureInternalFormat
	{
		// 8-bit Unsigned Integer Formats
		R8, RG8, RGB8, RGBA8,

		// 16-bit Half-Float Formats
		R16F, RG16F, RGB16F, RGBA16F,

		//  32-bit Full-Float Formats
		R32F, RG32F, RGB32F, RGBA32F,

		// Depth/Stencil formats
		DEPTH_COMPONENT16, DEPTH_COMPONENT24, DEPTH_COMPONENT32F,
		DEPTH_STENCIL,
		UNKNOWN
	};







	struct TextureCreateInfo
	{

		TextureType type = TextureType::UNKNOWN;

		// --- Data Source ---
		//std::unique_ptr<std::byte[], std::function<void(void*)>> data;//Repalce with Memory block
		Memory::MemoryBlock memoryBlock;


		// --- Dimensions ---
		int width = 0;
		int height = 0;
		int depth = 1; // Default to 1 for 2D textures

		//--- Sampler Setting ---

		SamplerSetting samplerSetting;


		// --- Format & Type ---
		TextureInternalFormat internalFormat = TextureInternalFormat::UNKNOWN; // GPU side, 

		TextureSourcePixelFormat textureSourcePixelFormat = TextureSourcePixelFormat::RGBA;
		TextureSourceComponentType textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;
	};


	struct TextureInfo
	{
		TextureType type = TextureType::UNKNOWN;

		uint64_t residentHandle = 0;// invalid value

		int width = 0;
		int height = 0;
		int depth = 0; // For 3D or Array Textures

		TextureInternalFormat internalFormat = TextureInternalFormat::UNKNOWN;
		SamplerSetting samplerSettings;

		bool hasMipmaps = false;
	};






	struct RenderTargetCreateInfo
	{
		TextureType type = TextureType::UNKNOWN;

		int width = 0;
		int height = 0;
		int depth = 0; // For 3D or Array Textures

		TextureInternalFormat internalFormat = TextureInternalFormat::UNKNOWN;
	
	};




}