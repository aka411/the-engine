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


	struct SamplerSetting
	{
		TextureFilter minFilter = TextureFilter::LINEAR;
		TextureFilter magFilter = TextureFilter::LINEAR;

		TextureWrap wrapS = TextureWrap::REPEAT;
		TextureWrap wrapT = TextureWrap::REPEAT;

		TextureWrap wrapR = TextureWrap::REPEAT; // For 3D or Cube Maps
		// Optional: Anisotropy, LOD bias, etc.




		bool operator==(const SamplerSetting& other) const
		{
			return minFilter == other.minFilter &&
				magFilter == other.magFilter &&
				wrapS == other.wrapS &&
				wrapT == other.wrapT &&
				wrapR == other.wrapR;
		}

	};


	//For MVP
	struct GPUSampler
	{
		//yeah gonna take a assumption this is enough to store GLuint  etc
		//Why not just use GLuint -> I just dont want to , i also dont want to drag glad headers all over here
		uint64_t residentHandle;//may need to rethink
	};






}