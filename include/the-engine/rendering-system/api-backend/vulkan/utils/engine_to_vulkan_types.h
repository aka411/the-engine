#pragma once
#include <volk.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/data-structures/gpu_types.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{

	VkBool32 getVkBool(const bool value);

	VkFormat resourceFormatToVkFormat(const TheEngine::RenderingSystem::ResourceFormat format);

	VkBlendFactor getVkBlendFactor(const BlendFactor factor);
	VkBlendOp getVkBlendOp(const BlendOp op);

	
	VkCullModeFlags cullModeToVkCullModeFlags(const CullMode mode);
	VkCompareOp compareOpToVkCompareOp(const CompareOp op);
	VkStencilOp stencilOpToVkStencilOp(const StencilOp op);

	VkPolygonMode fillModeToVkPolygonMode(const FillMode mode);
	VkFrontFace frontFaceToVkFrontFace(const FrontFace face);
	VkStencilOpState stencilFaceStateToVkStencilOpState(const StencilFaceState state);


	VkSampleCountFlagBits getVkSampleCount(const SampleCountBit sampleCountBit);





	VkImageUsageFlags textureUsageFlagsToVkImageUsageFlags(const TextureUsageFlags flags);
	VkImageLayout textureLayoutToVkImageLayout(const TextureLayout layout);



}