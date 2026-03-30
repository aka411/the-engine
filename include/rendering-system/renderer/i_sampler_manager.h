#pragma once
#include <vector>


namespace TheEngine::RenderingSystem
{


	class ISamplerManager
	{

	private:

		// Every backend (Vulkan/GL) needs to know the metadata
		std::vector<SamplerMetadata> m_samplerRegistry;

		// A way to track "Free" indices for handles
		std::vector<uint32_t> m_freeIndices;

	public:

		ISamplerManager();
		virtual ~ISamplerManager() =default;

		virtual const SamplerHandle getOrCreateSampler(const SamplerSetting& SamplerSetting) = 0;

		SamplerMetadata getSamplerMetadata(const SamplerHandle& samplerHandle)
		{
			assert(m_samplerRegistry.size() < samplerHandle.id && "ISamplerManager : Invalid handle");

			return m_samplerRegistry[samplerHandle.id];

		}


		virtual void destroySampler(const SamplerHandle& samplerHandle) = 0;



	};





}