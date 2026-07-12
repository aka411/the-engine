
#include <assert.h>
#include <map>
#include <algorithm>

#include <spirv_reflect.h>

#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/data-structures/gpu_types.h>
#include <rendering-system/rhi/data-structures/descriptor_set_layout.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>
#include <rendering-system/rhi/i_shader_manager.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{



	ResourceType mapSpirvToResource(SpvReflectDescriptorType reflectType)
	{
		switch (reflectType) 
		{
		case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:                return ResourceType::SAMPLER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return ResourceType::COMBINED_IMAGE_SAMPLER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:          return ResourceType::SAMPLED_IMAGE;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:          return ResourceType::STORAGE_IMAGE;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:   return ResourceType::UNIFORM_TEXEL_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:   return ResourceType::STORAGE_TEXEL_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:         return ResourceType::UNIFORM_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:         return ResourceType::STORAGE_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return ResourceType::UNIFORM_BUFFER_DYNAMIC;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return ResourceType::STORAGE_BUFFER_DYNAMIC;
		case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:       return ResourceType::INPUT_ATTACHMENT;
		case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return ResourceType::ACCELERATION_STRUCTURE;

		default: return ResourceType::UNKNOWN;
		}
	}



	ResourceFormat mapSpirvToFormat(SpvReflectFormat reflectFormat)
	{
		switch (reflectFormat)
		{
		case SPV_REFLECT_FORMAT_R32_UINT:           return ResourceFormat::R32_UINT;
		case SPV_REFLECT_FORMAT_R32_SFLOAT:         return ResourceFormat::R32_FLOAT;
		case SPV_REFLECT_FORMAT_R32G32_SFLOAT:       return ResourceFormat::RG32_FLOAT;
		case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:    return ResourceFormat::RGB32_FLOAT;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return ResourceFormat::RGBA32_FLOAT;
		

		default: return ResourceFormat::UNKNOWN;
		}
	}

	static ShaderStageFlags toShaderStageFlag(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::VERTEX:                  return ShaderStageFlags::VERTEX;
		case ShaderType::FRAGMENT:                return ShaderStageFlags::FRAGMENT;
		case ShaderType::COMPUTE:                 return ShaderStageFlags::COMPUTE;
		case ShaderType::GEOMETRY:                return ShaderStageFlags::GEOMETRY;
		case ShaderType::TESSELLATION_CONTROL:    return ShaderStageFlags::TESSELLATION_CONTROL;
		case ShaderType::TESSELLATION_EVALUATION: return ShaderStageFlags::TESSELLATION_EVALUATION;
		default: return ShaderStageFlags::NONE;
		}
	}


	void reflectDescriptors(
		std::map<uint32_t, DescriptorSetLayout>& layoutMap,
		const std::vector<uint32_t>& code,
		ShaderType shaderType)
	{
		SpvReflectShaderModule module;
		spvReflectCreateShaderModule(code.size() * sizeof(uint32_t), code.data(), &module);

		uint32_t count = 0;
		spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
		std::vector<SpvReflectDescriptorSet*> sets(count);
		spvReflectEnumerateDescriptorSets(&module, &count, sets.data());

		ShaderStageFlags stageBit = toShaderStageFlag(shaderType);


		const int RESERVED_BINDLESS_TEXTURE_ARRAY_SET_INDEX = 0;

		for (auto* reflect_set : sets)
		{

			if (layoutMap.find(reflect_set->set) == layoutMap.end())
			{
				layoutMap.emplace(reflect_set->set, DescriptorSetLayout(reflect_set->set));
			}

			auto& internalLayout = layoutMap.at(reflect_set->set);


			if (reflect_set->set == RESERVED_BINDLESS_TEXTURE_ARRAY_SET_INDEX)
			{
				internalLayout.isBindless = true;

			}

			for (uint32_t i = 0; i < reflect_set->binding_count; ++i) 
			{
				auto* b = reflect_set->bindings[i];


				uint32_t finalCount = b->count;

				// If it's our bindless set and the reflection says 0, set a real limit
				if (reflect_set->set == RESERVED_BINDLESS_TEXTURE_ARRAY_SET_INDEX && finalCount == 0) 
				{
					finalCount = 4096;//I think this number needs to match the one we use when creating pipeline
				}
				internalLayout.addBinding(
					b->binding,
					mapSpirvToResource(b->descriptor_type),
					stageBit,
					ResourceFormat::UNKNOWN,
					finalCount
				);
			}
		}
		spvReflectDestroyShaderModule(&module);
	}





	void reflectVertexInputs(const std::vector<uint32_t>& code, TheEngine::RenderingSystem::VertexLayout& layout)
	{
		layout.clear();

		SpvReflectShaderModule module;
		spvReflectCreateShaderModule(code.size() * sizeof(uint32_t), code.data(), &module);

		uint32_t count = 0;
		spvReflectEnumerateInputVariables(&module, &count, nullptr);
		std::vector<SpvReflectInterfaceVariable*> inputs(count);
		spvReflectEnumerateInputVariables(&module, &count, inputs.data());


		std::vector<SpvReflectInterfaceVariable*> filteredInputs;

		for (auto* input : inputs)
		{


			if (input->built_in != -1)
			{
				continue;
			}
			filteredInputs.push_back(input);
		}

		std::sort(filteredInputs.begin(), filteredInputs.end(), [](SpvReflectInterfaceVariable* a, SpvReflectInterfaceVariable* b) {
			return a->location < b->location;
			});

		//Map attributes and calculate stride/offsets
		uint32_t currentOffset = 0;
		for (auto* input : filteredInputs)
		{
			ResourceFormat format = mapSpirvToFormat(input->format);

			layout.addAttribute(
				0,              // Interleaved
				input->location,
				format,
				currentOffset
			);

			// Calculate size of this attribute to move the offset forward
			uint32_t size = (input->numeric.scalar.width / 8) * input->numeric.vector.component_count;
			currentOffset += size;
		}


		layout.addBinding(0, currentOffset, false);

		spvReflectDestroyShaderModule(&module);
	}

}