#pragma once
#include <vector>
#include "memory-management/gpu_buffer_manager.h"
#include "memory-management/gpu_buffer_per_frame_allocator.h"

#include <cstddef>

namespace TheEngine
{

	class RenderCommandBufferManager
	{
	private:

		struct AllocatorGroup
		{
			Memory::GPUBufferPerFrameAllocator gpuBufferPerFrameAllocatorForArrayCommands;
			Memory::GPUBufferPerFrameAllocator gpuBufferPerFrameAllocatorForIndexedCommands;
		};


		const int MAX_NUM_OF_FRAMES = 2;
		Memory::GPUBufferManager& m_gpuBufferManager;

		std::vector<AllocatorGroup> m_perFrameAllocatorGroups;

		unsigned int m_currentFrameIndex = 0;

	public:
		RenderCommandBufferManager(Memory::GPUBufferManager& gpuBufferManager);

		Memory::GPUBufferInfo getArrayGPUBufferForThisFrame();
		Memory::GPUBufferInfo getIndexedGPUBufferForThisFrame();

		size_t uploadArrayCommandsForCurrentFrame(std::byte* data, const size_t size);
		size_t uploadIndexedCommandsForCurrentFrame(std::byte* data, const size_t size);

		//To be only called by rendersystem at start
		void nextFrame();

	};

}