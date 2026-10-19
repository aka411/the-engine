#pragma once
#include "glad/glad.h"
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{


	GLenum toGLEnum(TextureSourcePixelFormat format);

	GLenum toGLEnum(TextureSourceComponentType type);

	GLenum toGLEnum(TextureInternalFormat format);


	GLenum toGLEnum(TextureType type);



	//------- SAMPLER --------//


	GLenum toGLEnum(TextureFilter filter, bool isMagFilter = false);

	GLenum toGLEnum(TextureWrap wrap);

	bool requiresMipmaps(TextureFilter minFilter);




	//----Memory Utils ----//

	size_t getBytesPerPixel(TextureInternalFormat format);


}