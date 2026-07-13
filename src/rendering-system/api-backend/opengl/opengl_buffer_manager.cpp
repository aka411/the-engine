#include <rendering-system/api-backend/opengl/opengl_buffer_manager.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	OpenglBufferManager::OpenglBufferManager()
	{



	}


	OpenglBufferManager::~OpenglBufferManager()
	{
		if (m_allocatedBuffers.size() > 0)
		{
			glDeleteBuffers(m_allocatedBuffers.size(), m_allocatedBuffers.data());
		}
	}


	const BufferHandle OpenglBufferManager::createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo)
	{


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

	void OpenglBufferManager::destroyBuffer(const BufferHandle& bufferHandle)
	{




	}


	const OpenglBuffer OpenglBufferManager::getNativeBuffer(const GPUBufferHandle& gpuBufferHandle)
	{


	}


}