
#include "rendering-system/low-level-gpu-systems/gpu_buffer_manager.h"
#include <rendering-system/api-backend/opengl/opengl_buffer_helper.h>




namespace TheEngine::RenderingSystem
{




	GPUBufferManager::GPUBufferManager()
	{



	}

	GPUBufferManager::~GPUBufferManager()
	{
		if (m_allocatedBuffers.size() > 0)
		{
			glDeleteBuffers(m_allocatedBuffers.size(), m_allocatedBuffers.data());
		}
	}

	GPUBufferInfo GPUBufferManager::createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo)
	{


		//API specific code goes here
		//TODO : Implement this

		GLuint gpuBuffer;

		GLenum openglBufferType = TheEngine::RenderingSystem::OpenGLBackend::toGLenum(gpuBufferCreateInfo.gpuBufferType);

		glGenBuffers(1, &gpuBuffer);

		glBindBuffer(openglBufferType, gpuBuffer);

		GLbitfield flags = TheEngine::RenderingSystem::OpenGLBackend::toGLbitfield(gpuBufferCreateInfo.memoryFlags);

		glBufferStorage(openglBufferType, gpuBufferCreateInfo.size, nullptr, flags);

		GPUBufferInfo gpuBufferInfo;
		gpuBufferInfo.bufferType = gpuBufferCreateInfo.gpuBufferType;
		gpuBufferInfo.bufferHandle = gpuBuffer;
		gpuBufferInfo.size = gpuBufferCreateInfo.size;
		gpuBufferInfo.mappedPtr = nullptr;
		
		m_totalAllocatedGPUMemory += gpuBufferCreateInfo.size;

		
		if (gpuBufferCreateInfo.memoryFlags && MemoryFlags::CPU_VISIBLE)
		{
			gpuBufferInfo.mappedPtr = glMapBufferRange(openglBufferType, 0, gpuBufferCreateInfo.size, flags);
			gpuBufferInfo.isMapped = true;
		}


		glBindBuffer(openglBufferType, 0);


		m_allocatedBuffers.push_back(gpuBuffer);

		



		return gpuBufferInfo;


	}

	void GPUBufferManager::deleteBuffer(GPUBufferInfo gpuBufferInfo)
	{




	}

	size_t GPUBufferManager::getTotalAllocatedGPUMemory() const
	{
		return m_totalAllocatedGPUMemory;
	}

}