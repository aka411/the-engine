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

		// Generic Get Method (returns non-owning raw pointer for modification)
		template<typename GPUResourceType>
		GPUResourceType* getResource(const TheEngine::Core::ResourceHandle handle)const;

		void removeResource(const TheEngine::Core::ResourceHandle handle);
	

	};





	//for now templated method ,consider other design approach later
	template<typename GPUResourceType>
	GPUResourceType* TheEngine::Graphics::GPUResourceManager::getResource(const TheEngine::Core::ResourceHandle handle)const
	{

		auto it = m_gpuResourceMap.find(handle);
		if (it != m_gpuResourceMap.end())
		{
			IGPUResource* baseResource = it->second.get();
			if (baseResource == nullptr) return nullptr;//this is not needed ,but for now let it be there
			GPUResourceType* desiredResource = dynamic_cast<GPUResourceType*>(baseResource);//this dynamic cast might be a bottleneck,its in a hot path
			return desiredResource; // Returns nullptr if the cast fails
		}
		return nullptr;
	}




}

