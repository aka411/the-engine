#pragma once
#include <vector>


namespace TheEngine::RnederingSystem
{






	class IBufferManager
	{

	protected:

		// Every backend (Vulkan/GL) needs to know the metadata
		std::vector<BufferMetadata> m_bufferRegistry;

		// A way to track "Free" indices for handles
		std::vector<uint32_t> m_freeIndices;

	public:
		IBufferManager();
		virtual ~IBufferManager() = default;

		virtual const BufferHandle createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo) = 0;

		BufferMetadata getBufferMetadata(const BufferHandle& bufferHandle)
		{
			assert(m_bufferRegistry.size() < bufferHandle.id && "IGPUBufferManager : Invalid handle");

			return m_bufferRegistry[bufferHandle.id];

		}


		virtual void destroyBuffer(const BufferHandle& bufferHandle) = 0;


	};



}