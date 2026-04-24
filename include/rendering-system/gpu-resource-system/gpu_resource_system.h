#pragma once
#include <memory>

namespace TheEngine::RenderingSystem
{
	class IBufferManager;
	class ITransferManager;

	class GPUMeshSystem;
	class GPUMaterialManager;
	class GPULightManager;
	class GPUAnimationManager;

	class GPUTextureManager;
	class ITextureManager;

	class IRenderDevice;


	class GPUResourceSystem
	{

	private:


		
		IRenderDevice& m_renderDevice;

		IBufferManager& m_bufferManager;
		ITransferManager& m_transferManager;

		//OWNER 
		std::unique_ptr<GPUMeshSystem> m_gpuMeshSystem;
		std::unique_ptr<GPUMaterialManager> m_gpuMaterialManager;
		//std::unique_ptr<GPULightManager> m_gpuLightManager;
		std::unique_ptr<GPUAnimationManager> m_gpuAnimationManager;

		

	

	public:

		GPUResourceSystem(IRenderDevice& renderDevice);
		~GPUResourceSystem();


		GPUMeshSystem& getGPUMeshSystem();
		GPUMaterialManager& getGPUMaterialManager();

		//GPULightManager& getGPULightManager();

		GPUAnimationManager& getGPUAnimationManager();

		ITextureManager& getTextureManager();


	};


}