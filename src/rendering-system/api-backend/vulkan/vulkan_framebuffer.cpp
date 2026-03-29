
namespace TheEngine::RenderingSystem::VulkanBackend
{



    VulkanFramebuffer::VulkanFramebuffer(uint32_t width, uint32_t height) :
        IFramebuffer(width, height) 
    {

    }

   virtual void VulkanFramebuffer::setColorAttachment(uint32_t index, const FramebufferAttachment& attachment) override 
   {
            if (index >= m_nativeColors.size()) m_nativeColors.resize(index + 1);

            // Cast your agnostic texture handle to your internal Vulkan Texture object
            auto* vkTex = static_cast<VulkanTexture*>(attachment.texture.ptr);

            m_nativeColors[index].view = vkTex->getImageView();
            m_nativeColors[index].format = vkTex->getFormat();
            m_nativeColors[index].clearValue.color = {
                attachment.clearColor[0], attachment.clearColor[1],
                attachment.clearColor[2], attachment.clearColor[3]
            };
        }

       virtual void VulkanFramebuffer::setDepthStencilAttachment(const FramebufferAttachment& attachment) override
       {
            auto* vkTex = static_cast<VulkanTexture*>(attachment.texture.ptr);

            m_nativeDepthStencil.view = vkTex->getImageView();
            m_nativeDepthStencil.format = vkTex->getFormat();
            m_nativeDepthStencil.clearValue.depthStencil = { attachment.clearDepth, attachment.clearStencil };
            m_hasDepthStencil = true;
        }


        // Helpers for the Command Buffer
        virtual const std::vector<NativeAttachment>& VulkanFramebuffer::getNativeColors() const override
        { 
            return m_nativeColors; 
        }


        virtual const NativeAttachment& VulkanFramebuffer::getNativeDepth() const override
        { 
            return m_nativeDepthStencil; 
        }
    };


}