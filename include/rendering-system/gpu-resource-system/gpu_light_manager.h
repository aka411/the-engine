#pragma once
#include <vector>
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>


namespace TheEngine::RenderingSystem
{

	//This is still under developement , design constraints and goals are still being thought of

	using GPULightIndex = size_t;// not stable ID, changes, also conside using something that can have invalid also
	using LightId = size_t;//Stable ID ,also use generation
	
	class IRenderDevice;



	struct GPUBufferInfo;
	struct LightData;

	class GPULightManager
	{

	private:




		//GPU index , do i need this ?
		//Or use step allocator with step size
		std::vector<GPUSubAllocationInfo> m_lightIdToGPUAllocationInfo;//Light id to allocation info

		//Light Id to GPU Id
		std::vector<GPULightIndex> m_lightIdToGPULightIndex;//Light id to GPU light index

		std::vector<LightId> m_freeLightIds;// for reusing light Ids


		size_t m_totalLightCount = 0;
		IRenderDevice& m_renderDevice;

	public:

		GPULightManager(IRenderDevice& renderDevice);

	

		LightId addNewLight(const LightData& lightData);//Also uploads to GPU

		void uploadLightDataToGPU(const LightData& lightData);

		//for initial MVP will will just use bool active or not , later we can add more complexity
		void deactivateLight(const LightId lightId); // it will take that light out buffer and replace with last light.
		void activateLight(const LightId LightId, const LightData& lightData);// need reupload



		void deleteLight(const GPULightIndex gpuLightIndex);//this is when a hole occurs update that here itself



		//For renering system

		size_t getTotalLightCount() const;

		GPUBufferInfo& getGPUBufferInfoForLights();
	

	};


}