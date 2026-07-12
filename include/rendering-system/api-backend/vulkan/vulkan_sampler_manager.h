#pragma once
#include <unordered_map>

#include <volk.h>

#include <rendering-system/rhi/i_sampler_manager.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{
    class VulkanContext;

	class VulkanSamplerManager : public ISamplerManager
	{

	private:

        VulkanContext& m_vulkanContext;


        struct SamplerHasher
        {
            std::size_t operator()(const SamplerCreateInfo& s) const
            {

                std::size_t h1 = std::hash<int>{}(static_cast<int>(s.minFilter));
                std::size_t h2 = std::hash<int>{}(static_cast<int>(s.magFilter));
                std::size_t h3 = std::hash<int>{}(static_cast<int>(s.wrapS));
                std::size_t h4 = std::hash<int>{}(static_cast<int>(s.wrapT));
                std::size_t h5 = std::hash<int>{}(static_cast<int>(s.wrapR));


                std::size_t seed = 0;
                auto combine = [&](std::size_t h) 
                    {
                    seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                    };
                combine(h1); combine(h2); combine(h3); combine(h4); combine(h5);
                return seed;
            }
        };

        //Need to store sampler settings to sampler
        std::unordered_map<SamplerCreateInfo, VkSampler, SamplerHasher> m_cachedVulkanSamplers;

	public:

        VulkanSamplerManager(VulkanContext& vulkanContext);

		virtual ~VulkanSamplerManager() override;


		virtual const SamplerHandle getOrCreateSampler(const TheEngine::RenderingSystem::SamplerCreateInfo& samplerCreateInfo) override;
		virtual void destroySampler(const SamplerHandle& samplerHandle) override;

        //Vulkan Specific Methods
        VkSampler getOrCreateVkSampler(const TheEngine::RenderingSystem::SamplerCreateInfo& samplerCreateInfo);

	};





}