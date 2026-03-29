#pragma once 
#include "glad/glad.h"
#include <cstdint>

namespace TheEngine::RenderingSystem
{

	enum class GPUMemoryArchitecture
	{
		UMA,//Uinfied Memory Architecture
		DISCRETE

	};


	/* BUFFER SYSTEM */
	enum class BufferUsageType
	{
		UNKNOWN,
		STATIC,
		DYNAMIC,
		STREAMING

	};



	enum class BufferType : uint8_t
	{
		UNKNOWN,
		VERTEX,
		INDEX,
		UNIFORM,
		STORAGE,
		INDIRECT,
		STAGING // Its not a type in opengl, but it is there in vulkan i think also could indicate that buffer is for staging 
	};

	//for UMA Architectures dont think DEVICE_LOCAL has effect
	enum MemoryFlags : uint8_t
	{
		// CPU can read/write this memory, memcpy 
		CPU_VISIBLE = 1 << 0,

		//READ_BIT = 1 << 1;
		//WRITE_BIT =  1 << 2;

		// No need to call 'Flush' after writing; hardware handles it
		COHERENT = 1 << 1,// Only for CPU_VISIBLE
		// Memory stays mapped for the duration of the buffer life
		PERSISTENT_MAPPED = 1 << 2,//Only for CPU_VISIBLE



		// The fastest memory, but CPU cannot touch it directly
		DEVICE_LOCAL = 1 << 3



	};

	inline MemoryFlags operator|(MemoryFlags a, MemoryFlags b)
	{
		return static_cast<MemoryFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}



	//need rework??
	struct GPUBufferCreateInfo
	{
		GPUBufferType gpuBufferType;
		MemoryFlags memoryFlags;
		//other parameters
		size_t size;

	};





	struct GPUSubAllocationInfo
	{
		size_t offset;
		size_t size;
		bool isAllocationSuccessful = false;
	};






	//Need to Redesign 
	struct GPUBufferHandle
	{
		GPUBufferType bufferType = GPUBufferType::UNKNOWN;

		uint32_t gpuBufferHandle = 0;//zero is invalid

		size_t size = 0;//in bytes


	};


}