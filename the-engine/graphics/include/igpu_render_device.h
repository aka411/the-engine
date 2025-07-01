
#pragma once
#include <memory>
#include "igpu_buffer.h"

#include "igpu_command_buffer.h"

#include "igpu_render_pass.h"
#include "igpu_framebuffer.h"
#include <string>

#include "igpu_sampler.h"

#include "igpu_shader_module.h"
#include "igpu_texture.h"
#include "igpu_descriptor_set_layout.h"
#include "igpu_descriptor_set.h"

//Progress : 10%
//need to add more methods to this class
namespace TheEngine::Graphics
{
class IGPURenderDevice
{

public:
    virtual ~IGPURenderDevice() = default;


    //consider removing this
    // --- Device Lifetime and Frame Management ---
    virtual bool Initialize(uint32_t width, uint32_t height, const std::string& windowTitle) = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0; // Prepare for new frame (e.g., acquire swapchain image)
    virtual void EndFrame() = 0;   // Present the rendered frame
   

    // --- Resource Creation ---
    virtual std::unique_ptr<IGPUBuffer>  createBuffer(const BufferDescriptor& bufferDescriptor, const void* data,const  uint64_t size) = 0;
	virtual std::unique_ptr<IGPUTexture> CreateTexture(const TextureCreateInfo& createInfo, const void* data) = 0;
    virtual std::unique_ptr<IGPUSampler> createSampler(const SamplerCreateInfo& samplerCreateInfo) = 0;


    //will use internal command buffer for data transfer
    virtual void updateBuffer(IGPUBuffer* buffer, const void* data, uint64_t size, uint64_t offset = 0) = 0;
	virtual void updateTexture(IGPUTexture* texture, const void* data, uint64_t size, uint64_t offset = 0) = 0;
    
     // Shader creation (individual stages)
    //opnegl will compile it here
	//attaching and linking will be done in the pipeline state creation
    virtual std::unique_ptr < IGPUShaderModule> CreateShaderModule(const ShaderModuleCreateInfo& createInfo) = 0;
  




    // --- Resource Set Management (for binding resources to shaders) ---

    //used during creation of PipelineCreateInfo
    virtual std::unique_ptr < IGPUDescriptorSetLayout> CreateDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& descriptorSetLayoutBindings) = 0;//for single set layout
    //VkDescriptorSetLayout descriptorSetLayout;


    // Creates a single instance of an agnostic resource set for a specific ResourceLayout.
    // This is where Vulkan will allocate VkDescriptorSet and potentially create VkDescriptorSetLayout.
    //IGPUDescriptorSet will be populated with pointers to the actual resources (buffers, textures, samplers) that will be bound to the shader.
    virtual std::unique_ptr<IGPUDescriptorSet> CreateDescriptorSet(const IGPUDescriptorSetLayout& descriptorSetLayout) = 0;




    // Graphics Pipeline creation
    virtual std::unique_ptr <IGPUPipelineLayout> CreatePipelineLayout(
        const std::vector<IGPUDescriptorSetLayout>& setLayouts/*index is set index*/
       ) = 0;

    virtual std::unique_ptr <IGPUPipelineState> CreateGraphicsPipelineState( const PipelineCreateInfo& createInfo) = 0;




    // --- Command Buffer Management ---
    virtual std::unique_ptr<IGPUCommandBuffer> CreateCommandBuffer() = 0;
    virtual void SubmitCommandBuffers(const std::vector<TheEngine::Graphics::IGPUCommandBuffer*>& commandBuffers) = 0;

    // Render Pass and Framebuffer creation
    virtual std::unique_ptr <IGPUFramebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) = 0;



    // Method to create a render pass object.
    virtual std::unique_ptr <IGPURenderPass> CreateRenderPass(const RenderPassInfo& createInfo) = 0;//consider naming it RenderPassCreateInfo


    //--- Resource Destruction ---
    
    virtual void destroyBuffer(IGPUBuffer& buffer) = 0;
	virtual void destroyTexture(IGPUTexture& texture) = 0;
	virtual void destroySampler(IGPUSampler& sampler) = 0;
    virtual void DestroyShaderModule(IGPUShaderModule* shaderModule) = 0;

	
	virtual void destroyPipeline(IGPUPipelineState& pipeline) = 0;



	virtual void destroyRenderPass(IGPURenderPass& renderPass) = 0;
	virtual void destroyFramebuffer(IGPUFramebuffer& framebuffer) = 0;

    virtual void destroyCommandBuffer(IGPUCommandBuffer& commandBuffer) = 0;

    // Optional: Getters for unique native device context (for internal use by specific implementations)
    // For Vulkan: VkDevice
    // For OpenGL: No direct equivalent, but might return a context identifier or itself.
    // virtual void* GetNativeDeviceHandle() const = 0;





};

}