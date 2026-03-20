#pragma once

#include <vector>
#include "glad/glad.h"
#include "memory-management/memory_system_data_types.h"


namespace TheEngine::RenderingSystem
{
	





	class GPUBufferManager
	{

	private:


		std::vector<GLuint> m_allocatedBuffers;
		size_t m_totalAllocatedGPUMemory = 0;

	public:

		GPUBufferManager();
		~GPUBufferManager();

		// My graphics card has UMA - Unified Memory Architecture so it makes no difference between device and host memory is what I read
		// So all buffers are created as MAPPED for now
		//GPUBufferInfo createMappedVertexBuffer(const size_t size, void* data);
		//GPUBufferInfo createMappedUBOBuffer(const size_t size, void* data);
		//GPUBufferInfo createMappedIndexBuffer(const size_t size, void* data);
		//GPUBufferInfo createMappedSSBO(const size_t size, void* data);
		//GPUBufferInfo createMappedIBO(const size_t size);//Indirect Buffer Object

		//instead of above methods a single method 
		GPUBufferInfo createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo);

		void deleteBuffer(GPUBufferInfo gpuBufferInfo);

		size_t getTotalAllocatedGPUMemory() const;

	};

}