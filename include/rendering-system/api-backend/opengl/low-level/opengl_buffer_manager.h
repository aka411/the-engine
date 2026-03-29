#pragma once



namespace TheEngine::RenderingSystem::OpenGLBackend
{
	


	struct OpenglBuffer
	{
		GLuint openglAPIBufferHandle;

		MemoryFlags memoryFlags;
		void* mappedPtr = nullptr;
		bool isMapped = false;
	};


	class OpenglBufferManager : public IGPUBufferManager
	{

	private:


		//std::vector<GLuint> m_allocatedBuffers;
		std::vector<OpenglBuffer> m_allocatedOpenglBuffers;
	

	public:

		OpenglBufferManager();

		virtual ~OpenglBufferManager() override;


		//instead of above methods a single method 
		virtual const GPUBufferHandle createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo) override;
		virtual void destroyBuffer(const GPUBufferHandle& gpuBufferHandle) override;

		const OpenglBuffer getNativeBuffer(const GPUBufferHandle& gpuBufferHandle);

	

	};

}