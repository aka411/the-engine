#pragma once



namespace TheEngine::RenderingSystem::OpenGLBackend
{

	class GPUSamplerManager;
		

	class GPUTextureManager
	{

	private:

		std::unordered_map<uint64_t, GLuint> m_residentHandleToTextureApiHandle;

		size_t m_totalAllocatedTextureMemory = 0;

		GPUSamplerManager& m_gpuSamplerManager;

	public:

		GPUTextureManager(GPUSamplerManager& gpuSamplerManager);
		~GPUTextureManager();

		TextureInfo createNewTexture(const TextureCreateInfo& textureCreateInfo);
		RenderTarget createRenderTarget(const RenderTargetCreateInfo& bufferTextureCreateInfo);



		void destroyTexture(TextureInfo& textureInfo);

		size_t getTotalAllocatedTextureMemory() const;

	};


}