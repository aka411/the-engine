#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>


namespace TheEngine::RenderingSystem
{

	IGPUBufferSubAllocator::IGPUBufferSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize) :
		m_bufferHandle(bufferHandle),
		m_bufferSize(bufferSize)

	{

	}



	BufferHandle IGPUBufferSubAllocator::getBufferHandle()
	{
		return m_bufferHandle;
	}




}