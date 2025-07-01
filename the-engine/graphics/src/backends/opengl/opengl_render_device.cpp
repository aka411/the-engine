#include "../../../include/backends/opengl/opengl_render_device.h"
#include <memory>
#include "../../../include/igpu_buffer.h"
#include "../../../include/backends/opengl/opengl_buffer.h"
#include "../../../include/backends/opengl/opengl_texture.h"


//need to add more methods to this class
namespace TheEngine::Graphics
{

	OpenGLRenderDevice::~OpenGLRenderDevice()
	{

	}

	bool OpenGLRenderDevice::Initialize(uint32_t width, uint32_t height, const std::string& windowTitle)
	{
		return false;
	}





	std::unique_ptr<IGPUBuffer> OpenGLRenderDevice::createBuffer(const BufferDescriptor& bufferDescriptor, const void* data, const uint64_t size)
	{
		//Opnegl version 4.5 , DSA functions are used

		GLbitfield flags = OpenGLUtils::getGLMemoryAccessFlags(bufferDescriptor);
		GLenum target = OpenGLUtils::getGLTarget(bufferDescriptor);

		
		if (target == 0)
		{
			throw std::runtime_error("Unsupported buffer usage or target for OpenGL.");
		}
		GLuint bufferId;
		glCreateBuffers(1, &bufferId);


		glNamedBufferStorage(bufferId, size,data, flags);

		

		return std::make_unique<OpenGLBuffer>(bufferDescriptor,bufferId,target,size);
	}



	void OpenGLRenderDevice::updateBuffer(IGPUBuffer* buffer, const void* data, uint64_t size, uint64_t offset)
	{
		OpenGLBuffer* glBuffer = dynamic_cast<OpenGLBuffer*>(buffer);
		if (!glBuffer)
		{
			throw std::runtime_error("Invalid buffer type for OpenGL update.");
		}
		if (glBuffer->isCurrentlyMapped())
		{
			throw std::runtime_error("Cannot update a currently mapped buffer.");
		}
		if (offset + size > glBuffer->getSize())
		{
			throw std::runtime_error("Update exceeds buffer size.");
		}
		glNamedBufferSubData(glBuffer->GetNativeHandle(), offset, size, data);
	}



	std::unique_ptr<IGPUTexture> OpenGLRenderDevice::CreateTexture(const TextureCreateInfo& createInfo,const void* data)
	{
		GLuint textureId;
		GLenum target = OpenGLUtils::getGLTextureTarget(createInfo.type);

		GLenum internalFormat = OpenGLUtils::getGLInternalFormat(createInfo.format);

		GLenum pixelFormat = OpenGLUtils::getGLPixelDataFormat(createInfo.format);
		GLenum pixelDataType = OpenGLUtils::getGLPixelDataType(createInfo.format);

	
		//branching needed based on target type

		glCreateTextures(target, 1, &textureId);



		switch (createInfo.type)
		{
		case TextureType::TEXTURE_1D:
			glTextureStorage1D(textureId, createInfo.mipLevels, internalFormat, createInfo.width);
			if (data != nullptr)
			{
				glTextureSubImage1D(textureId, 0, 0, createInfo.width, pixelFormat, pixelDataType, data);
			}
			break;
		case TextureType::TEXTURE_2D:
			glTextureStorage2D(textureId, createInfo.mipLevels, internalFormat, createInfo.width, createInfo.height);
			if (data != nullptr)
			{
				glTextureSubImage2D(textureId, 0, 0, 0, createInfo.width, createInfo.height, pixelFormat, pixelDataType, data);
			}
			break;
		case TextureType::TEXTURE_3D:
			glTextureStorage3D(textureId, createInfo.mipLevels, internalFormat, createInfo.width, createInfo.height, createInfo.depth);
			if (data != nullptr)
			{
				glTextureSubImage3D(textureId, 0, 0, 0, 0, createInfo.width, createInfo.height, createInfo.depth, pixelFormat, pixelDataType, data);
			}
			break;
			//for cubemap, we need to create 6 faces
		default:
			std::cerr << "Unsupported texture type for OpenGL: " << static_cast<int>(createInfo.type) << std::endl;
			break;

		}
	





		const OpenGLTextureCreateInfo openGLTextureCreateInfo;
		openGLTextureCreateInfo.textureId = textureId;
		openGLTextureCreateInfo.pixelFormat = pixelFormat;
		openGLTextureCreateInfo.pixelDataType = pixelDataType;
		openGLTextureCreateInfo.internalFormat = internalFormat;
	

		return std::make_unique<OpenGLTexture>(openGLTextureCreateInfo);
	}



	void OpenGLRenderDevice::updateTexture(IGPUTexture* texture, const void* data, uint64_t size, uint64_t offset)
	{
		OpenGLTexture* glTexture = dynamic_cast<OpenGLTexture*>(texture);

		if (!glTexture)
		{
			throw std::runtime_error("Invalid texture type for OpenGL update.");
		}

		if (glTexture->isCurrentlyMapped())
		{
			throw std::runtime_error("Cannot update a currently mapped texture.");
		}

		glTextureSubImage2D(glTexture->getNative, 0, 0, 0, glTexture->getWidth, glTexture->getHeight, glTexture->getPixelFormat, glTexture->pixelDataType, data);
	
	//need to work on this later
		switch (glTexture->)
		{
		case TextureType::TEXTURE_1D:
			glTextureStorage1D(textureId, createInfo.mipLevels, internalFormat, createInfo.width);
			if (data != nullptr)
			{
				glTextureSubImage1D(textureId, 0, 0, createInfo.width, pixelFormat, pixelDataType, data);
			}
			break;
		case TextureType::TEXTURE_2D:
			glTextureStorage2D(textureId, createInfo.mipLevels, internalFormat, createInfo.width, createInfo.height);
			if (data != nullptr)
			{
				glTextureSubImage2D(textureId, 0, 0, 0, createInfo.width, createInfo.height, pixelFormat, pixelDataType, data);
			}
			break;
		case TextureType::TEXTURE_3D:
			glTextureStorage3D(textureId, createInfo.mipLevels, internalFormat, createInfo.width, createInfo.height, createInfo.depth);
			if (data != nullptr)
			{
				glTextureSubImage3D(textureId, 0, 0, 0, 0, createInfo.width, createInfo.height, createInfo.depth, pixelFormat, pixelDataType, data);
			}
			break;
			//for cubemap, we need to create 6 faces
		default:
			std::cerr << "Unsupported texture type for OpenGL: " << static_cast<int>(createInfo.type) << std::endl;
			break;

		}

	
	
	
	
	}

	std::unique_ptr<IGPUSampler> OpenGLRenderDevice::createSampler(const SamplerCreateInfo& samplerCreateInfo)
	{

		const GLenum  magFilter = OpenGLUtils::getGLMagFilter(samplerCreateInfo.magFilter);
		const GLenum  minFilter = OpenGLUtils::getGLMinFilter(samplerCreateInfo.minFilter, samplerCreateInfo.mipmapMode);

		const GLenum  addressModeU = OpenGLUtils::getGLAddressMode(samplerCreateInfo.addressModeU);
		const GLenum  addressModeV = OpenGLUtils::getGLAddressMode(samplerCreateInfo.addressModeV);
		const GLenum  addressModeW = OpenGLUtils::getGLAddressMode(samplerCreateInfo.addressModeW);

		const float mipLodBias =  samplerCreateInfo.mipLodBias;

		const float minLod = samplerCreateInfo.minLod;
		const float maxLod = samplerCreateInfo.maxLod;

		GLuint sampler;
		glGenSamplers(1, &sampler);



		// Example: Linear filtering for minification and magnification
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter);
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter); 
		

		// Example: Repeat wrapping for S and T texture coordinates
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, addressModeU);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, addressModeV);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, addressModeW); // For 3D textures or cube maps


		// Example: Border color for GL_CLAMP_TO_BORDER
		if (addressModeU == GL_CLAMP_TO_BORDER ||
			addressModeV == GL_CLAMP_TO_BORDER ||
			addressModeW == GL_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow border
			glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, borderColor);
		}



		// Example: Level-of-detail (LOD) bias
		glSamplerParameterf(sampler, GL_TEXTURE_LOD_BIAS,mipLodBias);

		// Example: Min/Max LOD for mipmap selection
		glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, minLod);
		glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, maxLod);



		return std::make_unique<OpenGLSampler>(sampler, samplerCreateInfo);
	}

	std::unique_ptr<IGPUShaderModule> OpenGLRenderDevice::CreateShaderModule(const ShaderModuleCreateInfo& createInfo)
	{
		//for now shader code will be in glsl format


		GLuint shaderId = 0;
		switch (createInfo.stage)
		{
		case ShaderStage::VERTEX:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderStage::FRAGMENT:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderStage::COMPUTE:
			shaderId = glCreateShader(GL_COMPUTE_SHADER);
			break;
		case ShaderStage::GEOMETRY:
			shaderId = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			throw std::runtime_error("Unsupported shader stage for OpenGL.");
		}
		if (shaderId == 0)
		{
			throw std::runtime_error("Failed to create OpenGL shader.");
		}

		const char* shaderCode = reinterpret_cast<const char*>(createInfo.spirvCode.data());

		glShaderSource(shaderId, 1, &shaderCode, nullptr);
		glCompileShader(shaderId);
		GLint compileStatus;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLint logLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> log(logLength);
			glGetShaderInfoLog(shaderId, logLength, nullptr, log.data());
			glDeleteShader(shaderId);
			throw std::runtime_error("OpenGL shader compilation failed: " + std::string(log.data()));
		}

		return std::make_unique<OpenGLShaderModule>(shaderId, createInfo.stage, createInfo.entryPoint);

	}

	std::unique_ptr<IGPUDescriptorSetLayout> OpenGLRenderDevice::CreateDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& descriptorSetLayoutBindings)
	{
		return std::make_unique<OpenGLDescriptorSetLayout>(descriptorSetLayoutBindings);
	}

	std::unique_ptr<IGPUDescriptorSet> OpenGLRenderDevice::CreateDescriptorSet(const IGPUDescriptorSetLayout& descriptorSetLayout)
	{




		return std::unique_ptr<OpenGLDescriptorSet>(static_cast<OpenGLDescriptorSetLayout>(descriptorSetLayout));
	}

	std::unique_ptr<IGPUPipelineLayout> OpenGLRenderDevice::CreatePipelineLayout(const std::vector<IGPUDescriptorSetLayout>& setLayouts, const std::vector<PushConstantRange>& pushConstantRanges)
	{



		return std::unique_ptr<OpenGLPipelineLayout>(static_cast<>(setLayouts), pushConstantRanges);
	}

	std::unique_ptr<IGPUPipelineState> OpenGLRenderDevice::CreateGraphicsPipelineState(const PipelineCreateInfo& createInfo)
	{


		const  std::vector<PipelineShaderStageCreateInfo>& shaderStages = createInfo.shaderStages;

		GLuint(shaderProgramId = glCreateProgram();


		for (auto& shaderStage : shaderStages)
		{

			glAttachShader((shaderProgramId, static_cast<OpenGLShaderModule>(shaderStage.pShaderModule).GetNativeHandle());
		}

		glLinkProgram(shaderProgramId);
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			// Handle error: Delete program, shaders, etc.
		}

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		const VertexInputLayout& vertexInputLayout = createInfo.vertexInputLayout;

	
			for (auto& attribute : vertexInputLayout.attributes)
			{
				
					glVertexAttribBinding(attribute.location, attribute.binding);
					glVertexAttribFormat(attribute.location, OpenGLUtils::getGLAttributeSize(attribute.format), OpenGLUtils::getGLAttributeType(attribute.format), OpenGLUtils::getGLVertexAttributeNormalized(attribute.format), attribute.offset);
					glEnableVertexAttribArray(attribute.location);
				
			}
		


			glBindVertexArray(0); // Unbind VAO after setup

	
			OpenGLInputAssemblyState inputAssemblyState;
			
			inputAssemblyState.topology = OpenGLUtils::getGLPrimitiveTopology(createInfo.inputAssemblyState.topology);
			
			
			OpenglRasterizationState rasterizationState;

			rasterizationState.cullMode = OpenGLUtils::getGLCullMode(createInfo.rasterizationState.cullMode);
			rasterizationState.frontFace = OpenGLUtils::getGLFrontFace(createInfo.rasterizationState.frontFace);
			rasterizationState.polygonMode = OpenGLUtils::getGLPolygonMode(createInfo.rasterizationState.polygonMode);

			OpenGLDepthStencilState depthStencilState;

			depthStencilState.depthTestEnable = createInfo.depthStencilState.depthTestEnable ? GL_TRUE : GL_FALSE;
			depthStencilState.depthWriteEnable = createInfo.depthStencilState.depthWriteEnable ? GL_TRUE : GL_FALSE;
			depthStencilState.depthCompareOp = OpenGLUtils::getGLDepthCompareOp(createInfo.depthStencilState.depthCompareOp);
			




		return std::unique_ptr<OpenGLPipelineState>();
	}



	std::unique_ptr<IGPUCommandBuffer> OpenGLRenderDevice::CreateCommandBuffer()
	{
		 

		

		return std::unique_ptr<OpenGLCommandBuffer>();
	}

	void OpenGLRenderDevice::SubmitCommandBuffers(const std::vector<TheEngine::Graphics::IGPUCommandBuffer*>& commandBuffers)
	{

		//Do nothing

	}

	std::unique_ptr < IGPUFramebuffer> OpenGLRenderDevice::CreateFramebuffer(const FramebufferCreateInfo& createInfo)
	{

		//need to make more changes to this function

		std::vector<GLuint> frameBuffers;
		//create unique fbo for each subpass
		for (int i = 0; i < createInfo.renderPass.subpasses.size(); i++)
		{
			const Subpass& subpass = createInfo.renderPass.subpasses[i];

			GLuint fbo;
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			for (int colorAttachmentIndex = 0 ; colorAttachmentIndex < subpass.colorAttachments.size(); colorAttachmentIndex++)
			{ 
			
				
				  const uint32_t attachmentIndex =  subpass.colorAttachments[colorAttachmentIndex].attachmentIndex;
				  const IGPUTextureView* pView = createInfo.attachments[attachmentIndex].pView;
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_2D, static_cast<OpenGLTexture*>(pView->GetTexture())->getNativeHandle(), 0);
				
			}

			if (subpass.hasDepthStencil)
			{
				const uint32_t depthStencilAttachmentIndex = subpass.depthStencilAttachment.attachmentIndex;
				const IGPUTextureView* pView = createInfo.attachments[depthStencilAttachmentIndex].pView;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, static_cast<OpenGLTexture*>(pView->GetTexture())->getNativeHandle(), 0);
			}

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cerr << "Framebuffer not complete: " << status << std::endl;
				//delete the vector of fbos not just one
				glDeleteFramebuffers(1, &fbo);
				return nullptr;
			}

			frameBuffers.push_back(fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind framebuffer

		}


		return std::unique_ptr<OpenGLFramebuffer>(createInfo, frameBuffers);
		
	}

	std::unique_ptr<IGPURenderPass> OpenGLRenderDevice::CreateRenderPass(const RenderPassInfo& createInfo)
	{


		return std::unique_ptr<OpenGLRenderPass>(createInfo);
	}


	






}