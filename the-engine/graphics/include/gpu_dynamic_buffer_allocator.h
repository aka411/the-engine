#pragma once
#include "igpu_dynamic_buffer_allocator.h"


namespace TheEngine::Graphics
{

	class GPUDynamicBufferAllocator : public IGPUDynamicBufferAllocator
	{
	private:

		IGPUBUffer& m_gpuBuffer; // The GPU buffer to allocate from

		size_t m_totalAllocatedSize = 0; // Total size allocated from the GPU buffer
		size_t m_totalFreeSize = 0; // Total free size in the GPU buffer
		size_t m_currentOffset = 0; // Current offset in the GPU buffer for allocation
		size_t m_bufferSize = 0; // Size of the GPU buffer

	public:

		~GPUDynamicBufferAllocator() override = default;
		GPUDynamicBufferAllocator();

		virtual void* allocate(size_t size, size_t alignment) override;


	};
}