#pragma once
#include "igpu_resource.h"
#include <cfloat>
//clean
namespace TheEngine::Graphics
{


    enum class FilterMode
	{
		LINEAR,
		NEAREST,
        CUBIC,     // Bicubic filtering (if supported)

	
	};

    
    enum class AddressMode 
    {
        NONE,
		REPEAT,
		CLAMP_TO_EDGE,
		MIRRORED_REPEAT,
		CLAMP_TO_BORDER, 
		MIRROR_CLAMP_TO_EDGE, 
		
    };
    enum class MipmapMode 
    {
        NONE,
		LINEAR,
		NEAREST,


		
    };

    struct SamplerCreateInfo
    {
        FilterMode magFilter;
        FilterMode minFilter;

        AddressMode addressModeU= AddressMode ::NONE;
        AddressMode addressModeV= AddressMode ::NONE;
        AddressMode addressModeW= AddressMode ::NONE;

        MipmapMode mipmapMode = MipmapMode::NONE;

        float mipLodBias = 0.0f;
        float minLod = 0.0f;
        float maxLod = FLT_MAX; // Or appropriate max

        bool enableAnisotropy = false;
        float maxAnisotropy = 1.0f; // Only if enableAnisotropy is true

        // bool enableCompare = false;
        // CompareOp compareOp = CompareOp_Always;
        // BorderColor borderColor = BorderColor_OpaqueBlack;
		// bool unnormalizedCoordinates = false; // vulkan specific, not used in OpenGL
    };

    
	class IGPUSampler : public IGPUResource
    {
    private: 
		IGPUSampler() = default; // Default constructor is private to enforce use of factory methods
    public:
        virtual ~IGPUSampler() = default;

  

		//remove these methods as i dont see their use when using the sampler
        virtual FilterMode GetMagFilter() const = 0;
        virtual FilterMode GetMinFilter() const = 0;

        virtual AddressMode GetAddressModeU() const = 0;
        virtual AddressMode GetAddressModeV() const = 0;
        virtual AddressMode GetAddressModeW() const = 0;

        virtual MipmapMode GetMipmapMode() const = 0;

        virtual float GetMinLod() const = 0;
        virtual float GetMaxLod() const = 0;

        virtual float GetMipLodBias() const = 0;

       // virtual bool IsAnisotropyEnabled() const = 0;
       // virtual float GetMaxAnisotropy() const = 0;
    

     
    };


}