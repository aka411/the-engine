#pragma once
#include <vector>
#include "memory-management/gpu_buffer_manager.h"
#include <glm/ext/vector_float4.hpp>


namespace TheEngine
{

	/***Design ***
	
	Maintains a list of all lights in the scene.
	Lights will be send to SSBO as needed.

	***************/


	//This goes to GPU
	struct Light
	{
		
		glm::vec4 position;//for point and spotlight
		glm::vec4 color;
		glm::vec4 direction; //for directional and spotlight
		float intensity;// for all lights, units -> 
		uint8_t padding1[4]; //padding to align next member
		enum class  LightType : uint32_t // maps to int in shader
		{   
			Point,
			Directional,
			Spotlight,
		} lightType;


		uint8_t padding2[4]; 

	};

	class LightSystem
	{

	private:
		std::vector<Light> m_lights;//index should match light id and be used to send to ssbo

		Memory::GPUBufferManager& m_gpuBufferManager;

		Memory::GPUBufferInfo m_lightSSBO;
		bool m_newLightAdded = false;

	public:

		LightSystem(Memory::GPUBufferManager& gpuBufferManager);

		//later add id system
		void addLight(const Light& light);
		
	    //Currently only has one single ssbo so if gpu is reading while writing its a issue
		void uploadLightsToGPU();

		Memory::GPUBufferInfo getLightSSBO() const;


	};

}