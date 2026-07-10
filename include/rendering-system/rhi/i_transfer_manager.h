#pragma once
#include <rendering-system/engine_handles.h>
#include <memory-management/memory_block.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>


namespace TheEngine::RenderingSystem
{



	//CPU TO GPU  Buffer Transfer
	//GPU TO GPU Buffer Transfer
	//CPU To Image Buffer Transfer
	enum class TransferPriority
	{
		LOW,
		MEDIUM,
		HIGH

	};

	struct BufferTransferRequest
	{
		Memory::MemoryBlock memoryblock;

		const BufferHandle targetBufferHandle;
		const TransferPriority transferPriority;

		const size_t targetOffset;



		BufferTransferRequest(
			Memory::MemoryBlock&& memoryblock,
			const BufferHandle& targetBufferHandle,
			const size_t targetOffset,
			const TransferPriority transferPriority
		)
			:
			memoryblock(std::move(memoryblock)),
			targetBufferHandle(targetBufferHandle),
			targetOffset(targetOffset),
			transferPriority(transferPriority)
		{
		}
	};


	struct ImageTransferRequest
	{
		Memory::MemoryBlock memoryBlock;
		TextureDescription textureDescription;

		uint32_t destinationMipLevel = 0;
		uint32_t destinationBaseArrayLayer = 0;
		uint32_t destinationLayerCount = 1;

		ImageTransferRequest(
			Memory::MemoryBlock&& memoryBlock,
			TextureDescription& textureDescription
		) :
			memoryBlock(std::move(memoryBlock)),
			textureDescription(textureDescription)
	
		{

		}
	};

	class IBufferManager;
	class ITransferManager
	{

	private:

	
		IBufferManager& m_bufferManager;

	public:

		
		ITransferManager(IBufferManager& bufferManager) :
			m_bufferManager(bufferManager)
		{

		};

		virtual ~ITransferManager() = default;


		
		virtual void transferToBuffer(BufferTransferRequest&& request) = 0;

		virtual void quickTransfer(const std::byte* srcDataPtr, const size_t bytesToCopy, const BufferHandle targetBufferHandle, const size_t targetOffset) = 0;
		

		//virtual void transferToImage(ImageTransferRequest&& request) = 0;
		virtual void flush() = 0;

	};



}