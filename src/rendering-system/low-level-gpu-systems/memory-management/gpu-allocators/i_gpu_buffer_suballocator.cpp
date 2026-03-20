#include "rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.h"


namespace TheEngine::RenderingSystem
{

	IGPUBufferSubAllocator::IGPUBufferSubAllocator(GPUBufferInfo gpuBufferInfo):
		m_gpuBufferInfo(gpuBufferInfo)

	{

	}



	GPUBufferInfo IGPUBufferSubAllocator::getGPUBufferInfo()
	{
		return m_gpuBufferInfo;
	}




}