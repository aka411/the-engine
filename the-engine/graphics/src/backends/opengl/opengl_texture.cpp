#include "../../../include/backends/opengl/opengl_texture.h"


namespace TheEngine::Graphics
{


	Graphics::OpenGLTexture::OpenGLTexture(const OpenGLTextureCreateInfo& openGLTextureCreateInfo)
		:m_openGLTextureCreateInfo(openGLTextureCreateInfo)
	{

	}
	



	GLuint OpenGLTexture::getNativeHandle() const
{
	return m_textureId;
}


}