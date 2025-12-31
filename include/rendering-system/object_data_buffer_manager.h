#pragma once
#include "memory-management/gpu_buffer_manager.h"
#include "memory-management/gpu_buffer_per_frame_allocator.h"

#include  <cstddef>


namespace TheEngine
{
	class ObjectDataBufferManager
	{

	private:

		const int MAX_NUM_OF_FRAMES = 2;

		Memory::GPUBufferManager& m_gpuBufferManager;

		std::vector<Memory::GPUBufferPerFrameAllocator> m_gpuBufferPerFrameAllocators;

		unsigned int m_currentFrameIndex = 0;

	public:

		ObjectDataBufferManager(Memory::GPUBufferManager& gpuBufferManager);


		Memory::GPUBufferInfo getGPUBufferForThisFrame();
		size_t uploadDataForCurrentFrame(std::byte* data, const size_t size);

		//To be only called by rendersystem at start
		void nextFrame();
	};

}