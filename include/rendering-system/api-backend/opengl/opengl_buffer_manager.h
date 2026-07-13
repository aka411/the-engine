#pragma once
#include <vector>

#include <glad.h>

#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/gpu_buffer_data_types.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{
	


	struct OpenglBuffer
	{
		GLuint openglAPIBufferHandle;

		//MemoryFlags memoryFlags;
		void* mappedPtr = nullptr;
		bool isMapped = false;
	};


	class OpenglBufferManager : public IBufferManager
	{

	private:


		//std::vector<GLuint> m_allocatedBuffers;
		std::vector<OpenglBuffer> m_allocatedOpenglBuffers;
	

	public:

		OpenglBufferManager();

		virtual ~OpenglBufferManager() override;


		
		virtual const BufferHandle createBuffer(const BufferCreateInfo& bufferCreateInfo) override;
		virtual void destroyBuffer(const BufferHandle& bufferHandle) override;

		const OpenglBuffer getNativeBuffer(const BufferHandle& bufferHandle);

	

	};

}