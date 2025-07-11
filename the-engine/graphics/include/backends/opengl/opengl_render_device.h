#pragma once
#include "../../igpu_render_device.h"

namespace TheEngine::Graphics
{
	class OpenGLRenderDevice : public IGPURenderDevice
	{
        ~OpenGLRenderDevice() ;

        // --- Device Lifetime and Frame Management ---
        virtual bool Initialize(uint32_t width, uint32_t height, const std::string& windowTitle) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0; // Prepare for new frame (e.g., acquire swapchain image)
        virtual void EndFrame() = 0;   // Present the rendered frame



        // --- Resource Creation ---
        virtual std::unique_ptr<IGPUBuffer>  createBuffer(const BufferDescriptor& bufferDescriptor, const void* data, const uint64_t size) override;
        virtual std::unique_ptr<IGPUTexture> CreateTexture(const TextureCreateInfo& createInfo, const void* data) override;

        virtual void updateBuffer(IGPUBuffer* buffer, const void* data, uint64_t size, uint64_t offset = 0) override;
        virtual void updateTexture(IGPUTexture* texture, const void* data, uint64_t size, uint64_t offset = 0) override;

        virtual std::unique_ptr<IGPUSampler> createSampler(const SamplerCreateInfo& samplerCreateInfo) override;

        virtual std::unique_ptr < IGPUShaderModule> CreateShaderModule(const ShaderModuleCreateInfo& createInfo) override;
	
        virtual std::unique_ptr < IGPUDescriptorSetLayout> CreateDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& descriptorSetLayoutBindings) override;
    
        virtual std::unique_ptr<IGPUDescriptorSet> CreateDescriptorSet(const IGPUDescriptorSetLayout& descriptorSetLayout) override;

        virtual std::unique_ptr<IGPUPipelineLayout> CreatePipelineLayout(
            const std::vector<IGPUDescriptorSetLayout>& setLayouts//index is set index
            ) override;//up to 256 bytes


        virtual std::unique_ptr <IGPUPipelineState> CreateGraphicsPipelineState(const PipelineCreateInfo& createInfo) override;


        virtual std::unique_ptr<IGPUCommandBuffer> CreateCommandBuffer() override;

        virtual void SubmitCommandBuffers(const std::vector<TheEngine::Graphics::IGPUCommandBuffer*>& commandBuffers) override;

        // Render Pass and Framebuffer creation
        virtual std::unique_ptr < IGPUFramebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) override;



        // Method to create a render pass object.
        virtual std::unique_ptr <IGPURenderPass> CreateRenderPass(const RenderPassInfo& createInfo) override;//consider nam


    };
}