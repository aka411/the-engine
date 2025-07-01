#pragma once
#include "igpu_descriptor_set_layout.h"
#include "igpu_framebuffer.h"
#include "igpu_texture_view.h"
#include "igpu_buffer.h"
#include "igpu_sampler.h"



//cleaned but not checked and corrected
//union needs correction
namespace TheEngine::Graphics
{





	// Structures for updating descriptor set (API-agnostic)
	struct BufferDescriptorInfo
	{
		IGPUBuffer* buffer;
		uint64_t offset;
	};

	struct ImageDescriptorInfo
	{
		IGPUTextureView* imageView; // A view into a texture
		TextureLayout imageLayout;   // Current layout of the image (e.g., ShaderReadOnlyOptimal)
		IGPUSampler* sampler = nullptr;       // Optional: for CombinedImageSampler types
	};


	struct SamplerDescriptorInfo
	{
		IGPUSampler* sampler; // For sampler types
	};

	struct DescriptorBinding // used to update a descriptor set
	{
		uint32_t binding;          // The binding point in the shader (e.g., layout(binding = 0))
		DescriptorType type;       // Type of the descriptor (e.g., UniformBuffer, SampledImage)
		uint32_t arrayElement;     // If this is an array of descriptors, which element to bind

		// std::variant<BufferDescriptorInfo, ImageDescriptorInfo, IGPUSampler*> resourceInfo;//consider variant instead of union (C++17 required)
		union
		{
			BufferDescriptorInfo buffer; // For buffer types
			ImageDescriptorInfo image;   // For image types
			SamplerDescriptorInfo sampler; // For sampler types

		};

	};


	//maps to one unique set in shader
	class IGPUDescriptorSet : public IGPUResource
	{
	protected:
		IGPUDescriptorSet() = default; 
	public:
		virtual ~IGPUDescriptorSet() = default;

		virtual void UpdateDescriptors(const std::vector<DescriptorBinding>& bindings) = 0;

	};

}