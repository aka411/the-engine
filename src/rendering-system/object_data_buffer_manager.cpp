#include "rendering-system/object_data_buffer_manager.h"
#include <cstring>

namespace TheEngine
{

	ObjectDataBufferManager::ObjectDataBufferManager(Memory::GPUBufferManager& gpuBufferManager):
		m_gpuBufferManager(gpuBufferManager)
	{
		for (int i = 0; i < MAX_NUM_OF_FRAMES; ++i)
		{
			//1 MiB
			m_gpuBufferPerFrameAllocators.push_back(Memory::GPUBufferPerFrameAllocator(gpuBufferManager.createMappedSSBO(1024 * 1024, nullptr)));
		}

	}

	Memory::GPUBufferInfo ObjectDataBufferManager::getGPUBufferForThisFrame()
	{


		return m_gpuBufferPerFrameAllocators[m_currentFrameIndex].getGPUBufferInfo();
		
	}



	size_t ObjectDataBufferManager::uploadDataForCurrentFrame(std::byte* data, const size_t size)
	{




		Memory::GPUBufferPerFrameAllocator& gpuBufferPerFrameAllocator = m_gpuBufferPerFrameAllocators[m_currentFrameIndex];


		Memory::AllocationInfo allocationInfo = gpuBufferPerFrameAllocator.allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = gpuBufferPerFrameAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset


	}


	void ObjectDataBufferManager::nextFrame()
	{
		
		++m_currentFrameIndex;
		m_currentFrameIndex = m_currentFrameIndex % MAX_NUM_OF_FRAMES;
		m_gpuBufferPerFrameAllocators[m_currentFrameIndex].reset();
	}


}
