#pragma once
#include <vector>
#include <map>
#include <rendering-system/engine_handles.h>
#include <assert.h>
#include <rendering-system/rhi/data-structures/gpu_sampler_data_structures.h>

namespace TheEngine::RenderingSystem
{

	


	class ISamplerManager
	{

	private:
		
	
		std::vector<SamplerCreateInfo> m_samplerRegistry;
		//std::map<SamplerCreateInfoHash, SamplerHandle> m_samplerCache;
		//each api will one on one map to m_samplerRegistry
		// A way to track "Free" indices for handles

		std::vector<uint32_t> m_freeIndices;

	public:

		ISamplerManager() = default;
		virtual ~ISamplerManager() = default;

		virtual const SamplerHandle getOrCreateSampler(const SamplerCreateInfo& samplerCreateInfo) = 0;

		SamplerCreateInfo getSamplerMetadata(const SamplerHandle& samplerHandle)
		{
			assert(m_samplerRegistry.size() > samplerHandle.id && "ISamplerManager : Invalid handle");

			return m_samplerRegistry[samplerHandle.id];

		}


		virtual void destroySampler(const SamplerHandle& samplerHandle) = 0;



	};





}