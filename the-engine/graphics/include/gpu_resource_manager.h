#pragma once


#include "../../core/include/types.h"
#include <unordered_map>
#include "igpu_resource.h"

#include <memory>

#include "gpu_resource_streamer.h"//consider using interfaces to help unit
//testing by avoiding direct dependence on concrete class like gpu streamer.

namespace TheEngine::Graphics
{
	

	class GPUResourceManager
	{
	private:
		IGPURenderDevice& m_renderDevice;
	private:
		std::unique_ptr<GPUResourceStreamer>  m_GPUResourceStreamer;

	private:

		std::unordered_map<TheEngine::Core::ResourceHandle,std::unique_ptr<IGPUResource>> m_gpuResourceMap;



	public:

		GPUResourceManager(IGPURenderDevice& renderDevice, std::unique_ptr<GPUResourceStreamer>&& gpuResourceStreamer);
			
	
		~GPUResourceManager() = default;



	public:

	
		void storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<IGPUResource>&& resource);


		IGPUResource* getResource(const TheEngine::Core::ResourceHandle handle) const;

		void removeResource(const TheEngine::Core::ResourceHandle handle);
	

	};





}

