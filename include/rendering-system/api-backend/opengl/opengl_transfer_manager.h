#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include <rendering-system/rhi/i_transfer_manager.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{






	//TO DO : add funtionality to transfer data from one gpu buffer to another gpubuffer,
	/*
	struct GPUToGPUBufferTransferRequest
	{
	GPUBufferInfo sourceBuffer;
	GPUBufferInfo targetBuffer;

	size_t dataSize;

	size_t sourceOffset = 0;
	size_t targetOffset = 0;


	}
	*/


	class OpenglBufferManager;

	class OpenglTransferManager : public ITransferManager
	{
		//Owns a staging buffer that is used to transfer data to target GPU buffer.

	private:

		bool m_transferInProgress = false;

		OpenglBufferManager& m_openglBufferManager;


		std::vector<BufferTransferRequest> m_transferQueue;



	public:

		OpenglTransferManager(OpenglBufferManager& openglBufferManager);

		virtual ~OpenglTransferManager() override;



		
		virtual void transferToBuffer(BufferTransferRequest&& request) override;

		
		virtual void transferToImage(ImageTransferRequest&& request) override;






	};




}