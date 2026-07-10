#pragma once 
#include <cstdint>

namespace TheEngine::RenderingSystem
{

	enum class GPUMemoryArchitecture
	{
		UMA,		//Unified Memory Architecture
		DISCRETE

	};






	enum BufferUsage : uint32_t
	{

		TRANSFER_SRC_BIT = 1 << 0, // Staging source
		TRANSFER_DST_BIT = 1 << 1, // Staging destination
		UNIFORM_BUFFER_BIT = 1 << 2, // UBO
		STORAGE_BUFFER_BIT = 1 << 3, // SSBO 
		VERTEX_BUFFER_BIT = 1 << 4,
		INDEX_BUFFER_BIT = 1 << 5,
		INDIRECT_BUFFER_BIT = 1 << 6,
		DEVICE_ADDRESS_BIT = 1 << 7 // Vulkan 1.3 / GL_ARB_buffer_addr
	};

	inline BufferUsage operator|(BufferUsage a, BufferUsage b)
	{
		return static_cast<BufferUsage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}




	enum MemoryPropertyFlags : uint32_t // Changed to uint32 for easier masking
	{
		DEVICE_LOCAL_BIT = 1 << 0,
		HOST_VISIBLE_BIT = 1 << 1, //GL_MAP_READ / WRITE	Memory can be mapped to a CPU pointer using vkMapMemory.
		HOST_COHERENT_BIT = 1 << 2, //GL_MAP_COHERENT_BIT	CPU and GPU see each other's writes automatically. No need for vkFlushMappedMemoryRanges.
		HOST_CACHED_BIT = 1 << 3
	};




	struct BufferCreateInfo
	{
		size_t size;
		uint32_t bufferUsage;
		uint32_t memoryPropertyFlags;
	};


	struct BufferMetadata
	{
		size_t size;
		uint32_t bufferUsage;
		uint32_t memoryPropertyFlags;

		bool isMapped = false;
		void* mappedPtr = nullptr;
	};


}