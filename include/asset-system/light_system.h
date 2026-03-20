#pragma once
#include <vector>
#include <set>
#include "rendering-system/gpu-resource-system/gpu_light_manager.h"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>



namespace ThEngine::RenderingSystem
{
	class GPULightManager;
}




namespace TheEngine::AssetSystem
{



	//Design Note : use Shared Pointer for each light and also have a dirty flag 
	//or let game tell which lights to update

	enum class LightType
	{
		AMBIENT_LIGHT, //the global light that affects everything equally, to account for indirect lighting
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		SPOT_LIGHT
		
	};

	struct LightData // need to have getter and setter to set dirty flag, but who will be responsible for putting it in list of dirty lights?
	{
		LightType lightType;
		bool isDirty;


		glm::vec3 position;// Unit : in meters
		glm::vec4 color;

		glm::vec3 direction;
		float intensity;// Unit : in lumens

		//for spot light
		//Unit : in degrees
		float innerConeAngle;
		float outerConeAngle;


		RenderingSystem::LightId lightId;
	};









	class LightSystem
	{

	private:



		RenderingSystem::GPULightManager& m_gpuLightManager;

	
		std::vector<LightData> m_lights;//cpu side copy that game sees , owner


		//set?
		std::set<RenderingSystem::LightId> m_dirtyLightIds;

	public:

		LightSystem(RenderingSystem::GPULightManager& gpuLightManager);

		//Should be stable reference
		RenderingSystem::LightId createNewLight(LightData& lightData);
		LightData& getLightData(const RenderingSystem::LightId lightId);

		
		void setDirtyLight(const RenderingSystem::LightId lightId);


		void deleteLight(const RenderingSystem::GPULightIndex gpuLightIndex);

		void syncAllDirtyLightsToGPU();//To be called Before rendering or after???


	};


	class DirectionalLight
	{

	private:

		LightSystem& m_lightSystem;
		RenderingSystem::LightId lightId;


		void setDirtyLight();
		LightData& getLightData();

	public:
		DirectionalLight(LightSystem& lightSystem) :
			m_lightSystem(lightSystem)
		{

		};

	};


}