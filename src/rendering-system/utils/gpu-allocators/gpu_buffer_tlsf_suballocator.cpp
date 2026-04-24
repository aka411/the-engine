#include <rendering-system/utils/gpu-allocators/gpu_buffer_tlsf_suballocator.h>


namespace TheEngine::RenderingSystem
{



	GPUBufferTLSFSubAllocator::GPUBufferTLSFSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize):
		IGPUBufferSubAllocator(bufferHandle, bufferSize)

	{


	}

	GPUBufferTLSFSubAllocator::~GPUBufferTLSFSubAllocator()
	{

	}

	GPUSubAllocationInfo GPUBufferTLSFSubAllocator::allocate(const size_t size)
	{
		return GPUSubAllocationInfo{.isAllocationSuccessful = false};
	}


    void TheEngine::RenderingSystem::GPUBufferTLSFSubAllocator::deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo)
    {




    }

}