#pragma once
#include <memory>

namespace TheEngine::RenderingSystem
{
	class GPUBufferManager;
	class GPUBufferTransferManager;

	class GPUMeshSystem;
	class GPUMaterialManager;
	class GPULightManager;
	class GPUAnimationManager;

	class GPUTextureManager;

	class LowLevelGPUSystem;



	class GPUResourceSystem
	{

	private:


		LowLevelGPUSystem& m_lowLevelGPUSystem;

		GPUBufferManager& m_gpuBufferManager;
		GPUBufferTransferManager& m_gpuBufferTransferManager;

		//OWNER 
		std::unique_ptr<GPUMeshSystem> m_gpuMeshSystem;
		std::unique_ptr<GPUMaterialManager> m_gpuMaterialManager;
		std::unique_ptr<GPULightManager> m_gpuLightManager;
		std::unique_ptr<GPUAnimationManager> m_gpuAnimationManager;

	

	public:

		GPUResourceSystem(LowLevelGPUSystem& lowLevelGPUSystem);
		~GPUResourceSystem();

		GPUMeshSystem& getGPUMeshSystem();
		GPUMaterialManager& getGPUMaterialManager();

		GPULightManager& getGPULightManager();

		GPUAnimationManager& getGPUAnimationManager();

		GPUTextureManager& getGPUTextureManager();


	};


}