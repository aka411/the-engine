#include <rendering-system/api-backend/opengl/opengl_transfer_manager.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{




	OpenGLTransferManager::OpenGLTransferManager(OpenglBufferManager& openglBufferManager) :
		m_openglBufferManager(openglBufferManager)
	{

	}

	OpenGLTransferManager::~OpenGLTransferManager()
	{

	}



	void OpenGLTransferManager::transferToBuffer(BufferTransferRequest&& request)
	{


		if (gpuBufferTransferRequest.targetBuffer.isMapped)
		{
			assert(gpuBufferTransferRequest.targetBuffer.mappedPtr != nullptr && "GPUBuffer created with nullptr mapped pointer and mislabeled as mapped");

			std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(gpuBufferTransferRequest.targetBuffer.mappedPtr);
			std::uintptr_t offsetAddress = baseAddress + gpuBufferTransferRequest.targetOffset;


			assert(baseAddress + gpuBufferTransferRequest.targetBuffer.size >= offsetAddress + gpuBufferTransferRequest.memoryblock.getSize());

			void* finalDest = reinterpret_cast<void*>(offsetAddress);


			memcpy(finalDest, gpuBufferTransferRequest.memoryblock.getData(), gpuBufferTransferRequest.memoryblock.getSize());
		}
		else
		{
			//for MVP we only use mapped type
			assert(false && " GPUBuffer created with wrong flags");
		}



	}


	void OpenGLTransferManager::transferToImage(ImageTransferRequest&& request)
	{



	}



}