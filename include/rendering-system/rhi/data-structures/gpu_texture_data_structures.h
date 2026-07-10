#pragma once
#include <cstdint>
#include <cstddef>
#include "gpu_sampler_data_structures.h"
#include <memory>
#include <memory-management/memory_block.h>
#include <rendering-system/rhi/data-structures/gpu_types.h>


namespace TheEngine::RenderingSystem
{





	enum class TextureType
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE_MAP,
		TEXTURE_2D_ARRAY,
		UNKNOWN
	};

	enum  TextureUsageFlags : uint32_t 
	{
		IMAGE_USAGE_NONE = 0,

	
		IMAGE_USAGE_TRANSFER_SRC_BIT = 1 << 0,

		
		IMAGE_USAGE_TRANSFER_DST_BIT = 1 << 1,


		IMAGE_USAGE_SAMPLED_BIT = 1 << 2,


		IMAGE_USAGE_STORAGE_BIT = 1 << 3,


		IMAGE_USAGE_COLOR_ATTACHMENT_BIT = 1 << 4,


		IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 1 << 5,

	
		IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT = 1 << 6,

		
		IMAGE_USAGE_INPUT_ATTACHMENT_BIT = 1 << 7
	};


	inline TextureUsageFlags operator|(TextureUsageFlags a, TextureUsageFlags b) {
		return static_cast<TextureUsageFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}


	inline TextureUsageFlags operator&(TextureUsageFlags a, TextureUsageFlags b) {
		return static_cast<TextureUsageFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
	}


	inline TextureUsageFlags& operator|=(TextureUsageFlags& a, TextureUsageFlags b) {
		a = a | b;
		return a;
	}










	enum class TextureLayout : uint32_t 
	{
		UNDEFINED = 0,           // Maps to VK_IMAGE_LAYOUT_UNDEFINED (Data is discarded/don't care)
		GENERAL,                 // Maps to VK_IMAGE_LAYOUT_GENERAL (Slow but supports everything)
		COLOR_ATTACHMENT,        // Maps to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		DEPTH_STENCIL,           // Maps to VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		SHADER_READ_ONLY,        // Maps to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL (Sampling)
		TRANSFER_SRC,            // Maps to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL (Copy from)
		TRANSFER_DST,            // Maps to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL (Copy to)
		PREINITIALIZED,          // Maps to VK_IMAGE_LAYOUT_PREINITIALIZED (For CPU-visible textures)
		PRESENT_SRC              // Maps to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR (Swapchain display)
	};



	// Shared by both CreateInfo and Metadata to ensure zero duplication.
	struct TextureDescription 
	{
		uint32_t width = 1;
		uint32_t height = 1;
		uint32_t depth = 1;

		uint32_t mipLevelCount = 1;
		uint32_t arrayLayerCount = 1;

		ResourceFormat format = ResourceFormat::RGBA8_UNORM;
		TextureType type = TextureType::TEXTURE_2D;
		SampleCountBit sampleCount = SampleCountBit::SAMPLE_COUNT_1_BIT;
		TextureUsageFlags usageFlags = TextureUsageFlags::IMAGE_USAGE_SAMPLED_BIT | TextureUsageFlags::IMAGE_USAGE_TRANSFER_DST_BIT;
	};



	struct TextureCreateInfo 
	{
		TextureDescription desc{}; // Reusing the same core definition

		// Data Source
		Memory::MemoryBlock memoryBlock;

		// Creation-only settings
		TextureLayout initialLayout = TextureLayout::UNDEFINED;
		bool useSampler = true;
		SamplerCreateInfo samplerCreateInfo;
		bool makeBindless = true;
	};






	// The Metadata (Persistent)
	struct TextureMetadata
	{
		TextureDescription desc{};

		TextureLayout currentLayout = TextureLayout::UNDEFINED;
		uint32_t queueFamilyIndex = 0;

		bool isBindless = false;
		uint64_t bindlessHandle = 0;
	};



}