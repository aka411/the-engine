#pragma once
#include "../../igpu_texture.h"
#include "../../igpu_sampler.h"
#include "../../igpu_buffer.h"
#include "../../core/common_enums.h"


namespace TheEngine::Graphics::OpenGLUtils
{
	//Texture utility functions
    GLenum getGLInternalFormat(const TextureFormat& format);
    GLenum getGLPixelDataFormat(const TextureFormat& format);
    GLenum getGLPixelDataType(const TextureFormat& format);
    GLenum getGLTextureTarget(const TextureType& type);

	//Buffer utility functions
    GLbitfield getGLMemoryAccessFlags(const BufferDescriptor& bufferDescriptor);
    GLenum getGLTarget(const BufferDescriptor& bufferDescriptor);


	//Sampler utility functions
	GLenum getMagFilter(const FilterMode& filterMode);
	GLenum getMinFilter(const FilterMode& filterMode,const MipmapMode& mipmapMode);
	GLenum getGLAddressMode(const AddressMode& addressMode);
	


	//VertexAttribute utility functions
	GLenum getGLVertexAttributeType(const VertexAttributeFormat& format);
	GLint  getGLVertexAttributeTypeSize(const VertexAttributeFormat& format);
	GLboolean getGLVertexAttributeNormalized(const VertexAttributeFormat& format);

	// Index utility functions
	GLenum getGLIndexFormat(const IndexFormat& format);



	//OpenGL State Utility Functions

	GLenum getGLPrimitiveTopology(const PrimitiveTopology& topology);
	GLenum getGLCullMode(const CullMode& cullMode);
	GLenum getGLPolygonMode(const PolygonMode& polygonMode);
	GLenum getGLDepthCompareOp(const CompareOp& compareOp);
	GLenum getGLFrontFace(const FrontFace& frontFace);



   
}