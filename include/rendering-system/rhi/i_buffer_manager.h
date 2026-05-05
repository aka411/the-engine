#pragma once
#include <vector>
#include <assert.h>
#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/gpu_buffer_data_types.h>



namespace TheEngine::RenderingSystem
{






	class IBufferManager
	{

	protected:

	
		std::vector<BufferMetadata> m_bufferRegistry;
		std::vector<uint32_t> m_freeIndices;

	public:
		IBufferManager() = default;
		virtual ~IBufferManager() = default;

		virtual const BufferHandle createBuffer(const BufferCreateInfo& bufferCreateInfo) = 0;

		BufferMetadata getBufferMetadata(const BufferHandle& bufferHandle)
		{
			assert(m_bufferRegistry.size() > bufferHandle.id && "IGPUBufferManager : Invalid handle");

			return m_bufferRegistry[bufferHandle.id];

		}


		virtual void destroyBuffer(const BufferHandle& bufferHandle) = 0;


	};



}