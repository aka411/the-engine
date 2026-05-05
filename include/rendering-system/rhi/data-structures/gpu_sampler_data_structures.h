#pragma once
#include <cstdint>



namespace TheEngine::RenderingSystem
{


	/***** Sampler Settings *****/


	enum class TextureFilter
	{
		NEAREST,             
		LINEAR,                 

		NEAREST_MIPMAP_NEAREST, 
		LINEAR_MIPMAP_NEAREST,  

		NEAREST_MIPMAP_LINEAR, 
		LINEAR_MIPMAP_LINEAR,    

		UNKNWON
	};

	enum class TextureWrap
	{
		REPEAT,             
		CLAMP_TO_EDGE,     
		MIRRORED_REPEAT,    
		UNKNOWN
	};


	struct SamplerCreateInfo
	{
		TextureFilter minFilter = TextureFilter::LINEAR;
		TextureFilter magFilter = TextureFilter::LINEAR;

		TextureWrap wrapS = TextureWrap::REPEAT;
		TextureWrap wrapT = TextureWrap::REPEAT;

		TextureWrap wrapR = TextureWrap::REPEAT; 
	



		
		bool operator==(const SamplerCreateInfo& other) const
		{
			return minFilter == other.minFilter &&
				magFilter == other.magFilter &&
				wrapS == other.wrapS &&
				wrapT == other.wrapT &&
				wrapR == other.wrapR;
		}

	};








}