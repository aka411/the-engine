#include "low-level/world_vertex_buffer_management_system.h"
#include <cstring>

namespace TheEngine
{

	

	WorldVertexBufferManagementSystem::WorldVertexBufferManagementSystem(Memory::GPUBufferManager& gpuBufferManager) 
		: 
		m_gpuBuffermanager(gpuBufferManager)
	{


	}

	void WorldVertexBufferManagementSystem::createNewVertexBufferForFormat(const VertexFormat vertexFormat)
	{

		Memory::GPUBufferInfo gpuBufferInfo = m_gpuBuffermanager.createMappedVertexBuffer(250 * 1024 * 1024, nullptr);//250MiB

		Memory::GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);

		m_formatToVertexBufferSubAllocators.emplace(vertexFormat,
			std::move(gpuBufferSubBumpAllocator)
		);


	}


	void WorldVertexBufferManagementSystem::createNewIndexBufferForIndexType(const IndexType indexType)
	{

		Memory::GPUBufferInfo gpuBufferInfo = m_gpuBuffermanager.createMappedIndexBuffer(200 * 1024 * 1024, nullptr);//200MiB

		Memory::GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);

		m_formatToIndexBufferSubAllocators.emplace(indexType,
			std::move(gpuBufferSubBumpAllocator)
		);



	}

	size_t WorldVertexBufferManagementSystem::uploadVertexData(const VertexFormat vertexFormat, std::byte* data, const size_t size)
	{


		const auto& it = m_formatToVertexBufferSubAllocators.find(vertexFormat);

		if (it == m_formatToVertexBufferSubAllocators.end())
		{
			createNewVertexBufferForFormat(vertexFormat);
		}

		Memory::GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToVertexBufferSubAllocators.at(vertexFormat);


		Memory::AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset

	}


	size_t WorldVertexBufferManagementSystem::uploadIndexData(const IndexType indexType, std::byte* data, const size_t size)
	{

		const auto& it = m_formatToIndexBufferSubAllocators.find(indexType);

		if (it == m_formatToIndexBufferSubAllocators.end())
		{
			createNewIndexBufferForIndexType(indexType);
		}


		Memory::GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToIndexBufferSubAllocators.at(indexType);

		Memory::AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset

	}



	Memory::GPUBufferInfo WorldVertexBufferManagementSystem::getBufferInfoForVertexFormat(const VertexFormat vertexFormat)
	{

		return m_formatToVertexBufferSubAllocators.at(vertexFormat).getGPUBufferInfo();

	}

	Memory::GPUBufferInfo WorldVertexBufferManagementSystem::getBufferInfoForIndexType(const IndexType indextype)
	{

		return m_formatToIndexBufferSubAllocators.at(indextype).getGPUBufferInfo();

	}


}