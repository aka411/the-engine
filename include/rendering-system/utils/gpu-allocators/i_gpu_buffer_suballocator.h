#pragma once

#include <rendering-system/engine_handles.h>


namespace TheEngine::RenderingSystem
{


	struct GPUSubAllocationInfo
	{
		size_t offset;
		size_t size;
		bool isAllocationSuccessful = false;
	};


	//more like abstract class
	class IGPUBufferSubAllocator
	{
	protected:

		const BufferHandle m_bufferHandle;
		const size_t m_bufferSize;
	public:

		IGPUBufferSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize);

		//add message to warn gpu memory leak, cause we still have not added methods to clean up buffers
		virtual ~IGPUBufferSubAllocator() = default;

		BufferHandle getBufferHandle();

		virtual GPUSubAllocationInfo allocate(const size_t size) = 0;
		virtual void deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo) = 0;

	};

}