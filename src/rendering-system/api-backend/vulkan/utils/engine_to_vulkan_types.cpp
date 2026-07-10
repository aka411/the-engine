#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <cassert>



namespace TheEngine::RenderingSystem::VulkanBackend
{



	VkBool32 getVkBool(const bool value)
	{
		return value ? VK_TRUE : VK_FALSE;
	}




	VkFormat resourceFormatToVkFormat(const ResourceFormat format)
	{
		switch (format)
		{
			// --- Standard Unorm Formats ---
		case ResourceFormat::R8_UNORM:           return VK_FORMAT_R8_UNORM;
		case ResourceFormat::RG8_UNORM:          return VK_FORMAT_R8G8_UNORM;
		case ResourceFormat::RGBA8_UNORM:        return VK_FORMAT_R8G8B8A8_UNORM;
		case ResourceFormat::BGRA8_UNORM:        return VK_FORMAT_B8G8R8A8_UNORM;

			// --- Vertex Buffer Specific Formats ---
		case ResourceFormat::RG32_FLOAT:         return VK_FORMAT_R32G32_SFLOAT;    // Typical for UVs (vec2)
		case ResourceFormat::RGB32_FLOAT:        return VK_FORMAT_R32G32B32_SFLOAT; // Typical for Positions/Normals (vec3)

			// --- SRGB Formats ---
		case ResourceFormat::RGBA8_SRGB:         return VK_FORMAT_R8G8B8A8_SRGB;

			// --- Floating Point Formats (HDR / Simulation) ---
		case ResourceFormat::R16_FLOAT:          return VK_FORMAT_R16_SFLOAT;
		case ResourceFormat::RG16_FLOAT:         return VK_FORMAT_R16G16_SFLOAT;
		case ResourceFormat::RGBA16_FLOAT:       return VK_FORMAT_R16G16B16A16_SFLOAT;
		case ResourceFormat::R32_FLOAT:          return VK_FORMAT_R32_SFLOAT;
		case ResourceFormat::RGBA32_FLOAT:       return VK_FORMAT_R32G32B32A32_SFLOAT;

			// --- Special Packed Formats ---
		case ResourceFormat::R11G11B10_FLOAT:    return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			//case ResourceFormat::RGB10A2_UNORM:      return VK_FORMAT_A2B10G11R11_UNORM_PACK32;

				// --- Integer Formats ---
		case ResourceFormat::R32_UINT:           return VK_FORMAT_R32_UINT;
		case ResourceFormat::RGBA32_UINT:        return VK_FORMAT_R32G32B32A32_UINT;

			// --- Depth and Stencil ---
		case ResourceFormat::D16_UNORM:          return VK_FORMAT_D16_UNORM;
		case ResourceFormat::D32_FLOAT:          return VK_FORMAT_D32_SFLOAT;
		case ResourceFormat::D24_S8_UINT:        return VK_FORMAT_D24_UNORM_S8_UINT;
		case ResourceFormat::D32_S8_FLOAT:       return VK_FORMAT_D32_SFLOAT_S8_UINT;

		case ResourceFormat::UNKNOWN:
		default:
		{
			assert(false && "Unknown ResourceFormat");
			return VK_FORMAT_UNDEFINED;
		}
		}
	}





	VkBlendFactor getVkBlendFactor(const BlendFactor factor)
	{
		switch (factor)
		{
		case BlendFactor::ZERO:                return VK_BLEND_FACTOR_ZERO;
		case BlendFactor::ONE:                 return VK_BLEND_FACTOR_ONE;
		case BlendFactor::SRC_ALPHA:           return VK_BLEND_FACTOR_SRC_ALPHA;
		case BlendFactor::ONE_MINUS_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		default:
		{
			assert(false && "add missing VkBlendFactors");
			return VK_BLEND_FACTOR_ZERO;
		}

		}

	}


	VkBlendOp getVkBlendOp(const BlendOp op)
	{
		switch (op)
		{
		case BlendOp::ADD:              return VK_BLEND_OP_ADD;
		case BlendOp::SUBTRACT:         return VK_BLEND_OP_SUBTRACT;

		default:
		{
			assert(false && "add missing VkBlendOp");
			return VK_BLEND_OP_ADD;
		}
		}

	}



	VkCullModeFlags cullModeToVkCullModeFlags(const CullMode mode)
	{
		switch (mode) 
		{
		case CullMode::NONE:           return VK_CULL_MODE_NONE;
		case CullMode::BACK:           return VK_CULL_MODE_BACK_BIT;
		case CullMode::FRONT:          return VK_CULL_MODE_FRONT_BIT;
		case CullMode::FRONT_AND_BACK: return VK_CULL_MODE_FRONT_AND_BACK;

		default:
		{
			assert(false && "Unknown CullMode");
			return VK_CULL_MODE_BACK_BIT;
		}
		}
	}



	VkCompareOp compareOpToVkCompareOp(const CompareOp op)
	{
		switch (op)
		{
		case CompareOp::NEVER:         return VK_COMPARE_OP_NEVER;
		case CompareOp::LESS:          return VK_COMPARE_OP_LESS;
		case CompareOp::EQUAL:         return VK_COMPARE_OP_EQUAL;
		case CompareOp::LESS_OR_EQUAL: return VK_COMPARE_OP_LESS_OR_EQUAL;
		case CompareOp::GREATER:       return VK_COMPARE_OP_GREATER;
		case CompareOp::NOT_EQUAL:     return VK_COMPARE_OP_NOT_EQUAL;
		case CompareOp::GREATER_OR_EQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case CompareOp::ALWAYS:        return VK_COMPARE_OP_ALWAYS;

		default:
		{
			assert(false && "Unknown CompareOp");
			return VK_COMPARE_OP_LESS;
		}
		}
	}

	VkStencilOp stencilOpToVkStencilOp(const StencilOp op)
	{
		switch (op)
		{
		case StencilOp::KEEP:                return VK_STENCIL_OP_KEEP;
		case StencilOp::ZERO:                return VK_STENCIL_OP_ZERO;
		case StencilOp::REPLACE:             return VK_STENCIL_OP_REPLACE;
		case StencilOp::INCREMENT_AND_CLAMP: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		case StencilOp::DECREMENT_AND_CLAMP: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		case StencilOp::INVERT:               return VK_STENCIL_OP_INVERT;
		case StencilOp::INCREMENT_AND_WRAP:  return VK_STENCIL_OP_INCREMENT_AND_WRAP;
		case StencilOp::DECREMENT_AND_WRAP:  return VK_STENCIL_OP_DECREMENT_AND_WRAP;

		default:
		{
			assert(false && "Unknown StencilOp");
			return VK_STENCIL_OP_KEEP;
		}

		}
	}




	VkImageUsageFlags textureUsageFlagsToVkImageUsageFlags(const TextureUsageFlags flags)
	{
		VkImageUsageFlags vkFlags = 0;
		if (flags & IMAGE_USAGE_TRANSFER_SRC_BIT)           vkFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (flags & IMAGE_USAGE_TRANSFER_DST_BIT)           vkFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		if (flags & IMAGE_USAGE_SAMPLED_BIT)                vkFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		if (flags & IMAGE_USAGE_STORAGE_BIT)                vkFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
		if (flags & IMAGE_USAGE_COLOR_ATTACHMENT_BIT)       vkFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (flags & IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) vkFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		if (flags & IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)   vkFlags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		if (flags & IMAGE_USAGE_INPUT_ATTACHMENT_BIT)       vkFlags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

		return vkFlags;
	}

	VkImageLayout textureLayoutToVkImageLayout(const TextureLayout layout)
	{
		switch (layout) 
		{
		case TextureLayout::UNDEFINED:        return VK_IMAGE_LAYOUT_UNDEFINED;
		case TextureLayout::GENERAL:          return VK_IMAGE_LAYOUT_GENERAL;
		case TextureLayout::COLOR_ATTACHMENT: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case TextureLayout::DEPTH_STENCIL:    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case TextureLayout::SHADER_READ_ONLY: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		case TextureLayout::TRANSFER_SRC:     return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case TextureLayout::TRANSFER_DST:     return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case TextureLayout::PRESENT_SRC:      return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		default:
		{
			assert(false && "Unknown TextureLayout");
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
		}
	}






	VkPolygonMode fillModeToVkPolygonMode(const FillMode mode)
	{
		switch (mode)
		{
		case FillMode::SOLID:     return VK_POLYGON_MODE_FILL;
		case FillMode::WIREFRAME: return VK_POLYGON_MODE_LINE;
		case FillMode::POINT:     return VK_POLYGON_MODE_POINT;
		default:
		{
			assert(false && "Unknown FillMode");
			return VK_POLYGON_MODE_FILL;
		}
		}
	}


	VkFrontFace frontFaceToVkFrontFace(const FrontFace face)
	{
		return (face == FrontFace::CLOCKWISE) ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}






	VkStencilOpState stencilFaceStateToVkStencilOpState(const StencilFaceState state)
	{
		VkStencilOpState vkState = {};
		vkState.failOp = stencilOpToVkStencilOp(state.failOp);
		vkState.passOp = stencilOpToVkStencilOp(state.passOp);
		vkState.depthFailOp = stencilOpToVkStencilOp(state.depthFailOp);
		vkState.compareOp = compareOpToVkCompareOp(state.compareOp);
		vkState.compareMask = state.compareMask;
		vkState.writeMask = state.writeMask;
		vkState.reference = state.reference;
		return vkState;
	}




	VkSampleCountFlagBits getVkSampleCount(const SampleCountBit sampleCountBit)
	{
		switch (sampleCountBit)
		{
		case SampleCountBit::SAMPLE_COUNT_1_BIT:  return VK_SAMPLE_COUNT_1_BIT;
		case SampleCountBit::SAMPLE_COUNT_2_BIT:  return VK_SAMPLE_COUNT_2_BIT;
		case SampleCountBit::SAMPLE_COUNT_4_BIT:  return VK_SAMPLE_COUNT_4_BIT;
		case SampleCountBit::SAMPLE_COUNT_8_BIT:  return VK_SAMPLE_COUNT_8_BIT;
		case SampleCountBit::SAMPLE_COUNT_16_BIT: return VK_SAMPLE_COUNT_16_BIT;
		case SampleCountBit::SAMPLE_COUNT_32_BIT: return VK_SAMPLE_COUNT_32_BIT;
		case SampleCountBit::SAMPLE_COUNT_64_BIT: return VK_SAMPLE_COUNT_64_BIT;

		default:
		{
			assert(false && "Unsupported SampleCountBit value passed to getVkSampleCount");
			return VK_SAMPLE_COUNT_1_BIT;
		}
		}
	}



}