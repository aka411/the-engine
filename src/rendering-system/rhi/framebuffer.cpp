#include <rendering-system/rhi/framebuffer.h>
#include <algorithm> 

namespace TheEngine::RenderingSystem
{

    Framebuffer::Framebuffer(const uint32_t width, const uint32_t height) :
        m_width(width),
        m_height(height),

        m_currentColorAttachmentIndex(0),
        m_hasDepth(false),
        m_hasStencil(false),
        m_isCombinedDS(false)
    {
   

    }

    unsigned int Framebuffer::getWidth() const
    {
        return m_width; 
    }

    unsigned int Framebuffer::getHeight() const
    { 
        return m_height;
    }

    // Standard Non-MSAA approach
    void Framebuffer::addColorAttachment(const uint32_t location, const TextureHandle swapchainImage, const glm::vec4& colorClearValue, const AttachmentOps ops)
    {
        RenderAttachmentInfo info{};
        info.image = swapchainImage;            
        info.resolveImage;     
        info.imageAttachmentOps = ops;          
        info.clearValue.colour = colorClearValue;
        info.resolveMode = AttachmentResolveMode::NONE;

        m_colourAttachments[location] = info;
        m_currentColorAttachmentIndex = std::max<uint32_t>(m_currentColorAttachmentIndex, location + 1);
    }

    // MSAA approach
    void Framebuffer::addMultisampledColorTarget(uint32_t location, TextureHandle msaaBuffer, TextureHandle finalResult, const glm::vec4& colorClearValue)
    {
        RenderAttachmentInfo info{};
        info.image = msaaBuffer;                // GPU draws here (4x)
        info.resolveImage = finalResult;        // GPU resolves to here (1x Swapchain)

        info.clearValue.colour = colorClearValue;

        info.imageAttachmentOps = { AttachmentLoadOp::CLEAR, AttachmentStoreOp::DONT_CARE };

        
        info.resolveImageAttachmentOps = { AttachmentLoadOp::DONT_CARE, AttachmentStoreOp::STORE };

        info.resolveMode = AttachmentResolveMode::AVERAGE;

        m_colourAttachments[location] = info;
        m_currentColorAttachmentIndex = std::max<uint32_t>(m_currentColorAttachmentIndex, location + 1);
        m_isMSAA = true;
    }





    void Framebuffer::addDepthAttachment(const TextureHandle textureHandle,
        const float depthClearValue,
        const AttachmentOps attachmentOps)
    {
        m_hasDepth = true;
        m_isCombinedDS = false; // Reset combined flag as this is a specific depth call

        m_depthAttachment.image = textureHandle;
        m_depthAttachment.imageAttachmentOps.load = attachmentOps.load;
        m_depthAttachment.imageAttachmentOps.store = attachmentOps.store;
        m_depthAttachment.clearValue.depthStencil.depth = depthClearValue;
    }

    void Framebuffer::addStencilAttachment(const TextureHandle textureHandle,
        const uint32_t stencilClearValue,
        const AttachmentOps attachmentOps)
    {
        //NOTE : STENCIL BUFFER SHOULD NOT BE STANDALONE
        m_hasStencil = true;
        m_isCombinedDS = false; 

        m_stencilAttachment.image = textureHandle;
        m_stencilAttachment.imageAttachmentOps.load = attachmentOps.load;
        m_stencilAttachment.imageAttachmentOps.store = attachmentOps.store;
        m_stencilAttachment.clearValue.depthStencil.stencil = stencilClearValue;
    }

    void Framebuffer::setCombinedDepthStencil(const TextureHandle textureHandle,
        float depthClearValue,
        uint32_t stencilClearValue,
        const AttachmentOps depthAttachmentOps,
        const AttachmentOps stencilAttachmentOps)
    {

    
        m_hasDepth = true;
        m_hasStencil = true;
        m_isCombinedDS = true;


        m_depthAttachment.image = textureHandle;
        m_depthAttachment.imageAttachmentOps.load = depthAttachmentOps.load;
        m_depthAttachment.imageAttachmentOps.store = depthAttachmentOps.store;
        m_depthAttachment.clearValue.depthStencil.depth = depthClearValue;

        m_stencilAttachment.image = textureHandle;
        m_stencilAttachment.imageAttachmentOps.load = stencilAttachmentOps.load;
        m_stencilAttachment.imageAttachmentOps.store = stencilAttachmentOps.store;
        m_stencilAttachment.clearValue.depthStencil.stencil = stencilClearValue;


    }

    void Framebuffer::addMultisampledDepthTargetTransient(
        const TextureHandle msaaDepthBuffer,
        float depthClearValue,
        const AttachmentOps depthOps,
        AttachmentResolveMode resolveMode)
    {


        // Configure Depth
        m_depthAttachment.image = msaaDepthBuffer;
       // m_depthAttachment.resolveImage;
        m_depthAttachment.imageAttachmentOps = depthOps;
        m_depthAttachment.clearValue.depthStencil.depth = depthClearValue;
        m_depthAttachment.resolveMode = resolveMode;


        m_hasDepth = true;
        m_hasStencil = false;
        m_isCombinedDS = false;

        m_isMSAA = true;



    }


    void Framebuffer::addMultisampledDepthStencilTargetTransient(
        const TextureHandle msaaDepthBuffer,
        float depthClearValue,
        uint32_t stencilClearValue,
        const AttachmentOps depthOps,
        const AttachmentOps stencilOps,
        AttachmentResolveMode resolveMode)
    {
        
        m_depthAttachment.image = msaaDepthBuffer;
        //m_depthAttachment.resolveImage;
        m_depthAttachment.imageAttachmentOps = depthOps;
        m_depthAttachment.clearValue.depthStencil.depth = depthClearValue;
        m_depthAttachment.resolveMode = resolveMode;

        
        m_stencilAttachment.image = msaaDepthBuffer;
        m_stencilAttachment.imageAttachmentOps = stencilOps;
        m_stencilAttachment.clearValue.depthStencil.stencil = stencilClearValue;

        m_hasDepth = true;
        m_hasStencil = true;
        m_isCombinedDS = true;

        m_isMSAA = true;
    }




}