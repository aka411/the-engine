#pragma once 
#include "glad/glad.h"

namespace TheEngine::Memory
{




	/* BUFFER SYSTEM */
	enum class BufferUsageType
	{
		UNKNOWN,
		STATIC,
		DYNAMIC,
		STREAMING

	};

	enum BufferType
	{
		VERTEX,
		INDEX,
		SSBO,
		UBO,
		UNKNOWN
	};

	struct AllocationInfo
	{
		size_t offset = 0;

	};


	struct GPUBufferInfo
	{
		BufferType bufferType = BufferType::UNKNOWN;

		GLuint bufferHandle;

		//size in bytes
		size_t size = 0;

		void* mappedPtr = nullptr;

		bool isMapped = false;

	};


}