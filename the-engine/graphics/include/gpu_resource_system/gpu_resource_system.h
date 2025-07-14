#pragma once
#include "igpu_resource_system.h"
#include "igpu_resource_manager.h"
#include "../../../resources/include/icpu_resource_manager.h"

#include "converter/igpu_resource_factory.h"


namespace TheEngine::Graphics
{

	class GPUResourceSystem : IGPUResourceSystem
	{
	private:

		IGPUResourceManager& m_gpuResourceManager;
		TheEngine::Resource::ICPUResourceManager& m_cpuResourceManager;

		IGPUResourceFactory& m_gpuResourceFactory;


	public:
		GPUResourceSystem(IGPUResourceManager& m_gpuResourceManager, TheEngine::Resource::ICPUResourceManager& m_cpuResourceManager, IGPUResourceFactory& m_gpuResourceFactory);
		virtual IGPUResource* getGPUResource(const TheEngine::Core::ResourceHandle handle)  override;

	};
}
