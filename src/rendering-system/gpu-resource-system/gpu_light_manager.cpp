#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/rhi/i_render_device.h>
#include <glm/ext/vector_float4.hpp>
#include <rendering-system/gpu-resource-system/gpu_light_manager.h>

namespace TheEngine::RenderingSystem
{


	struct GPULightData
	{
		glm::vec4 position;
		glm::vec4 colour;

		glm::vec4 direction;// w is for padding for now, maybe we can  reuse it for something else

		//add more
	};



	GPULightManager::GPULightManager(IRenderDevice& renderDevice) :

		m_renderDevice(renderDevice)

	{



	}






}