#pragma once



namespace TheEngine::RenderingSystem::OpenGLBackend
{

    struct OpenglSampler
    {
        SamplerHandle samplerHandle;//only to for use by manager,not to be used by user
        GLuint openglSamplerHandle;
    };


	class OpenglSamplerManager : IGPUSamplerManager
	{

	private :

        struct SamplerHasher
        {
            std::size_t operator()(const SamplerSetting& s) const
            {
              
                std::size_t h1 = std::hash<int>{}(static_cast<int>(s.minFilter));
                std::size_t h2 = std::hash<int>{}(static_cast<int>(s.magFilter));
                std::size_t h3 = std::hash<int>{}(static_cast<int>(s.wrapS));
                std::size_t h4 = std::hash<int>{}(static_cast<int>(s.wrapT));
                std::size_t h5 = std::hash<int>{}(static_cast<int>(s.wrapR));

                
                std::size_t seed = 0;
                auto combine = [&](std::size_t h) {
                    seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                    };
                combine(h1); combine(h2); combine(h3); combine(h4); combine(h5);
                return seed;
            }
        };
		//Need to store sampler settings to sampler
        //If we are using hash, where will we het handle from
		std::unordered_map<SamplerSetting, OpenglSampler, SamplerHasher> m_cachedGPUSamplers;

	public  :

        OpenglSamplerManager();
		~OpenglSamplerManager();


        virtual const SamplerHandle getOrCreateSampler(const SamplerSetting& SamplerSetting) override;

        virtual void destroySampler(const SamplerHandle& samplerHandle) override;

        const OpenglSampler getNativeSampler(const SamplerHandle& samplerHandle);
	};


}