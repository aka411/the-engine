#pragma once
#include "glad/glad.h"
#include "low-level/rendering_system_data_types.h"


namespace TheEngine::OpenGLBackend
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