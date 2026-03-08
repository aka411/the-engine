#include "light-system/light_system.h"


namespace TheEngine
{



	LightSystem::LightSystem(Memory::GPUBufferManager& gpuBufferManager) 
				: 
		m_gpuBufferManager(gpuBufferManager)
	{

		//just to make sure
		assert(sizeof(Light) % 16 == 0 && "Light struct size must be multiple of 16 bytes for std430 alignment");


		m_lightSSBO = m_gpuBufferManager.createMappedSSBO(
			sizeof(Light) * 100, //initial size for 100 lights
			nullptr
		);

		//add a sample lights 

		Light sampleLight{};
		//(255, 228, 132)/255.0f   ---->  colour of sunlight
		sampleLight.color = glm::vec4(255.0f/255.0f, 228.0f / 255.0f, 132.0f / 255.0f, 1.0f);
		sampleLight.intensity = 1.0f;
		sampleLight.lightType = Light::LightType::Directional;
		sampleLight.direction = glm::vec4(-0.5f, -1.0f, -0.5f, 0.0f);
		//addLight(sampleLight);


		Light sampleLight2{};
		sampleLight2.color = glm::vec4(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f, 1.0f);
		sampleLight2.intensity = 1.0f;
		sampleLight2.lightType = Light::LightType::Directional;
		sampleLight2.direction = glm::vec4(-0.5f, -1.0f, -0.5f, 0.0f);
		addLight(sampleLight2);



		uploadLightsToGPU();
		// normalize(vec3(0.5, -0.8, 0.2));
		//const vec3 LIGHT_COLOR = vec3(light[0].color.xyz);//vec3(1.0, 1.0, 1.0); 

	}

	void LightSystem::addLight(const Light& light)
	{
		m_newLightAdded = true;
		m_lights.push_back(light);
	}

	void LightSystem::uploadLightsToGPU()
	{ 
		if (m_newLightAdded == false) return;
		m_newLightAdded = false;


		void* ptr = m_lightSSBO.mappedPtr;

		const size_t requiredSize = sizeof(Light) * m_lights.size();

		memcpy(ptr, m_lights.data(), requiredSize);
	
	}

	Memory::GPUBufferInfo LightSystem::getLightSSBO() const
	{
		return m_lightSSBO;
	}

}