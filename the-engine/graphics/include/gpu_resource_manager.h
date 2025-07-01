#pragma once

#include "igpu_texture.h"
#include "igpu_buffer.h"
#include "../../core/include/types.h"
#include <unordered_map>

#include "igpu_material.h"
#include <memory>
#include "gpu_mesh.h"
#include "gpu_asset_orchestrator.h"
#include "igpu_render_device.h"



namespace TheEngine::Graphics
{
	

	class GPUResourceManager
	{
	private:
		IGPURenderDevice& m_renderDevice;
	private:
		std::unique_ptr<GPUAssetOrchestrator> m_GPUAssetOrchestrator;

	private:

		std::unordered_map<TheEngine::Core::ResourceHandle,std::unique_ptr<IGPUResource>> m_gpuResourceMap;



	public:

		GPUResourceManager(IGPURenderDevice& renderDevice, std::unique_ptr<GPUAssetOrchestrator> gpuAssetOrchestrator)
			: m_renderDevice(renderDevice), m_GPUAssetOrchestrator(std::move(gpuAssetOrchestrator))
		{
		};

		~GPUResourceManager() = default;



	public:

		template<typename GPUResourceType>
		void storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<GPUResourceType>&& resource);

		// Generic Get Method (returns non-owning raw pointer for modification)
		template<typename GPUResourceType>
		GPUResourceType* getResource(const TheEngine::Core::ResourceHandle handle)const;

		void removeResource(const TheEngine::Core::ResourceHandle handle);
	

	};
}

