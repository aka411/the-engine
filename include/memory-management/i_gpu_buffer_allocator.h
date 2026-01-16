#pragma once
#include "memory-management/memory_system_data_types.h"


namespace TheEngine::Memory
{


	class IGPUBufferAllocator
	{


	public:

		virtual ~IGPUBufferAllocator() = default;

		virtual GPUBufferInfo getGPUBufferInfo() = 0;

		virtual AllocationInfo allocate(const size_t size) = 0;


	};

}
