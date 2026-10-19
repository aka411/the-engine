#pragma once
#include <cstdint>
#include <vector>

#include <rendering-system/rhi/data-structures/gpu_types.h>




namespace TheEngine::RenderingSystem
{


	struct DescriptorBinding
	{
		uint32_t binding;           //  shader layout(binding = N)
		ResourceType type;          // Uniform, Storage, Sampler, etc.
		ShaderStageFlags stageFlags;     // Which stages can see this

		//Used for Storage Images, Texel Buffers, and Input Attachments
		ResourceFormat format = ResourceFormat::UNKNOWN;

		uint32_t count = 1;         // For arrays


		bool operator==(const DescriptorBinding& other) const
		{
			return this->binding == other.binding &&
				this->type == other.type &&
				this->stageFlags == other.stageFlags &&
				this->format == other.format &&
				this->count == other.count;
		}
	};




	class DescriptorSetLayout
	{


	private:

		uint32_t m_setIndex;
		std::vector<DescriptorBinding> m_bindings;

		// Helper to combine hashes 
		static void hashCombine(size_t& seed, size_t value);


	public:
		bool isBindless = false;
		DescriptorSetLayout(const uint32_t setIndex = 0);


		void addBinding(uint32_t bindingSlot, ResourceType type, ShaderStageFlags stages,
			ResourceFormat format = ResourceFormat::UNKNOWN, uint32_t count = 1);

		// Getters
		uint32_t getSetIndex() const;
		const std::vector<DescriptorBinding>& getBindings() const;

		//To help cache layouts
		uint64_t getHash() const;

	
		//Operators
		bool operator==(const DescriptorSetLayout& other) const;

	};




}