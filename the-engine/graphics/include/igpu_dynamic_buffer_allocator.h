#pragma once
#include <cstdint>

namespace TheEngine::Graphics
{
	/*
	struct AllocationResult
	{
		void* cpuMappedPointer; // Pointer to the CPU-accessible memory
		size_t gpuOffset;        // Offset within the GPU buffer for binding
	};
	*/




	class IGPUDynamicBufferAllocator //another cool name IGPUStreamingBufferAllocator
	{
	public:
		virtual ~IGPUDynamicBufferAllocator() = default;
		// Allocate a buffer of the specified size and alignment
		virtual void* allocate(size_t size, size_t alignment) = 0;

		// Reset the allocator, freeing all allocated buffers
		virtual void Reset() = 0;
		// Get the total allocated size
		virtual size_t GetTotalAllocatedSize() const = 0;

		// Get the total free size
		virtual size_t GetTotalFreeSize() const = 0;


		virtual uint64_t getBufferHandle() const = 0;

	};
}
