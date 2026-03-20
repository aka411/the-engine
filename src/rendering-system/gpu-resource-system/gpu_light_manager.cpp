#include "rendering-system/gpu-resource-system/gpu_light_manager.h"
#include "rendering-system/low-level-gpu-systems/low_level_gpu_system.h"
#include <glm/ext/vector_float4.hpp>


namespace TheEngine::RenderingSystem
{


	struct GPULightData
	{
		glm::vec4 position;
		glm::vec4 colour;

		glm::vec4 direction;// w is for padding for now, maybe we can  reuse it for something else

		//add more
	};



	GPULightManager::GPULightManager(LowLevelGPUSystem& lowLevelGPUSystem):
		m_gpuBufferManager(lowLevelGPUSystem.getGPUBufferManager()),
		m_gpuBufferTransferManager(lowLevelGPUSystem.getGPUBufferTransferManager())

	{



	}






}