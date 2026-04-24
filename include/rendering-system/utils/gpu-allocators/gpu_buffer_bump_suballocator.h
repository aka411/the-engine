#pragma once 
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>


namespace TheEngine::RenderingSystem
{

	class GPUBufferBumpSubAllocator : public IGPUBufferSubAllocator
	{

	private:

		size_t m_currentOffset = 0;

	public:

		GPUBufferBumpSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;


		/**
	 * @brief Allocates and ensures the start offset is a multiple of 'alignment'.
	 * * @note If the current offset is not aligned, this method will insert
	 * empty padding bytes to reach the next valid boundary.
	 * * @warning Assumes the buffer's base address is also aligned.
	 */
		GPUSubAllocationInfo allocateAligned(const size_t size, const size_t alignment);

		virtual void deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo) override;

		void reset();
	};

}