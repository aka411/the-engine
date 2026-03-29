#pragma once


namespace TheEngine::RnederingSystem
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

		const GPUBufferHandle targetBufferHandle;
		const TransferPriority transferPriority;

		const size_t targetOffset;



		BufferTransferRequest(
			Memory::MemoryBlock memoryblock,
			const GPUBufferHandle& targetBufferHandle,
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
		Memory::MemoryBlock memoryblock;

	};


	class ITransferManager
	{

	private:



	public:

		ITransferManager() = default;

		virtual ~ITransferManager() = default;


		// Path A: The Linear copy (Buffers)
		virtual void transferToBuffer(BufferTransferRequest&& request) = 0;

		// Path B: The Tiled/Formatted copy (Images/Textures)
		virtual void transferToImage(ImageTransferRequest&& request) = 0;


	};



}