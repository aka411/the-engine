#pragma once
#include <unordered_map>
#include "../igpu_resource.h"
#include <memory>
#include "../../../core/include/types.h"
#include "igpu_resource_manager.h"


namespace TheEngine::Graphics
{
	

	class GPUResourceManager : public IGPUResourceManager
	{

	private:

		std::unordered_map<TheEngine::Core::ResourceHandle,std::unique_ptr<IGPUResource>> m_gpuResourceMap;



	public:

		GPUResourceManager();
			
	
		~GPUResourceManager() = default;
	
		void storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<IGPUResource>&& resource);


		virtual IGPUResource* getResource(const TheEngine::Core::ResourceHandle handle) const override;

		void removeResource(const TheEngine::Core::ResourceHandle handle);
	

	};





}

