#pragma once
#include <cstddef>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace TheEngine::RenderingSystem
{



	using MaterialId = uint64_t;//Offset in GPU material buffer

	//i was thinking of also adding a float only version of pbr
	enum class ShadingModel
	{
		PBR_METALLIC_ROUGHNESS,
	};




	//This can be hidden
	struct GPUPBRMaterial
	{

		//This goes to GPU Padding is crucial

		//N = 4
		//Alignement : 4N , 4 * 4 =16
		glm::vec4 baseColorFactor{ 0 };//4N ,offset =  0
		glm::vec3 emissiveFactor{ 0 };// offset = 16

		//Alignement : N , 4
		float metallicFactor = 0.0f; //offset = 28
		float roughnessFactor = 0.0f; //offset = 32

		float padding0 = 0;//offset = 36

		// Alignement : 2N , 2*4 = 8 
		uint64_t albedoTextureHandle = 0; //offset = 40
		uint64_t metallicRoughnessTextureHandle = 0; //offset = 48

		uint64_t normalTextureHandle = 0; //offset = 56
		uint64_t occlusionTextureHandle = 0;// offset = 64
		uint64_t emissiveTextureHandle = 0;//offset =  72
		//end at 79

		//next struct starts at 80 which is multiple of 16
	
	};



}