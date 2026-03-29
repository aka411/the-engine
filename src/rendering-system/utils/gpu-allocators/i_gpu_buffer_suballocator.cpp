


namespace TheEngine::RenderingSystem
{

	IGPUBufferSubAllocator::IGPUBufferSubAllocator(GPUBufferHandle gpuBufferHandle) :
		m_gpuBufferHandle(gpuBufferHandle)

	{

	}



	GPUBufferHandle IGPUBufferSubAllocator::getGPUBufferHandle();
	{
		return m_gpuBufferHandle;
	}




}