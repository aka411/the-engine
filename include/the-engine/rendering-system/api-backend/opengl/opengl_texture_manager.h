#pragma once
#include <unordered_map>
#include <glad/glad.h>

#include <rendering-system/rhi/i_texture_manager.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	struct OpenglTextureHandle
	{
		GLuint glTextureId = 0;
		uint64_t residentHandle = 0; // For bindless textures

	};

	class OpenglTextureManager : public ITextureManager
	{

	private:


		std::vector<OpenglTextureHandle> m_allotedGLTextures; 



	public:

		OpenglTextureManager();

		virtual TextureHandle createNewTexture(const TextureCreateInfo& textureCreateInfo) override;

	};


}