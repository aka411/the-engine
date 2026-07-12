#include <assert.h>

#include <rendering-system/rhi/data-structures/descriptor_set_layout.h>


namespace TheEngine::RenderingSystem
{




	void DescriptorSetLayout::hashCombine(size_t& seed, size_t value)
	{
		seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}



	DescriptorSetLayout::DescriptorSetLayout(const uint32_t setIndex) :
		m_setIndex(setIndex)
	{

	}



	void DescriptorSetLayout::addBinding(uint32_t bindingSlot, ResourceType type, ShaderStageFlags stages,
		ResourceFormat format, uint32_t count)
	{

		if (type == ResourceType::STORAGE_IMAGE && format == ResourceFormat::UNKNOWN)
		{
			assert(false && "No format for ResourceType::STORAGE_IMAGE");
		}

	
		for (auto& b : m_bindings)
		{
			if (b.binding == bindingSlot)
			{
				b.stageFlags = b.stageFlags | stages;
				return;
			}
		}



		m_bindings.push_back({ bindingSlot, type, stages, format, count });
	}

	// Getters
	uint32_t DescriptorSetLayout::getSetIndex() const
	{
		return m_setIndex;
	}


	const std::vector<DescriptorBinding>& DescriptorSetLayout::getBindings() const
	{
		return m_bindings;
	}


	uint64_t DescriptorSetLayout::getHash() const
	{

		size_t seed = 0;

		//Hash the set index
		hashCombine(seed, std::hash<uint32_t>{}(m_setIndex));

		//Hash every binding
		for (const auto& b : m_bindings)
		{
			hashCombine(seed, std::hash<uint32_t>{}(isBindless));
			hashCombine(seed, std::hash<uint32_t>{}(b.binding));
			hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(b.type)));
			hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(b.stageFlags)));
			hashCombine(seed, std::hash<uint32_t>{}(static_cast<uint32_t>(b.format)));
			hashCombine(seed, std::hash<uint32_t>{}(b.count));
		}

		return seed;
	}


	bool DescriptorSetLayout::operator==(const DescriptorSetLayout& other) const
	{
		return this->getSetIndex() == other.getSetIndex() && this->getBindings() == other.getBindings();
	}



}