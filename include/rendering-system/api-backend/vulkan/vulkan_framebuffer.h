#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{

    class VulkanFramebuffer : public IFramebuffer
    {

    private:

        // Internal struct to map your agnostic handles to Vulkan views
        struct NativeAttachment 
        {
            VkImageView view;
            VkFormat format;
            VkClearValue clearValue;
        };

        std::vector<NativeAttachment> m_nativeColors;
        NativeAttachment m_nativeDepthStencil;

    public:


        VulkanFrameBuffer(uint32_t width, uint32_t height) : 
            IFrameBuffer(width, height) 
        {

        }

        void setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) override 
        {
            if (index >= m_nativeColors.size()) m_nativeColors.resize(index + 1);

            // Cast agnostic texture handle to  internal Vulkan Texture object
            auto* vkTex = static_cast<VulkanTexture*>(attachment.texture.ptr);

            m_nativeColors[index].view = vkTex->getImageView();
            m_nativeColors[index].format = vkTex->getFormat();
            m_nativeColors[index].clearValue.color = {
                attachment.clearColor[0], attachment.clearColor[1],
                attachment.clearColor[2], attachment.clearColor[3]
            };
        }

        void setDepthStencilAttachment(const FramebufferAttachment& attachment) override
        {
            auto* vkTex = static_cast<VulkanTexture*>(attachment.texture.ptr);

            m_nativeDepthStencil.view = vkTex->getImageView();
            m_nativeDepthStencil.format = vkTex->getFormat();
            m_nativeDepthStencil.clearValue.depthStencil = { attachment.clearDepth, attachment.clearStencil };
            m_hasDepthStencil = true;
        }

        // Helpers for the Command Buffer
        const std::vector<NativeAttachment>& getNativeColors() const { return m_nativeColors; }
        const NativeAttachment& getNativeDepth() const { return m_nativeDepthStencil; }
    };


}