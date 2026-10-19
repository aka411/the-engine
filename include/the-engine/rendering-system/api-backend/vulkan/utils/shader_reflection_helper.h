#pragma once
#include <vector>
#include <map>

#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{



	void reflectDescriptors(
		std::map<uint32_t, DescriptorSetLayout>& layoutMap, 
		const std::vector<uint32_t>& code,
		ShaderType shaderType);

	void reflectVertexInputs(const std::vector<uint32_t>& code, TheEngine::RenderingSystem::VertexLayout& layout);




}