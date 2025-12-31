#pragma once
#include <unordered_map>
#include "rendering_system_data_types.h"
#include "memory-management/gpu_buffer_sub_bump_allocator.h"
#include "memory-management/gpu_buffer_manager.h"


namespace TheEngine
{
	class WorldVertexBufferManagementSystem
	{


	private:

		Memory::GPUBufferManager m_gpuBuffermanager;

		std::unordered_map<VertexFormat, Memory::GPUBufferSubBumpAllocator> m_formatToVertexBufferSubAllocators;
		std::unordered_map<IndexType, Memory::GPUBufferSubBumpAllocator> m_formatToIndexBufferSubAllocators;

		void createNewVertexBufferForFormat(const VertexFormat vertexFormat);
		void createNewIndexBufferForIndexType(const IndexType indexType);

	public:

		WorldVertexBufferManagementSystem(Memory::GPUBufferManager& gpuBufferManager);

		//get buffer for given vertex format
		size_t uploadVertexData(const VertexFormat vertexFormat, std::byte* data, const size_t size);
		size_t uploadIndexData(const IndexType indexType, std::byte* data, const size_t size);

		Memory::GPUBufferInfo getBufferInfoForVertexFormat(const VertexFormat vertexFormat);
		Memory::GPUBufferInfo getBufferInfoForIndexType(const IndexType indextype);


	};


}