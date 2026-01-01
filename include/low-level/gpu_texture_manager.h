#pragma once
#include <unordered_map>
#include "rendering_system_data_types.h"
#include "glad/glad.h"



namespace TheEngine
{


	class GPUTextureManager
	{

	private:

		std::unordered_map<uint64_t, GLuint> m_residentHandleToTextureApiHandle;
		size_t m_totalAllocatedTextureMemory = 0;

	public:

		GPUTextureManager();
		~GPUTextureManager();

		TextureInfo createNewTexture(const TextureCreateInfo& textureCreateInfo);
		TextureInfo createBufferTexture(const BufferTextureCreateInfo& bufferTextureCreateInfo);


		//GLuint loadTexture(const char* filePath);

		void destroyTexture(TextureInfo& textureInfo);

		size_t getTotalAllocatedTextureMemory() const;

	};


}