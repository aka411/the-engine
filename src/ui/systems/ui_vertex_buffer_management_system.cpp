#include "ui/systems/ui_vertex_buffer_management_system.h"
#include "low-level/world_vertex_buffer_management_system.h"
#include "memory-management/gpu_buffer_sub_bump_allocator.h"
#include "memory-management/gpu_buffer_circular_allocator.h"
#include <cstring>



namespace TheEngine::UI
{



	std::unique_ptr <Memory::IGPUBufferAllocator> UIVertexBufferManagementSystem::createNewSSBOAllocator(const UIBufferType uiBufferType)
	{

		Memory::GPUBufferInfo gpuBufferInfo;
		switch (uiBufferType)
		{
		case UIBufferType::STATIC:
		{
			gpuBufferInfo = m_gpuBufferManager.createMappedSSBO(50 * 1024 * 1024, nullptr);//50MiB

			return std::make_unique<Memory::GPUBufferSubBumpAllocator>(gpuBufferInfo);
			break;
		}
		case UIBufferType::DYNAMIC:
		{
			gpuBufferInfo = m_gpuBufferManager.createMappedSSBO(2 * 1024 * 1024, nullptr);//50MiB
			return std::make_unique<Memory::GPUBufferCircularAllocator>(gpuBufferInfo);
			break;
		}
		default:
			assert(false && "Unhandled UIBufferType in UIVertexBufferManagementSystem::createNewSSBOAllocator");
			break;
		}



	}


	std::unique_ptr<Memory::IGPUBufferAllocator> UIVertexBufferManagementSystem::createNewVertexBufferAllocator(const UIBufferType uiBufferType)
	{

		Memory::GPUBufferInfo gpuBufferInfo;
		switch (uiBufferType)
		{
		case UIBufferType::STATIC:
		{
			gpuBufferInfo = m_gpuBufferManager.createMappedVertexBuffer(50 * 1024 * 1024, nullptr);//50MiB

			return std::make_unique<Memory::GPUBufferSubBumpAllocator>(gpuBufferInfo);
			break;
		}
		case UIBufferType::DYNAMIC:
		{
			gpuBufferInfo = m_gpuBufferManager.createMappedVertexBuffer(2 * 1024 * 1024, nullptr);//2 MiB

			return std::make_unique<Memory::GPUBufferCircularAllocator>(gpuBufferInfo);
			break;
		}
		default:
			assert(false && "Unhandled UIBufferType in UIVertexBufferManagementSystem::createNewSSBOAllocator");
			break;
		}


	}


	UIVertexBufferManagementSystem::UIVertexBufferManagementSystem(Memory::GPUBufferManager& gpuBufferManager)
		:
		m_gpuBufferManager(gpuBufferManager)
	{



	}


	size_t UIVertexBufferManagementSystem::uploadVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size)
	{



		auto& bufferTypeMap = m_formatToNormalVertexBufferSubAllocators[uiVertexFormat];


		if (bufferTypeMap.find(uiBufferType) == bufferTypeMap.end())
		{


			bufferTypeMap.emplace(uiBufferType, createNewSSBOAllocator(uiBufferType));

		}





		Memory::IGPUBufferAllocator* iGPUBufferAllocator = bufferTypeMap.at(uiBufferType).get();
		assert(iGPUBufferAllocator && "iGPUBufferAllocator is null in UIVertexBufferManagementSystem::uploadVertexData");

		Memory::AllocationInfo allocationInfo = iGPUBufferAllocator->allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = iGPUBufferAllocator->getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset;
	}



	size_t UIVertexBufferManagementSystem::uploadTextVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size)
	{





		auto& bufferTypeMap = m_formatToTextVertexBufferSubAllocators[uiVertexFormat];


		if (bufferTypeMap.find(uiBufferType) == bufferTypeMap.end())
		{


			bufferTypeMap.emplace(uiBufferType, createNewVertexBufferAllocator(uiBufferType));

		}





		Memory::IGPUBufferAllocator* iGPUBufferAllocator = bufferTypeMap.at(uiBufferType).get();
		assert(iGPUBufferAllocator && "iGPUBufferAllocator is null in UIVertexBufferManagementSystem::uploadTextVertexData");

		Memory::AllocationInfo allocationInfo = iGPUBufferAllocator->allocate(size);
		Memory::GPUBufferInfo gpuBufferInfo = iGPUBufferAllocator->getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset






	}



	Memory::GPUBufferInfo UIVertexBufferManagementSystem::getBufferInfoForVertexFormat(const UIVertexFormat uiVertexFormat, UIBufferType uiBufferType)
	{
		return m_formatToNormalVertexBufferSubAllocators.at(uiVertexFormat).at(uiBufferType).get()->getGPUBufferInfo();

	}



	Memory::GPUBufferInfo UIVertexBufferManagementSystem::getBufferInfoForTextWithVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType)
	{

		return m_formatToTextVertexBufferSubAllocators.at(uiVertexFormat).at(uiBufferType).get()->getGPUBufferInfo();

	}


}