#pragma once
#include <map>
#include "ui/core/ui_core_data_structures.h"
#include "memory-management/gpu_buffer_manager.h"
#include "memory-management/i_gpu_buffer_allocator.h"




namespace TheEngine::UI
{


	class UIVertexBufferManagementSystem
	{
		

	private:

		Memory::GPUBufferManager& m_gpuBufferManager;

	
		std::map<UIVertexFormat,std::unordered_map<UIBufferType, std::unique_ptr<Memory::IGPUBufferAllocator>>> m_formatToNormalVertexBufferSubAllocators;
		std::map<UIVertexFormat, std::unordered_map<UIBufferType,std::unique_ptr<Memory::IGPUBufferAllocator>>> m_formatToTextVertexBufferSubAllocators;

		//std::map<UIBufferType, GPUBufferSubBumpAllocator> m_textVertexBufferAllocators;


		std::unique_ptr<Memory::IGPUBufferAllocator> createNewSSBOAllocator(const UIBufferType uiBufferType);
		std::unique_ptr<Memory::IGPUBufferAllocator> createNewVertexBufferAllocator(const UIBufferType uiBufferType);

	public:

		UIVertexBufferManagementSystem(Memory::GPUBufferManager& gpuBufferManager);

		//get buffer for given vertex format
		size_t uploadVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size);
		size_t uploadTextVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size);


		Memory::GPUBufferInfo getBufferInfoForVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType);
		Memory::GPUBufferInfo getBufferInfoForTextWithVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType);


	};


}